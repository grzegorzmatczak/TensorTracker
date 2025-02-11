#include <QCoreApplication>
#include <QString>

#include "global.h"
#include "mainlogger.hpp"
#include "logger.hpp"
#include "configreader.hpp"
#include "service.h"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    MainLogger::start();

    ConfigReader configReader;
    QJsonObject config;
    configReader.readConfig(QString::fromStdString(CONFIG), config);
    Service service{ config };
    return app.exec();
}