#pragma once

#include <QMainWindow>
#include "geolocationdownloader.h"
#include "databasemanager.h"
#include "geolocation.h"

#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void mapDataChanged(double latitude, double longitude);
    void geoLocationDataChanged(const GeoLocation& geoLocation);

public slots:
    void updateTextBox(const GeoLocation& geoLocation);

private slots:
    void onNetworkError(const QString &error);

    void on_pushButton_getFromIPStack_clicked();

    void on_pushButton_getFromDB_clicked();

    void on_pushButton_AddToDB_clicked();

    void on_pushButton_DeleteFromDb_clicked();

    void updateMap(double latitude, double longitude);

private:
    Ui::MainWindow *ui;
    GeoLocationDownloader geoLocationDownloader;
    std::unique_ptr<IDatabaseManager> dbManager;
};
