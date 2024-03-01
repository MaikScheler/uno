#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include "server.h"
#include <iostream>

int main(int argc, char *argv[])
{
    srand(time(NULL));
    QCoreApplication a(argc, argv);

    Server server;

    return a.exec();
}
