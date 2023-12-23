#pragma once

#include <QMetaType>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Language {
    std::string code;
    std::string name;
    std::string native;

    static Language from_json(const json& j) {
        Language lang;
        lang.code = j.value("code", "");
        lang.name = j.value("name", "");
        lang.native = j.value("native", "");
        return lang;
    }
};

struct Location {
    std::string calling_code;
    std::string capital;
    std::string country_flag;
    std::string country_flag_emoji;
    std::string country_flag_emoji_unicode;
    int geoname_id;
    bool is_eu;
    std::vector<Language> languages;

    static Location from_json(const json& j) {
        Location loc;
        loc.calling_code = j.value("calling_code", "");
        loc.capital = j.value("capital", "");
        loc.country_flag = j.value("country_flag", "");
        loc.country_flag_emoji = j.value("country_flag_emoji", "");
        loc.country_flag_emoji_unicode = j.value("country_flag_emoji_unicode", "");
        loc.geoname_id = j.value("geoname_id", 0);
        loc.is_eu = j.value("is_eu", false);

        if (j.contains("languages") && j["languages"].is_array()) {
            const auto& languagesArray = j["languages"];
            loc.languages.clear();

            for (const auto& langJson : languagesArray) {
                loc.languages.push_back(Language::from_json(langJson));
            }
        } else {
            loc.languages.clear();
        }

        return loc;
    }
};

struct GeoLocation {
    std::string city;
    std::string continent_code;
    std::string continent_name;
    std::string country_code;
    std::string country_name;
    std::string ip;
    double latitude;
    Location location;
    double longitude;
    std::string region_code;
    std::string region_name;
    std::string type;
    std::string zip;

    static GeoLocation from_json(const json& j) {
        GeoLocation geo;

        geo.city = j.value("city", "");
        geo.continent_code = j.value("continent_code", "");
        geo.continent_name = j.value("continent_name", "");
        geo.country_code = j.value("country_code", "");
        geo.country_name = j.value("country_name", "");
        geo.ip = j.value("ip", "");
        geo.latitude = j.value("latitude", 0.0);
        geo.location = Location::from_json(j.value("location", json()));
        geo.longitude = j.value("longitude", 0.0);
        geo.region_code = j.value("region_code", "");
        geo.region_name = j.value("region_name", "");
        geo.type = j.value("type", "");
        geo.zip = j.value("zip", "");

        return geo;
    }
};

Q_DECLARE_METATYPE(GeoLocation)
