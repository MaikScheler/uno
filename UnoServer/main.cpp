#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;

    return a.exec();
}
