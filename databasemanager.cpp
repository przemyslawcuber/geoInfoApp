#include "databasemanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QJsonObject>
#include <QVariant>
#include <spdlog/spdlog.h>

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent)
{
    openDatabase("geolocation.db");
    createTables();
}

DatabaseManager::~DatabaseManager()
{
    closeDatabase();
}

bool DatabaseManager::openDatabase(const QString &dbName)
{
    database = QSqlDatabase::addDatabase("QSQLITE"); // replace with factory
    database.setDatabaseName(dbName);

    if (!database.open()) {
        spdlog::error("Error: Unable to open database");
        return false;
    }

    return true;
}

void DatabaseManager::closeDatabase()
{
    if (database.isOpen()) {
        database.close();
    }
}

bool DatabaseManager::createTables()
{
    QSqlQuery query;

    if (!query.exec("CREATE TABLE IF NOT EXISTS Location ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "calling_code TEXT, "
                    "capital TEXT, "
                    "country_flag TEXT, "
                    "country_flag_emoji TEXT, "
                    "country_flag_emoji_unicode TEXT, "
                    "geoname_id INTEGER, "
                    "is_eu INTEGER)")) {
        spdlog::error("Error creating Location table: {}", query.lastError().text().toStdString());
        return false;
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS Language ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "code TEXT, "
                    "name TEXT, "
                    "native TEXT)")) {
        spdlog::error("Error creating Language table: {}", query.lastError().text().toStdString());
        return false;
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS GeoLocation ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "city TEXT, "
                    "continent_code TEXT, "
                    "continent_name TEXT, "
                    "country_code TEXT, "
                    "country_name TEXT, "
                    "ip TEXT, "
                    "latitude REAL, "
                    "location_id INTEGER, "
                    "longitude REAL, "
                    "region_code TEXT, "
                    "region_name TEXT, "
                    "type TEXT, "
                    "zip TEXT, "
                    "FOREIGN KEY(location_id) REFERENCES Location(id))")) {
        spdlog::error("Error creating GeoLocation table: {}", query.lastError().text().toStdString());
        return false;
    }

    return true;
}

bool DatabaseManager::insertGeoLocation(const GeoLocation& geoLocation)
{
    QSqlDatabase::database().transaction();

    int locationId = findLocationId(geoLocation.location);

    if (locationId == -1) {
        locationId = insertLocation(geoLocation.location);
    }

    QSqlQuery query;
    query.prepare("INSERT INTO GeoLocation (city, continent_code, continent_name, "
                  "country_code, country_name, ip, latitude, location_id, longitude, "
                  "region_code, region_name, type, zip) "
                  "VALUES (:city, :continent_code, :continent_name, "
                  ":country_code, :country_name, :ip, :latitude, :location_id, "
                  ":longitude, :region_code, :region_name, :type, :zip)");
    query.bindValue(":city", QVariant::fromValue<QString>(QString::fromStdString(geoLocation.city)));
    query.bindValue(":continent_code", QVariant::fromValue<QString>(QString::fromStdString(geoLocation.continent_code)));
    query.bindValue(":continent_name", QVariant::fromValue<QString>(QString::fromStdString(geoLocation.continent_name)));
    query.bindValue(":country_code", QVariant::fromValue<QString>(QString::fromStdString(geoLocation.country_code)));
    query.bindValue(":country_name", QVariant::fromValue<QString>(QString::fromStdString(geoLocation.country_name)));
    query.bindValue(":ip", QVariant::fromValue<QString>(QString::fromStdString(geoLocation.ip)));
    query.bindValue(":latitude", QVariant::fromValue<double>(geoLocation.latitude));
    query.bindValue(":location_id", QVariant::fromValue<int>(locationId));
    query.bindValue(":longitude", QVariant::fromValue<double>(geoLocation.longitude));
    query.bindValue(":region_code", QVariant::fromValue<QString>(QString::fromStdString(geoLocation.region_code)));
    query.bindValue(":region_name", QVariant::fromValue<QString>(QString::fromStdString(geoLocation.region_name)));
    query.bindValue(":type", QVariant::fromValue<QString>(QString::fromStdString(geoLocation.type)));
    query.bindValue(":zip", QVariant::fromValue<QString>(QString::fromStdString(geoLocation.zip)));

    if (!query.exec()) {
        QSqlDatabase::database().rollback();
        spdlog::error("Error inserting GeoLocation: {}", query.lastError().text().toStdString());
        return false;
    }

    QSqlDatabase::database().commit();

    return true;
}

