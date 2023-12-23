#pragma once

#include "geolocation.h"

#include <QObject>
#include <QSqlDatabase>
#include <QVariant>

class IDatabaseManager
{
public:
    virtual bool openDatabase(const QString &dbName) = 0;
    virtual void closeDatabase() = 0;
    virtual bool createTables() = 0;
    virtual bool insertGeoLocation(const GeoLocation& geoLocation) = 0;
    virtual QVariant getGeoLocationByIp(const QString &ip) = 0;
    virtual bool deleteGeoLocationByIp(const QString &ip) = 0;
};

class DatabaseManager : public QObject, public IDatabaseManager
{
    Q_OBJECT

public:
    explicit DatabaseManager(QObject *parent = nullptr);
    virtual ~DatabaseManager();

    /** openDatabase, closeDatabase and createTables should be hiden */
    bool openDatabase(const QString &dbName) override;
    void closeDatabase() override;
    bool createTables() override;

    bool insertGeoLocation(const GeoLocation& geoLocation) override;
    QVariant getGeoLocationByIp(const QString &ip) override;
    bool deleteGeoLocationByIp(const QString &ip) override;

private:
    int findLocationId(const Location& loc);
    int insertLocation(const Location& loc);

    int findGeoLocationIdByIp(const QString &ip);

    Location getLocationById(int locationId) ;

    std::vector<Language> getLanguagesForLocation(int locationId);
    int insertLanguage(const Language& lang);
    void insertLocationLanguageLink(int locationId, int languageId);

    bool checkLanguageExists(QSqlQuery& query, const QString& languageName);

private:
    QSqlDatabase database;
};
