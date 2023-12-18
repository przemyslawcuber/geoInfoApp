#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>

class IDatabaseManager
{
public:
    virtual bool openDatabase(const QString &dbName) = 0;
    virtual void closeDatabase() = 0;
    virtual bool createTable() = 0;
    virtual bool insertData(const QString &ip, const QString &country, const QString &region, const QString &city) = 0;
    virtual QJsonObject searchData(const QString &ip) = 0;
    virtual bool deleteData(const QString &ip) = 0;
};

class DatabaseManager : public QObject, public IDatabaseManager
{
    Q_OBJECT

public:
    explicit DatabaseManager(QObject *parent = nullptr);
    virtual ~DatabaseManager();

    bool openDatabase(const QString &dbName) override;
    void closeDatabase() override;
    bool createTable() override;
    bool insertData(const QString &ip, const QString &country, const QString &region, const QString &city) override;
    QJsonObject searchData(const QString &ip) override;
    bool deleteData(const QString &ip) override;

private:
    QSqlDatabase database;
};

#endif // DATABASEMANAGER_H
