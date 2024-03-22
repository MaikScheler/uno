#include "mainwindow.h"
#include "maincontroller.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainController *main_controller = new MainController(0);
    MainWindow *w = new MainWindow(nullptr, main_controller);
    w->show();
    return a.exec();
}
