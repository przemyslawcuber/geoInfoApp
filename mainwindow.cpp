#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "maphelper.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

#include <QWebEngineView>

#include <spdlog/spdlog.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , geoLocationDownloader("6b0ddb97697f82a69e6b595cbfb04295") // move to config file
    , dbManager{std::make_unique<DatabaseManager>()}    // Should be replaced with factory
{
    ui->setupUi(this);

    //connect(ui->pushButton_getFromIPStack, &QPushButton::clicked, this, &MainWindow::on_pushButton_getFromIPStack_clicked);

    connect(&geoLocationDownloader, &GeoLocationDownloader::geoLocationDataReceived, this, &MainWindow::updateTextBox);
    connect(this, &MainWindow::geoLocationDataChanged, this, &MainWindow::updateTextBox);

    connect(&geoLocationDownloader, &GeoLocationDownloader::geoLocationDownloaderNetworkError, this, &MainWindow::onNetworkError);

    connect(this, &MainWindow::mapDataChanged, this, &MainWindow::updateMap);

    emit mapDataChanged(51.11733562622678, 17.022540083740783);   // Somofo as default
}

void MainWindow::on_pushButton_getFromIPStack_clicked()
{
    if (ui->textEdit_ipAddr->toPlainText().isEmpty()) {
        QMessageBox::warning(this, "Error", "Adress ip field is empty.", QMessageBox::Ok);
        return;
    }

    geoLocationDownloader.downloadGeoLocation(ui->textEdit_ipAddr->toPlainText());
}

void MainWindow::updateTextBox(const GeoLocation& geoLocation)
{
    ui->textEdit_city->setText( QString::fromStdString(geoLocation.city) );
    ui->textEdit_continent_code->setText( QString::fromStdString(geoLocation.continent_code) );
    ui->textEdit_continent_name->setText( QString::fromStdString(geoLocation.continent_name) );
    ui->textEdit_country_code->setText( QString::fromStdString(geoLocation.country_code) );
    ui->textEdit_country_name->setText( QString::fromStdString(geoLocation.country_name) );
    ui->textEdit_latitude->setText( QString::number(geoLocation.latitude, 'g', 17) );
    ui->textEdit_longitude->setText( QString::number(geoLocation.longitude, 'g', 17) );
    ui->textEdit_region_code->setText( QString::fromStdString(geoLocation.region_code) );
    ui->textEdit_region_name->setText( QString::fromStdString(geoLocation.region_name) );
    ui->textEdit_type->setText( QString::fromStdString(geoLocation.type) );
    ui->textEdit_zip->setText( QString::fromStdString(geoLocation.zip) );

    ui->textEdit_calling_code->setText( QString::fromStdString(geoLocation.location.calling_code) );
    ui->textEdit_capital->setText( QString::fromStdString(geoLocation.location.capital) );
    ui->textEdit_country_flag->setText( QString::fromStdString(geoLocation.location.country_flag) );
    ui->textEdit_country_flag_emoji->setText( QString::fromStdString(geoLocation.location.country_flag_emoji) );
    ui->textEdit_country_flag_emoji_unicode->setText( QString::fromStdString(geoLocation.location.country_flag_emoji_unicode) );
    ui->textEdit_geoname_id->setText( QString::number(geoLocation.location.geoname_id) );
    ui->textEdit_is_eu->setText( geoLocation.location.is_eu ? "Yes" : "No" );

    if (geoLocation.location.languages.size() > 0)
        ui->textEdit_language_name->setText( QString::fromStdString(geoLocation.location.languages[0].name) );

    spdlog::info("languages size: {}", geoLocation.location.languages.size());

    emit mapDataChanged(geoLocation.latitude, geoLocation.longitude);
}

