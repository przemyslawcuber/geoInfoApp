#pragma once

#include "geolocation.h"

#include <string>

class SqlQueryBuilder {
public:
    static std::string buildInsertLanguageQuery(const Language& language) {
        std::stringstream query;
        query << "INSERT INTO Language (code, name, native) VALUES ('"
              << language.code << "', '"
              << language.name << "', '"
              << language.native << "');";
        return query.str();
    }

    static std::string buildInsertLocationQuery(const Location& location, int languageId) {
        std::stringstream query;
        query << "INSERT INTO Location (calling_code, capital, country_flag, country_flag_emoji, country_flag_emoji_unicode, geoname_id, is_eu, language_id) VALUES ('"
              << location.calling_code << "', '"
              << location.capital << "', '"
              << location.country_flag << "', '"
              << location.country_flag_emoji << "', '"
              << location.country_flag_emoji_unicode << "', "
              << location.geoname_id << ", "
              << location.is_eu << ", "
              << languageId << ");";
        return query.str();
    }

    static std::string buildInsertGeoLocationQuery(const GeoLocation& geoLocation, int locationId) {
        std::stringstream query;
        query << "INSERT INTO GeoLocation (city, continent_code, continent_name, country_code, country_name, ip, latitude, location_id, longitude, region_code, region_name, type, zip) VALUES ('"
              << geoLocation.city << "', '"
              << geoLocation.continent_code << "', '"
              << geoLocation.continent_name << "', '"
              << geoLocation.country_code << "', '"
              << geoLocation.country_name << "', '"
              << geoLocation.ip << "', "
              << geoLocation.latitude << ", "
              << locationId << ", "
              << geoLocation.longitude << ", '"
              << geoLocation.region_code << "', '"
              << geoLocation.region_name << "', '"
              << geoLocation.type << "', '"
              << geoLocation.zip << "');";
        return query.str();
    }
};
