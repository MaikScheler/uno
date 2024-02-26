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

    this->configureUi();

    connect(mainController, &MainController::drawCardSignal, this, &MainWindow::drawCard);

    connect(ui->start_button, &QPushButton::clicked, this, &MainWindow::onStartButtonClicked);
    connect(ui->card_stack_button, &QPushButton::clicked, this, &MainWindow::onCardStackButtonClicked);
}

// Override paintEvent
void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), QPixmap(":/assets/background-image.png"));
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

void MainWindow::drawCard(CardModel *card, PlayerModel *player) {
    if (player->getPrimary()) {
        QLabel *cardLabel = new QLabel();
        cardLabel->setPixmap(QPixmap::fromImage(QImage(card->getAssetUrl())));

        ui->primary_card_holder->setWidget(cardLabel);
    } else {

    }

    qDebug() << "draw Card test funktion: " << player->getClientId() << card->getName() << "assetUrl: " << card->getAssetUrl();
}

void MainWindow::onCardStackButtonClicked()
{
    mainController->drawCard();
}

void MainWindow::showServerError(QString error) {
    ui->server_error_label->show();
    ui->server_error_label->setText(error);
}

void MainWindow::configureUi() {
    this->setFixedSize(1000, 563);
    this->ui->card_stack_button->setCursor(Qt::PointingHandCursor);
    this->ui->start_button->setCursor(Qt::PointingHandCursor);

    this->ui->primary_card_holder->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->main_screen->setCurrentIndex(0);
    ui->server_error_label->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}
