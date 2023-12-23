#include "../geolocation.h"

#include <gtest/gtest.h>

TEST(LanguageTest, ValidJson) {
    std::string jsonString = R"(
        {
            "code": "en",
            "name": "English",
            "native": "English"
        })";

    json jsonInput = json::parse(jsonString);

    Language lang = Language::from_json(jsonInput);

    EXPECT_EQ(lang.code, "en");
    EXPECT_EQ(lang.name, "English");
    EXPECT_EQ(lang.native, "English");
}

TEST(LanguageTest, InvalidJson) {
    std::string jsonString = R"(
        {
            "code": "en",
            "name": "English"
        })";

    json jsonInput = json::parse(jsonString);

    Language lang = Language::from_json(jsonInput);

    EXPECT_EQ(lang.code, "en");
    EXPECT_EQ(lang.name, "English");
    EXPECT_EQ(lang.native, "");
}

TEST(LocationTest, ValidJson) {
    std::string jsonString = R"(
        {
            "calling_code": "1",
            "capital": "Washington D.C.",
            "country_flag": "https://assets.ipstack.com/flags/us.svg",
            "country_flag_emoji": "🇺🇸",
            "country_flag_emoji_unicode": "U+1F1FA U+1F1F8",
            "geoname_id": 5375480,
            "is_eu": false,
            "languages": [
                {
                    "code": "en",
                    "name": "English",
                    "native": "English"
                }
            ]
        })";

    json jsonInput = json::parse(jsonString);

    Location loc = Location::from_json(jsonInput);

    EXPECT_EQ(loc.calling_code, "1");
    EXPECT_EQ(loc.capital, "Washington D.C.");
    EXPECT_EQ(loc.country_flag, "https://assets.ipstack.com/flags/us.svg");
    EXPECT_EQ(loc.country_flag_emoji, "🇺🇸");
    EXPECT_EQ(loc.country_flag_emoji_unicode, "U+1F1FA U+1F1F8");
    EXPECT_EQ(loc.geoname_id, 5375480);
    EXPECT_FALSE(loc.is_eu);

    EXPECT_EQ(loc.languages.size(), 1);
    EXPECT_EQ(loc.languages[0].code, "en");
    EXPECT_EQ(loc.languages[0].name, "English");
    EXPECT_EQ(loc.languages[0].native, "English");
}

TEST(LocationTest, InvalidJson) {
    std::string jsonString = R"(
        {
            "calling_code": "1",
            "capital": "Washington D.C."
        })";

    json jsonInput = json::parse(jsonString);

    Location loc = Location::from_json(jsonInput);

    EXPECT_EQ(loc.calling_code, "1");
    EXPECT_EQ(loc.capital, "Washington D.C.");

    EXPECT_EQ(loc.country_flag, "");
    EXPECT_EQ(loc.country_flag_emoji, "");
    EXPECT_EQ(loc.country_flag_emoji_unicode, "");
    EXPECT_EQ(loc.geoname_id, 0);
    EXPECT_FALSE(loc.is_eu);
    EXPECT_TRUE(loc.languages.empty());
}

TEST(GeoLocationTest, ValidJson) {
    std::string jsonString = R"(
        {
            "city": "Mountain View",
            "continent_code": "NA",
            "continent_name": "North America",
            "country_code": "US",
            "country_name": "United States",
            "ip": "172.253.122.94",
            "latitude": 37.419158935546875,
            "location": {
                "calling_code": "1",
                "capital": "Washington D.C.",
                "country_flag": "https://assets.ipstack.com/flags/us.svg",
                "country_flag_emoji": "🇺🇸",
                "country_flag_emoji_unicode": "U+1F1FA U+1F1F8",
                "geoname_id": 5375480,
                "is_eu": false,
                "languages": [
                    {
                        "code": "en",
                        "name": "English",
                        "native": "English"
                    }
                ]
            },
            "longitude": -122.07540893554688,
            "region_code": "CA",
            "region_name": "California",
            "type": "ipv4",
            "zip": "94043"
        })";

    json jsonInput = json::parse(jsonString);

    GeoLocation geo = GeoLocation::from_json(jsonInput);

    EXPECT_EQ(geo.city, "Mountain View");
    EXPECT_EQ(geo.continent_code, "NA");
    EXPECT_EQ(geo.continent_name, "North America");
    EXPECT_EQ(geo.country_code, "US");
    EXPECT_EQ(geo.country_name, "United States");
    EXPECT_EQ(geo.ip, "172.253.122.94");
    EXPECT_DOUBLE_EQ(geo.latitude, 37.419158935546875);

    EXPECT_EQ(geo.location.calling_code, "1");
    EXPECT_EQ(geo.location.capital, "Washington D.C.");

    EXPECT_DOUBLE_EQ(geo.longitude, -122.07540893554688);
    EXPECT_EQ(geo.region_code, "CA");
    EXPECT_EQ(geo.region_name, "California");
    EXPECT_EQ(geo.type, "ipv4");
    EXPECT_EQ(geo.zip, "94043");
}

TEST(GeoLocationTest, InvalidJson) {
    std::string jsonString = R"(
        {
            "city": "Mountain View",
            "continent_code": "NA",
            "country_code": "US",
            "country_name": "United States",
            "ip": "172.253.122.94",
            "latitude": 37.419158935546875,
            "location": {
                "calling_code": "1",
                "capital": "Washington D.C.",
                "country_flag_emoji": "🇺🇸",
                "country_flag_emoji_unicode": "U+1F1FA U+1F1F8",
                "geoname_id": 5375480,
                "is_eu": false,
                "languages": [
                    {
                        "code": "en",
                        "name": "English",
                        "native": "English"
                    }
                ]
            },
            "longitude": -122.07540893554688,
            "region_code": "CA",
            "region_name": "California",
            "type": "ipv4",
            "zip": "94043"
        })";

    json jsonInput = json::parse(jsonString);

    GeoLocation geo = GeoLocation::from_json(jsonInput);

    EXPECT_EQ(geo.city, "Mountain View");
    EXPECT_EQ(geo.continent_code, "NA");
    EXPECT_EQ(geo.continent_name, "");
    EXPECT_EQ(geo.country_code, "US");
    EXPECT_EQ(geo.country_name, "United States");
    EXPECT_EQ(geo.ip, "172.253.122.94");
    EXPECT_DOUBLE_EQ(geo.latitude, 37.419158935546875);

    EXPECT_EQ(geo.location.calling_code, "1");
    EXPECT_EQ(geo.location.capital, "Washington D.C.");
    EXPECT_EQ(geo.location.country_flag, "");
    EXPECT_EQ(geo.location.country_flag_emoji, "🇺🇸");
    EXPECT_EQ(geo.location.country_flag_emoji_unicode, "U+1F1FA U+1F1F8");
    EXPECT_EQ(geo.location.geoname_id, 5375480);
    EXPECT_EQ(geo.location.is_eu, false);

    ASSERT_EQ(geo.location.languages.size(), 1);
    EXPECT_EQ(geo.location.languages[0].code, "en");
    EXPECT_EQ(geo.location.languages[0].name, "English");
    EXPECT_EQ(geo.location.languages[0].native, "English");

    EXPECT_DOUBLE_EQ(geo.longitude, -122.07540893554688);
    EXPECT_EQ(geo.region_code, "CA");
    EXPECT_EQ(geo.region_name, "California");
    EXPECT_EQ(geo.type, "ipv4");
    EXPECT_EQ(geo.zip, "94043");
}
