#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedSize(1000, 563);

    connect(ui->start_button, &QPushButton::clicked, this, &MainWindow::onStartButtonClicked);
}

// Override paintEvent
void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), QPixmap(":/assets/assets/background-image.png"));
}

void MainWindow::onStartButtonClicked() {
    ui->main_screen->setCurrentIndex(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}
