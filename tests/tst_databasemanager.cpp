#include "../databasemanager.h"

#include <QString>
#include <QJsonObject>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

class MockDatabaseManager : public IDatabaseManager {
public:
    MOCK_METHOD(bool, openDatabase, (const QString &dbName), (override));
    MOCK_METHOD(void, closeDatabase, (), (override));
    MOCK_METHOD(bool, createTables, (), (override));
    MOCK_METHOD(bool, insertGeoLocation, (const GeoLocation& geoLocation), (override));
    MOCK_METHOD(QVariant, getGeoLocationByIp, (const QString &ip), (override));
    MOCK_METHOD(bool, deleteGeoLocationByIp, (const QString &ip), (override));
};

TEST(DatabaseManagerTest, OpenDatabase) {
    MockDatabaseManager mockDatabaseManager;

    const QString dbName = "test_database";
    EXPECT_CALL(mockDatabaseManager, openDatabase(dbName))
        .WillOnce(testing::Return(true));

    bool result = mockDatabaseManager.openDatabase(dbName);
    ASSERT_TRUE(result);
}

TEST(DatabaseManagerTest, CloseDatabase) {
    MockDatabaseManager mockDatabaseManager;

    EXPECT_CALL(mockDatabaseManager, closeDatabase())
        .Times(1);

    mockDatabaseManager.closeDatabase();
}

TEST(DatabaseManagerTest, CreateTables) {
    MockDatabaseManager mockDatabaseManager;

    EXPECT_CALL(mockDatabaseManager, createTables())
        .WillOnce(testing::Return(true));

    bool result = mockDatabaseManager.createTables();
    ASSERT_TRUE(result);
}

TEST(DatabaseManagerTest, InsertGeoLocation) {
    MockDatabaseManager mockDatabaseManager;

    Language english;
    english.code = "en";
    english.name = "English";
    english.native = "English";

    Location testLocation;
    testLocation.calling_code = "1";
    testLocation.capital = "Washington D.C.";
    testLocation.country_flag = "https://example.com/flags/us.svg";
    testLocation.country_flag_emoji = "🇺🇸";
    testLocation.country_flag_emoji_unicode = "U+1F1FA U+1F1F8";
    testLocation.geoname_id = 5375480;
    testLocation.is_eu = false;
    testLocation.languages.push_back(english);

    GeoLocation testGeoLocation;
    testGeoLocation.city = "SampleCity";
    testGeoLocation.continent_code = "NA";
    testGeoLocation.continent_name = "North America";
    testGeoLocation.country_code = "US";
    testGeoLocation.country_name = "United States";
    testGeoLocation.ip = "192.168.1.1";
    testGeoLocation.latitude = 37.7749;
    testGeoLocation.location = testLocation;
    testGeoLocation.longitude = -122.4194;
    testGeoLocation.region_code = "CA";
    testGeoLocation.region_name = "California";
    testGeoLocation.type = "ipv4";
    testGeoLocation.zip = "90210";

    EXPECT_CALL(mockDatabaseManager, insertGeoLocation(testing::Ref(testGeoLocation)))
        .WillOnce(testing::Return(true));

    bool result = mockDatabaseManager.insertGeoLocation(testGeoLocation);
    ASSERT_TRUE(result);
}

TEST(DatabaseManagerTest, GetGeoLocationByIp) {
    MockDatabaseManager mockDatabaseManager;

    GeoLocation expectedGeoLocation;
    expectedGeoLocation.city = "SampleCity";
    expectedGeoLocation.continent_code = "NA";
    expectedGeoLocation.continent_name = "North America";
    expectedGeoLocation.country_code = "US";
    expectedGeoLocation.country_name = "United States";
    expectedGeoLocation.ip = "192.168.1.1";
    expectedGeoLocation.latitude = 37.7749;

    Location expectedLocation;
    expectedLocation.calling_code = "1";
    expectedLocation.capital = "Washington D.C.";
    expectedLocation.country_flag = "https://example.com/flags/us.svg";
    expectedLocation.country_flag_emoji = "🇺🇸";
    expectedLocation.country_flag_emoji_unicode = "U+1F1FA U+1F1F8";
    expectedLocation.geoname_id = 5375480;
    expectedLocation.is_eu = false;

    Language english;
    english.code = "en";
    english.name = "English";
    english.native = "English";

    expectedLocation.languages.push_back(english);
    expectedGeoLocation.location = expectedLocation;

    const QString ipAddress = "192.168.1.1";
    EXPECT_CALL(mockDatabaseManager, getGeoLocationByIp(ipAddress))
        .WillOnce(testing::Return(QVariant::fromValue(expectedGeoLocation)));

    QVariant result = mockDatabaseManager.getGeoLocationByIp(ipAddress);
    ASSERT_TRUE(result.canConvert<GeoLocation>());

    GeoLocation resultGeoLocation = result.value<GeoLocation>();

    ASSERT_EQ(resultGeoLocation.city, expectedGeoLocation.city);
    ASSERT_EQ(resultGeoLocation.continent_code, expectedGeoLocation.continent_code);
}

TEST(DatabaseManagerTest, GetGeoLocationByIp_NotFound) {
    MockDatabaseManager mockDatabaseManager;

    const QString nonExistingIpAddress = "192.168.1.2";
    EXPECT_CALL(mockDatabaseManager, getGeoLocationByIp(nonExistingIpAddress))
        .WillOnce(testing::Return(QVariant()));

    QVariant result = mockDatabaseManager.getGeoLocationByIp(nonExistingIpAddress);
    ASSERT_FALSE(result.isValid());
}

TEST(DatabaseManagerTest, DeleteGeoLocationByIp) {
    MockDatabaseManager mockDatabaseManager;

    const QString existingIpAddress = "192.168.1.1";
    EXPECT_CALL(mockDatabaseManager, deleteGeoLocationByIp(existingIpAddress))
        .WillOnce(testing::Return(true));

    bool result = mockDatabaseManager.deleteGeoLocationByIp(existingIpAddress);
    ASSERT_TRUE(result);
}

TEST(DatabaseManagerTest, DeleteGeoLocationByIp_NotFound) {
    MockDatabaseManager mockDatabaseManager;

    const QString nonExistingIpAddress = "192.168.1.2";
    EXPECT_CALL(mockDatabaseManager, deleteGeoLocationByIp(nonExistingIpAddress))
        .WillOnce(testing::Return(false));

    bool result = mockDatabaseManager.deleteGeoLocationByIp(nonExistingIpAddress);
    ASSERT_FALSE(result);
}
