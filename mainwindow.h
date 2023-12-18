#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "geolocationdownloader.h"
#include "databasemanager.h"

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

public slots:
    void updateTextBox(const QJsonObject &jsonData);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_getFromDB_clicked();

    void on_pushButton_AddToDB_clicked();

    void on_pushButton_DeleteFromDb_clicked();

private:
    Ui::MainWindow *ui;
    GeoLocationDownloader geoLocationDownloader;
    std::unique_ptr<IDatabaseManager> dbManager;
};
#endif // MAINWINDOW_H
