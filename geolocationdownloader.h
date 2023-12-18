#ifndef GEOLOCATIONDOWNLOADER_H
#define GEOLOCATIONDOWNLOADER_H

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
    void geoLocationDataReceived(const QJsonObject &jsonData);

private slots:
    void onReplyFinished(QNetworkReply *reply);
    void processGeoLocation(const QByteArray &data);

private:
    QNetworkAccessManager manager;
    QString apiKey;
};

#endif // GEOLOCATIONDOWNLOADER_H
