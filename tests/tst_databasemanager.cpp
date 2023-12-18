#include "../databasemanager.h"

#include <QString>
#include <QJsonObject>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

class MockDatabaseManager : public IDatabaseManager
{
public:
    MOCK_METHOD(bool, openDatabase, (const QString &dbName), (override));
    MOCK_METHOD(void, closeDatabase, (), (override));
    MOCK_METHOD(bool, createTable, (), (override));
    MOCK_METHOD(bool, insertData, (const QString &ip, const QString &country, const QString &region, const QString &city), (override));
    MOCK_METHOD(QJsonObject, searchData, (const QString &ip), (override));
    MOCK_METHOD(bool, deleteData, (const QString &ip), (override));
};

TEST(DatabaseManagerTest, OpenDatabase)
{
    MockDatabaseManager mockDatabaseManager;

    QString dbName = "testDatabase";
    EXPECT_CALL(mockDatabaseManager, openDatabase(dbName)).WillOnce(testing::Return(true));

    bool result = mockDatabaseManager.openDatabase(dbName);
    EXPECT_TRUE(result);
}

TEST(DatabaseManagerTest, CloseDatabase)
{
    MockDatabaseManager mockDatabaseManager;

    EXPECT_CALL(mockDatabaseManager, closeDatabase()).Times(1);

    mockDatabaseManager.closeDatabase();
}

TEST(DatabaseManagerTest, InsertData)
{
    MockDatabaseManager mockDatabaseManager;

    QString ip = "127.0.0.1";
    QString country = "TestCountry";
    QString region = "TestRegion";
    QString city = "TestCity";

    EXPECT_CALL(mockDatabaseManager, insertData(ip, country, region, city)).WillOnce(testing::Return(true));

    bool result = mockDatabaseManager.insertData(ip, country, region, city);
    EXPECT_TRUE(result);
}

TEST(DatabaseManagerTest, SearchData)
{
    MockDatabaseManager mockDatabaseManager;

    QString ipToSearch = "127.0.0.1";

    QJsonObject expectedData;
    expectedData["ip"] = ipToSearch;
    expectedData["country"] = "SampleCountry";
    expectedData["region"] = "SampleRegion";
    expectedData["city"] = "SampleCity";

    EXPECT_CALL(mockDatabaseManager, searchData(ipToSearch)).WillOnce(testing::Return(expectedData));

    QJsonObject actualData = mockDatabaseManager.searchData(ipToSearch);
    EXPECT_EQ(actualData, expectedData);
}

TEST(DatabaseManagerTest, DeleteData)
{
    MockDatabaseManager mockDatabaseManager;

    QString ipToDelete = "127.0.0.1";
    EXPECT_CALL(mockDatabaseManager, deleteData(ipToDelete)).WillOnce(testing::Return(true));

    bool result = mockDatabaseManager.deleteData(ipToDelete);
    EXPECT_TRUE(result);
}
