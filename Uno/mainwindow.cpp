#include "mainwindow.h"
#include "ui_mainwindow.h"


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
        ClickableLabel *cardLabel = new ClickableLabel(Q_NULLPTR, Qt::WindowFlags(), card->getName());
        cardLabel->setPixmap(QPixmap::fromImage(QImage(card->getAssetUrl())));
        cardLabel->setFixedSize(117, 171);
        cardLabel->move(cardLabel->x(), 50);
        cardLabel->setScaledContents(true);
        cardLabel->setCursor(Qt::PointingHandCursor);
        connect(cardLabel, &ClickableLabel::clicked, this, &MainWindow::onCardClick);

        int spacing = -50; // Negative spacing for overlap
        QSpacerItem *spacer = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        spacer->changeSize(spacing, 0); // Adjust the size of the spacer

        ui->primary_card_holder_layout->addWidget(cardLabel);
        ui->primary_card_holder_layout->addSpacerItem(spacer);
    } else {

    }
}

void MainWindow::onCardClick(QString cardName) {
    qDebug() << "test" << cardName;
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
    ui->card_stack_button->setCursor(Qt::PointingHandCursor);
    ui->start_button->setCursor(Qt::PointingHandCursor);

    ui->primary_card_holder->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->primary_card_holder->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->primary_card_holder->setFixedSize(1000, 171 + 50 + 50);
    ui->primary_card_holder->move(0, height() - ui->primary_card_holder->height());
    ui->scrollAreaWidgetContents->setFixedHeight(ui->primary_card_holder->height());

    ui->primary_card_holder->widget()->setLayout(ui->primary_card_holder_layout);

    ui->main_screen->setCurrentIndex(0);
    ui->server_error_label->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}