void MainWindow::onNetworkError(const QString& error)
{
    QMessageBox::critical(nullptr, "Error", "Network request error: " + error);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_getFromDB_clicked()
{
    if (ui->textEdit_ipAddr->toPlainText().isEmpty()) {
        QMessageBox::warning(this, "Error", "Adress ip field is empty.", QMessageBox::Ok);
        return;
    }

    QString ipToRetrieve = ui->textEdit_ipAddr->toPlainText();

    QVariant result = dbManager->getGeoLocationByIp(ipToRetrieve);

    if (result.isValid()) {
        GeoLocation retrievedGeoLocation = result.value<GeoLocation>();
        spdlog::info("{} found in the database.", ipToRetrieve.toStdString());
        emit geoLocationDataChanged(retrievedGeoLocation);
    } else {
        QMessageBox::warning(nullptr, "Warrning", ipToRetrieve + " not found in the database.");
        spdlog::warn("{} not found in the database.", ipToRetrieve.toStdString());
    }
}

void MainWindow::on_pushButton_AddToDB_clicked()
{
    if (ui->textEdit_ipAddr->toPlainText().isEmpty()) {
        QMessageBox::warning(this, "Error", "Adress ip field is empty.", QMessageBox::Ok);
        return;
    }

    GeoLocation geoLocation;
    geoLocation.ip = ui->textEdit_ipAddr->toPlainText().toStdString();
    geoLocation.city = ui->textEdit_city->toPlainText().toStdString();
    geoLocation.continent_code = ui->textEdit_continent_code->toPlainText().toStdString();
    geoLocation.continent_name = ui->textEdit_continent_name->toPlainText().toStdString();
    geoLocation.country_code = ui->textEdit_country_code->toPlainText().toStdString();
    geoLocation.country_name = ui->textEdit_country_name->toPlainText().toStdString();
    geoLocation.latitude = ui->textEdit_latitude->toPlainText().toDouble();
    geoLocation.longitude = ui->textEdit_longitude->toPlainText().toDouble();
    geoLocation.region_code = ui->textEdit_region_code->toPlainText().toStdString();
    geoLocation.region_name = ui->textEdit_region_name->toPlainText().toStdString();
    geoLocation.type = ui->textEdit_type->toPlainText().toStdString();
    geoLocation.zip = ui->textEdit_zip->toPlainText().toStdString();
    geoLocation.location.calling_code = ui->textEdit_calling_code->toPlainText().toStdString();
    geoLocation.location.capital = ui->textEdit_capital->toPlainText().toStdString();
    geoLocation.location.country_flag = ui->textEdit_country_flag->toPlainText().toStdString();
    geoLocation.location.country_flag_emoji = ui->textEdit_country_flag_emoji->toPlainText().toStdString();
    geoLocation.location.country_flag_emoji_unicode = ui->textEdit_country_flag_emoji_unicode->toPlainText().toStdString();
    geoLocation.location.geoname_id = ui->textEdit_geoname_id->toPlainText().toInt();
    geoLocation.location.is_eu = ui->textEdit_is_eu->toPlainText().toLower() == "true";

    Language language;
    language.name = ui->textEdit_language_name->toPlainText().toStdString();
    geoLocation.location.languages.push_back(language);

    bool isAdded = dbManager->insertGeoLocation(geoLocation);
    if (isAdded) {
        QMessageBox::information(this, "Information", "New address ip has been added to database.", QMessageBox::Ok);
    }
    else {
        QMessageBox::warning(this, "Error", "Not able to add new ip to database.", QMessageBox::Ok);
    }
}

void MainWindow::on_pushButton_DeleteFromDb_clicked()
{
    if (ui->textEdit_ipAddr->toPlainText().isEmpty()) {
        QMessageBox::warning(this, "Error", "Adress ip field is empty.", QMessageBox::Ok);
        return;
    }

    QString ipToDelete = ui->textEdit_ipAddr->toPlainText();

    if (dbManager->deleteGeoLocationByIp(ipToDelete)) {
        QMessageBox::information(this, "Information", "Record deleted successfully.", QMessageBox::Ok);
    } else {
        QMessageBox::warning(this, "Error", "Failed to delete record.", QMessageBox::Ok);
    }
}

void MainWindow::updateMap(double latitude, double longitude)
{
    QWebEnginePage *page = ui->webView->page();

    QString mapURL = MapHelper::createMapUrl(latitude, longitude);

    page->load(QUrl(mapURL));
}
