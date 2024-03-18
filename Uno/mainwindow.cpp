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
    connect(mainController, &MainController::playCardSignal, this, &MainWindow::playCard);
    connect(mainController, &MainController::removeEnemyCardSignal, this, &MainWindow::removeEnemyCard);
    connect(mainController, &MainController::removePlayedCardSignal, this, &MainWindow::removePlayedCard);

    connect(ui->start_button, &QPushButton::clicked, this, &MainWindow::onStartButtonClicked);
    connect(ui->card_stack_button, &QPushButton::clicked, this, &MainWindow::onCardStackButtonClicked);
    connect(ui->skip_button, &QPushButton::clicked, this, &MainWindow::skipTurn);
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

void MainWindow::drawCard(QString cardId, QString cardName) {
    //if (player->getPrimary()) {
    if(cardName != "back")
    {
        ClickableLabel *cardLabel = new ClickableLabel(Q_NULLPTR, Qt::WindowFlags(), cardId.toInt());
        cardLabel->setObjectName(cardId);
        cardLabel->setPixmap(QPixmap::fromImage(QImage(":/assets/" + cardName + ".png")));
        cardLabel->setFixedSize(117, 171);
        cardLabel->move(cardLabel->x(), 50);
        cardLabel->setScaledContents(true);
        cardLabel->setCursor(Qt::PointingHandCursor);
        connect(cardLabel, &ClickableLabel::clicked, this, &MainWindow::onCardClick);

        int spacing = -20; // Negative spacing for overlap
        QSpacerItem *spacer = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        spacer->changeSize(spacing, 0); // Adjust the size of the spacer

        ui->primary_card_holder_layout->addWidget(cardLabel);
        ui->primary_card_holder_layout->addSpacerItem(spacer);

        ui->skip_button->setVisible(true);
    } else {
        enemyCardCounter++;

        if (enemyCardCounter <= 7) {
            QLabel *enemyCardLabel = new QLabel();
            enemyCardLabel->setPixmap(QPixmap::fromImage(QImage(":/assets/back.png")));
            enemyCardLabel->setFixedSize(117, 171);
            enemyCardLabel->move(enemyCardLabel->x(), 50);
            enemyCardLabel->setScaledContents(true);

            int spacing = -60; // Negative spacing for overlap
            QSpacerItem *spacer = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
            spacer->changeSize(spacing, 0); // Adjust the size of the spacer

            ui->enemy_card_holder_layout->addWidget(enemyCardLabel);
            ui->enemy_card_holder_layout->addSpacerItem(spacer);
        }

        ui->enemy_card_counter->setText("Spieler 2 hat " + QString::number(enemyCardCounter) + " Karten");
    }
}

void MainWindow::removeEnemyCard()
{

    if(enemyCardCounter <= 7)
    {
        ui->enemy_card_holder_layout->removeItem(ui->enemy_card_holder_layout->itemAt(1));
        delete ui->enemy_card_holder_layout->itemAt(0)->widget();
    }

    enemyCardCounter--;

    ui->enemy_card_counter->setText("Spieler 2 hat " + QString::number(enemyCardCounter) + " Karten");
}

void MainWindow::removePlayedCard(QString cardId)
{
    for (int i = 0; i < ui->primary_card_holder_layout->count(); ++i)
    {
        ClickableLabel *widget = (ClickableLabel*)ui->primary_card_holder_layout->itemAt(i)->widget();
        if (widget != NULL && widget->objectName() == cardId)
        {
            qDebug() << widget->objectName();
            ui->primary_card_holder_layout->removeItem(ui->primary_card_holder_layout->itemAt(ui->primary_card_holder_layout->indexOf(widget) + 1));
            delete widget;
        }
        else
        {
            // You may want to recurse, or perform different actions on layouts.
            // See gridLayout->itemAt(i)->layout()
        }
    }

}

void MainWindow::playCard(QString cardId, QString cardName)
{
    ui->played_card->setFixedSize(117, 171);
    ui->played_card->setPixmap(QPixmap::fromImage(QImage(":/assets/" + cardName + ".png")));
    ui->played_card->setScaledContents(true);

    ui->skip_button->setVisible(false);
}

void MainWindow::onCardClick(int cardId, ClickableLabel *cardLabel) {
    //ui->primary_card_holder_layout->removeItem(ui->primary_card_holder_layout->itemAt(ui->primary_card_holder_layout->indexOf(cardLabel) + 1));
    //delete cardLabel;

    qDebug() << "Card clicked" << cardId;
    mainController->playCard(cardId);
}

void MainWindow::skipTurn() {
    this->mainController->skipTurn();
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
    ui->primary_card_holder_layout->setStretch(0,1);
    ui->primary_card_holder_layout->setAlignment(Qt::AlignLeft);

    ui->primary_card_holder->widget()->setLayout(ui->primary_card_holder_layout);


    ui->enemy_card_holder->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->enemy_card_holder->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->enemy_card_holder->setFixedSize(800, 171 + 50 + 50);
    ui->enemy_card_holder->move(0, -150);
    ui->enemy_card_holder_layout->setStretch(0,1);
    ui->enemy_card_holder_layout->setAlignment(Qt::AlignLeft);
    ui->scrollAreaWidgetContents_Enemy->setFixedHeight(ui->enemy_card_holder->height());

    ui->enemy_card_holder->widget()->setLayout(ui->enemy_card_holder_layout);

    ui->skip_button->setVisible(false);

    ui->main_screen->setCurrentIndex(0);
    ui->server_error_label->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}