int DatabaseManager::findLocationId(const Location& loc) {
    QSqlQuery query;
    query.prepare("SELECT id FROM Location "
                  "WHERE calling_code = :calling_code "
                  "AND capital = :capital "
                  "AND country_flag = :country_flag "
                  "AND country_flag_emoji = :country_flag_emoji "
                  "AND country_flag_emoji_unicode = :country_flag_emoji_unicode "
                  "AND geoname_id = :geoname_id "
                  "AND is_eu = :is_eu");
    query.bindValue(":calling_code", QVariant::fromValue<QString>(QString::fromStdString(loc.calling_code)));
    query.bindValue(":capital", QVariant::fromValue<QString>(QString::fromStdString(loc.capital)));
    query.bindValue(":country_flag", QVariant::fromValue<QString>(QString::fromStdString(loc.country_flag)));
    query.bindValue(":country_flag_emoji", QVariant::fromValue<QString>(QString::fromStdString(loc.country_flag_emoji)));
    query.bindValue(":country_flag_emoji_unicode", QVariant::fromValue<QString>(QString::fromStdString(loc.country_flag_emoji_unicode)));
    query.bindValue(":geoname_id", QVariant::fromValue<int>(loc.geoname_id));
    query.bindValue(":is_eu", QVariant::fromValue<bool>(loc.is_eu));

    if (!query.exec()) {
        spdlog::error("Error searching for Location: {}", query.lastError().text().toStdString());
        return -1;
    }

    if (query.next()) {
        return query.value("id").toInt();
    }

    return -1;
}

int DatabaseManager::insertLocation(const Location& loc) {
    QSqlQuery query;
    query.prepare("INSERT INTO Location (calling_code, capital, country_flag, "
                  "country_flag_emoji, country_flag_emoji_unicode, geoname_id, is_eu) "
                  "VALUES (:calling_code, :capital, :country_flag, "
                  ":country_flag_emoji, :country_flag_emoji_unicode, :geoname_id, :is_eu)");
    query.bindValue(":calling_code", QVariant::fromValue<QString>(QString::fromStdString(loc.calling_code)));
    query.bindValue(":capital", QVariant::fromValue<QString>(QString::fromStdString(loc.capital)));
    query.bindValue(":country_flag", QVariant::fromValue<QString>(QString::fromStdString(loc.country_flag)));
    query.bindValue(":country_flag_emoji", QVariant::fromValue<QString>(QString::fromStdString(loc.country_flag_emoji)));
    query.bindValue(":country_flag_emoji_unicode", QVariant::fromValue<QString>(QString::fromStdString(loc.country_flag_emoji_unicode)));
    query.bindValue(":geoname_id", QVariant::fromValue<int>(loc.geoname_id));
    query.bindValue(":is_eu", QVariant::fromValue<bool>(loc.is_eu));

    if (!query.exec()) {
        spdlog::error("Error inserting Location: {}", query.lastError().text().toStdString());
        return -1;
    }

    int locationId = query.lastInsertId().toInt();

    for (const Language& lang : loc.languages) {
        int languageId = insertLanguage(lang);
        if (languageId != -1) {
            insertLocationLanguageLink(locationId, languageId);
        }
    }

    return locationId;
}

int DatabaseManager::insertLanguage(const Language& lang) {
    QSqlQuery query;
    query.prepare("INSERT INTO Language (code, name, native) "
                  "VALUES (:code, :name, :native)");
    query.bindValue(":code", QVariant::fromValue<QString>(QString::fromStdString(lang.code)));
    query.bindValue(":name", QVariant::fromValue<QString>(QString::fromStdString(lang.name)));
    query.bindValue(":native", QVariant::fromValue<QString>(QString::fromStdString(lang.native)));

    if (!query.exec()) {
        spdlog::error("Error inserting Language: {}", query.lastError().text().toStdString());
        return -1;
    }

    return query.lastInsertId().toInt();
}

void DatabaseManager::insertLocationLanguageLink(int locationId, int languageId) {
    QSqlQuery linkQuery;
    linkQuery.prepare("INSERT INTO LocationLanguage (location_id, language_id) "
                      "VALUES (:location_id, :language_id)");
    linkQuery.bindValue(":location_id", QVariant::fromValue<int>(locationId));
    linkQuery.bindValue(":language_id", QVariant::fromValue<int>(languageId));

    if (!linkQuery.exec()) {
        spdlog::error("Error inserting LocationLanguage link: {}", linkQuery.lastError().text().toStdString());
    }
}

