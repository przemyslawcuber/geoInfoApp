QT += core gui sql testlib widgets network webkitwidgets webenginewidgets

CONFIG += c++17 testcase

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -lspdlog -lfmt

SOURCES += \
    databasemanager.cpp \
    geolocationdownloader.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    databasemanager.h \
    geolocation.h \
    geolocationdownloader.h \
    mainwindow.h \
    maphelper.h \
    networkhelper.h \
    sqlquerybuilder.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
