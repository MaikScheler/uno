#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "maincontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, MainController *mainController = NULL);
    ~MainWindow();
    void showServerError(QString error);
signals:
    void connectionFailed();

private:
    void paintEvent(QPaintEvent *event);
    void onStartButtonClicked();
    Ui::MainWindow *ui;
    MainController *mainController;

};
#endif // MAINWINDOW_H