bool DatabaseManager::checkLanguageExists(QSqlQuery& query, const QString& languageName) {
    query.prepare("SELECT id FROM Language WHERE name = :name");
    query.bindValue(":name", languageName);

    if (!query.exec()) {
        spdlog::error("Error checking language existence: {}", query.lastError().text().toStdString());
        return false;
    }

    return query.next();
}

QVariant DatabaseManager::getGeoLocationByIp(const QString &ip)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM GeoLocation WHERE ip = :ip");
    query.bindValue(":ip", ip);

    GeoLocation geoLocation;

    if (!query.exec()) {
        spdlog::error("Error retrieving GeoLocation: {}", query.lastError().text().toStdString());
        return QVariant();
    }

    if (query.next()) {
        geoLocation.city = query.value("city").toString().toStdString();
        geoLocation.continent_code = query.value("continent_code").toString().toStdString();
        geoLocation.continent_name = query.value("continent_name").toString().toStdString();
        geoLocation.country_code = query.value("country_code").toString().toStdString();
        geoLocation.country_name = query.value("country_name").toString().toStdString();
        geoLocation.ip = query.value("ip").toString().toStdString();
        geoLocation.latitude = query.value("latitude").toDouble();
        geoLocation.location = getLocationById(query.value("location_id").toInt());
        geoLocation.longitude = query.value("longitude").toDouble();
        geoLocation.region_code = query.value("region_code").toString().toStdString();
        geoLocation.region_name = query.value("region_name").toString().toStdString();
        geoLocation.type = query.value("type").toString().toStdString();
        geoLocation.zip = query.value("zip").toString().toStdString();

        return QVariant::fromValue(geoLocation);
    }

    return QVariant();
}

Location DatabaseManager::getLocationById(int locationId) {
    QSqlQuery query;
    query.prepare("SELECT * FROM Location WHERE id = :id");
    query.bindValue(":id", locationId);

    Location location;

    if (!query.exec()) {
        spdlog::error("Error retrieving Location: {}", query.lastError().text().toStdString());
        return location;
    }

    if (query.next()) {
        location.calling_code = query.value("calling_code").toString().toStdString();
        location.capital = query.value("capital").toString().toStdString();
        location.country_flag = query.value("country_flag").toString().toStdString();
        location.country_flag_emoji = query.value("country_flag_emoji").toString().toStdString();
        location.country_flag_emoji_unicode = query.value("country_flag_emoji_unicode").toString().toStdString();
        location.geoname_id = query.value("geoname_id").toInt();
        location.is_eu = query.value("is_eu").toBool();

        location.languages = getLanguagesForLocation(locationId);
    }

    return location;
}

std::vector<Language> DatabaseManager::getLanguagesForLocation(int locationId) {
    QSqlQuery query;
    query.prepare("SELECT * FROM Language WHERE location_id = :location_id");
    query.bindValue(":location_id", locationId);

    std::vector<Language> languages;

    if (!query.exec()) {
        spdlog::error("Error retrieving Languages for Location: {}", query.lastError().text().toStdString());
        return languages;
    }

    while (query.next()) {
        Language language;
        language.code = query.value("code").toString().toStdString();
        language.name = query.value("name").toString().toStdString();
        language.native = query.value("native").toString().toStdString();
        languages.push_back(language);
    }

    return languages;
}

bool DatabaseManager::deleteGeoLocationByIp(const QString &ip)
{
    QSqlDatabase::database().transaction();

    int geoLocationId = findGeoLocationIdByIp(ip);

    if (geoLocationId == -1) {
        spdlog::error("GeoLocation with IP {} not found.", ip.toStdString());
        QSqlDatabase::database().rollback();
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM GeoLocation WHERE id = :id");    // delete only GeoLocation. The rest should stay because it might be linked to another rows.
    query.bindValue(":id", geoLocationId);

    if (!query.exec()) {
        QSqlDatabase::database().rollback();
        spdlog::error("Error deleting GeoLocation: {}", query.lastError().text().toStdString());
        return false;
    }

    QSqlDatabase::database().commit();

    return true;
}

int DatabaseManager::findGeoLocationIdByIp(const QString &ip) {
    QSqlQuery query;
    query.prepare("SELECT id FROM GeoLocation WHERE ip = :ip");
    query.bindValue(":ip", ip);

    if (!query.exec()) {
        spdlog::error("Error searching for GeoLocation: {}", query.lastError().text().toStdString());
        return -1;
    }

    if (query.next()) {
        return query.value("id").toInt();
    }

    return -1;
}
