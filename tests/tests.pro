include(gtest_dependency.pri)

QT       += core sql testlib

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += thread
CONFIG += qt

SOURCES += \
        main.cpp         \
        tst_databasemanager.cpp \
        tst_geolocation.cpp
