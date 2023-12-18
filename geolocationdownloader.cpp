#include "geolocationdownloader.h"
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
    }

    reply->deleteLater();
}

void GeoLocationDownloader::processGeoLocation(const QByteArray &data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

    if (!jsonDoc.isNull())
    {
        QJsonObject jsonObj = jsonDoc.object();

        emit geoLocationDataReceived(jsonObj);
    }
    else
    {
        spdlog::error("Failed to parse JSON data");
    }
}
