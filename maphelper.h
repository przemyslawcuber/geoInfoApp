#pragma once

#include <QString>

namespace MapHelper
{
    static QString createMapUrl(double latitude, double longitude) {
        QString mapUrl = "https://www.google.com/maps?q=" +
                         QString::number(latitude, 'g', 17) + "," +
                         QString::number(longitude, 'g', 17);
        return mapUrl;
    }
}
