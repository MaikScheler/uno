#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent, MainController *mainController)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->mainController = mainController;
    ui->setupUi(this);

    this->setFixedSize(1000, 563);

    ui->main_screen->setCurrentIndex(0);
    ui->server_error_label->hide();

    connect(ui->start_button, &QPushButton::clicked, this, &MainWindow::onStartButtonClicked);
    connect(ui->card_stack_button, &QPushButton::clicked, this, &MainWindow::onCardStackButtonClicked);
}

// Override paintEvent
void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), QPixmap(":/assets/assets/background-image.png"));
}

void MainWindow::onStartButtonClicked() {
    ui->start_button->setDisabled(true);
    bool isConntected = mainController->start("127.0.0.1", 8888);

    qDebug() << isConntected;

    if (isConntected) {
        ui->main_screen->setCurrentIndex(1);
    } else {
        ui->server_error_label->show();
        ui->server_error_label->setText("Connection Failed");
    }

    ui->start_button->setDisabled(false);
}

void MainWindow::onCardStackButtonClicked()
{
    mainController->drawCard();
}

void MainWindow::showServerError(QString error) {
    ui->server_error_label->show();
    ui->server_error_label->setText(error);
}

MainWindow::~MainWindow()
{
    delete ui;
}
