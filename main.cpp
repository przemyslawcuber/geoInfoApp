#include "mainwindow.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto file_logger = spdlog::basic_logger_mt("file_logger", "test_log.txt");
    spdlog::set_default_logger(file_logger);

    spdlog::flush_every(std::chrono::seconds(1));
    spdlog::set_level(spdlog::level::debug);

    spdlog::info("Starting Geo location app");

    MainWindow w;
    w.show();
    return a.exec();
}
