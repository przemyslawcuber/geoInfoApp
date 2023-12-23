#include "geolocationdownloader.h"

#include "networkhelper.h"
#include "geolocation.h"

#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QCoreApplication>

#include <spdlog/spdlog.h>

GeoLocationDownloader::GeoLocationDownloader(const QString &apiKey, QObject *parent)
    : QObject(parent), apiKey(apiKey)
{
    connect(&manager, &QNetworkAccessManager::finished, this, &GeoLocationDownloader::onReplyFinished);
}

void GeoLocationDownloader::downloadGeoLocation(const QString &ipAddress)
{
    spdlog::info("downloadGeoLocation");
    QUrl url("http://api.ipstack.com/" + ipAddress);

    QUrlQuery urlQuery;
    urlQuery.addQueryItem("access_key", apiKey);
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    manager.get(request);
}

void GeoLocationDownloader::onReplyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray data = reply->readAll();
        processGeoLocation(data);
    }
    else
    {
        spdlog::error("Error: {}", reply->errorString().toStdString());
        onError( NetworkHelper::networkErrorToString(reply->error()) );
    }
    reply->deleteLater();
}

void GeoLocationDownloader::onError(const QString& error) {
    spdlog::error("GeoLocationDownloader error: {}", error.toStdString());

    emit geoLocationDownloaderNetworkError( error );
}

void GeoLocationDownloader::processGeoLocation(const QByteArray &data)
{
    spdlog::info("processGeoLocation");

    try {
        nlohmann::json json = nlohmann::json::parse(data.toStdString());

        if (json.contains("error")) {
            nlohmann::json error = json["error"];

            if (error.contains("info")) {
                std::string errorInfo = error["info"];
                spdlog::info("Error Info: {}", errorInfo);
                onError( QString::fromStdString(errorInfo) );
                return;
            }
        }

        GeoLocation geoLocation = GeoLocation::from_json( json );
        emit geoLocationDataReceived(geoLocation);
    } catch (const std::exception& e) {
        QString errorMessage = QString::fromStdString(e.what());
        QString errorInfo = "Error parsing JSON: " + errorMessage;
        spdlog::error(errorInfo.toStdString());
        onError( errorInfo );
    }
}
