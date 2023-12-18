#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , geoLocationDownloader("6b0ddb97697f82a69e6b595cbfb04295")
    , dbManager{std::make_unique<DatabaseManager>()}
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);

    connect(&geoLocationDownloader, &GeoLocationDownloader::geoLocationDataReceived, this, &MainWindow::updateTextBox);
}

void MainWindow::on_pushButton_clicked()
{            
    if (ui->textEdit_ipAddr->toPlainText().isEmpty()) {
        QMessageBox::warning(this, "Error", "Adress ip field is empty.", QMessageBox::Ok);
        return;
    }

    geoLocationDownloader.downloadGeoLocation(ui->textEdit_ipAddr->toPlainText());
}

void MainWindow::updateTextBox(const QJsonObject &jsonData)
{
    ui->textEdit->setText(QJsonDocument(jsonData).toJson());

    ui->textEdit_City->setText(jsonData["city"].toString());
    ui->textEdit_Region->setText(jsonData["region_name"].toString());
    ui->textEdit_country->setText(jsonData["country_name"].toString());
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

    QJsonObject searchData = dbManager->searchData(ui->textEdit_ipAddr->toPlainText());

    if (searchData.isEmpty()) {
        QMessageBox::warning(this, "Error", "No data for the ip address.", QMessageBox::Ok);
    }
    else {
        ui->textEdit_City->setText(searchData["city"].toString());
        ui->textEdit_Region->setText(searchData["region"].toString());
        ui->textEdit_country->setText(searchData["country"].toString());
    }
}

void MainWindow::on_pushButton_AddToDB_clicked()
{
    if (ui->textEdit_ipAddr->toPlainText().isEmpty()) {
        QMessageBox::warning(this, "Error", "Adress ip field is empty.", QMessageBox::Ok);
        return;
    }

    bool isAdded = dbManager->insertData(ui->textEdit_ipAddr->toPlainText(), ui->textEdit_country->toPlainText(), ui->textEdit_Region->toPlainText(), ui->textEdit_City->toPlainText());
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

    if (dbManager->deleteData(ui->textEdit_ipAddr->toPlainText())) {
        QMessageBox::information(this, "Information", "Record deleted successfully.", QMessageBox::Ok);
    } else {
        QMessageBox::warning(this, "Error", "Failed to delete record.", QMessageBox::Ok);
    }
}

