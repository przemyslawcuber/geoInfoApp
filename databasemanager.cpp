#include "databasemanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QJsonObject>
#include <spdlog/spdlog.h>

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent)
{
    openDatabase("geolocation.db");
    createTable();
}

DatabaseManager::~DatabaseManager()
{
    closeDatabase();
}

bool DatabaseManager::openDatabase(const QString &dbName)
{
    database = QSqlDatabase::addDatabase("QSQLITE");
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

bool DatabaseManager::createTable()
{
    QSqlQuery query;
    return query.exec("CREATE TABLE IF NOT EXISTS geolocation (ip TEXT, country TEXT, region TEXT, city TEXT)");
}

bool DatabaseManager::insertData(const QString &ip, const QString &country, const QString &region, const QString &city)
{
    spdlog::info("Add to db ip {}, country {} region {} city {}", ip.toStdString(), country.toStdString(), region.toStdString(), city.toStdString());

    QSqlQuery query;
    query.prepare("INSERT INTO geolocation (ip, country, region, city) VALUES (?, ?, ?, ?)");
    query.addBindValue(ip);
    query.addBindValue(country);
    query.addBindValue(region);
    query.addBindValue(city);

    return query.exec();
}

QJsonObject DatabaseManager::searchData(const QString &ip)
{
    QJsonObject dataObject;

    QSqlQuery query;
    query.prepare("SELECT * FROM geolocation WHERE ip = ?");
    query.addBindValue(ip);

    if (query.exec() && query.next()) {
        dataObject["ip"] = query.value("ip").toString();
        dataObject["country"] = query.value("country").toString();
        dataObject["region"] = query.value("region").toString();
        dataObject["city"] = query.value("city").toString();
    }

    return dataObject;
}

bool DatabaseManager::deleteData(const QString &ip)
{
    QSqlQuery query;
    query.prepare("DELETE FROM geolocation WHERE ip = ?");
    query.addBindValue(ip);

    if (query.exec()) {
        spdlog::info("Deleted data for ip {}", ip.toStdString());
        return true;
    } else {
        spdlog::info("Failed to delete record: {}", query.lastError().text().toStdString());
        return false;
    }
}
