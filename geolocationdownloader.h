#pragma once

#include "geolocation.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class GeoLocationDownloader : public QObject
{
    Q_OBJECT

public:
    GeoLocationDownloader(const QString &apiKey, QObject *parent = nullptr);

    void downloadGeoLocation(const QString &ipAddress);

signals:
    void geoLocationDataReceived(const GeoLocation& geoLocation);
    void geoLocationDownloaderNetworkError(const QString &error);

private slots:
    void onReplyFinished(QNetworkReply *reply);
    void onError(const QString& error);
    void processGeoLocation(const QByteArray &data);

private:
    QNetworkAccessManager manager;
    QString apiKey;
};
