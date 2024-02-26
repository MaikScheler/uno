#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "maincontroller.h"
#include "cardmodel.h"
#include "playermodel.h"

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

public slots:
    void drawCard(CardModel *card, PlayerModel *player);

private:
    void paintEvent(QPaintEvent *event);
    void onStartButtonClicked();
    void onCardStackButtonClicked();
    Ui::MainWindow *ui;
    MainController *mainController;
    void configureUi();

};
#endif // MAINWINDOW_H
