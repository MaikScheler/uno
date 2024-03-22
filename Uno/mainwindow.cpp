#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
* Mapped alle Signale zu Slots
* Initalisiert den Maincontroller
*/
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
    connect(mainController, &MainController::changeTurn, this, &MainWindow::changeTurn);
    connect(mainController, &MainController::displayWonScreen, this, &MainWindow::wonScreen);
    connect(mainController, &MainController::pickColor, this, &MainWindow::pickColor);
    connect(mainController, &MainController::displayColor, this, &MainWindow::displayColor);
    connect(mainController, &MainController::countDraw, this, &MainWindow::countDraw);

    connect(mainController, &MainController::displayMessage, this, &MainWindow::displayMessage);
    connect(ui->chat_input, &QLineEdit::returnPressed, this, &MainWindow::sendUserMessage);

    connect(ui->start_button, &QPushButton::clicked, this, &MainWindow::onStartButtonClicked);
    connect(ui->card_stack_button, &QPushButton::clicked, this, &MainWindow::onCardStackButtonClicked);
    connect(ui->skip_button, &QPushButton::clicked, this, &MainWindow::skipTurn);

    connect(ui->pick_red, &QPushButton::clicked, this, [this] () -> void { selectColor('r'); });
    connect(ui->pick_green, &QPushButton::clicked, this, [this] () -> void { selectColor('g'); });
    connect(ui->pick_yellow, &QPushButton::clicked, this, [this] () -> void { selectColor('y'); });
    connect(ui->pick_blue, &QPushButton::clicked, this, [this] () -> void { selectColor('b'); });
}

/*
* Override paintEvent
* Setzt das Hintergrundbild
*/
void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), QPixmap(":/assets/background-image.png"));
}

/*
* Versucht verbindung zum Server aufzubauen
* Wenn der Start knopf gedrückt wird
*/
void MainWindow::onStartButtonClicked() {
    ui->start_button->setDisabled(true);
    bool isConntected = mainController->start("127.0.0.1", 8888);

    qDebug() << isConntected;

    if (isConntected) {
        ui->primary_card_holder->setVisible(true);
        ui->main_screen->setCurrentIndex(1);
    } else {
        ui->server_error_label->show();
        ui->server_error_label->setText("Connection Failed");
    }

    ui->start_button->setDisabled(false);
}

/*
* Legt die Karten im UI an für Spieler und Gegner
*/
void MainWindow::drawCard(QString cardId, QString cardName) {
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

        if (ui->draw_count->text() == "" && this->isTurn) {
            ui->skip_button->setVisible(true);
        }
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


/*
* Löscht eine Karte des gegners
*/
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

/*
* Löscht die Gespielte Karte
*/
void MainWindow::removePlayedCard(QString cardId)
{
    for (int i = 0; i < ui->primary_card_holder_layout->count(); ++i)
    {
        ClickableLabel *widget = (ClickableLabel*)ui->primary_card_holder_layout->itemAt(i)->widget();
        if (widget != NULL && widget->objectName() == cardId)
        {
            ui->primary_card_holder_layout->removeItem(ui->primary_card_holder_layout->itemAt(ui->primary_card_holder_layout->indexOf(widget) + 1));
            delete widget;
        }
        else
        {
            // You may want to recurse, or perform different actions on layouts.
            // See gridLayout->itemAt(i)->layout()
        }
    }

    ui->draw_count->setText("");
}

/*
* Ändert die Karte auf den Ablagestapel zu der Gespielten
*/
void MainWindow::playCard(QString cardId, QString cardName)
{
    ui->played_card->setFixedSize(117, 171);
    ui->played_card->setPixmap(QPixmap::fromImage(QImage(":/assets/" + cardName + ".png")));
    ui->played_card->setScaledContents(true);

    ui->skip_button->setVisible(false);

    ui->current_color->setText("");
}

/*
* Schickt Event play an den Server
* Wenn man auf eine Karte klickt
*/
void MainWindow::onCardClick(int cardId, ClickableLabel *cardLabel) {
    mainController->playCard(cardId);
}

/*
* Schickt Event skip an den Sever
* Zum Überspringen des Zuges
*/
void MainWindow::skipTurn() {
    this->mainController->skipTurn();

    ui->skip_button->setVisible(false);
}

/*
* Ändert das Label wer gerade am Zug ist
*/
void MainWindow::changeTurn(bool isTurn) {
    if (isTurn) {
        ui->turn_label->setText("Du bist am Zug");
        this->isTurn = true;
    } else {
        ui->turn_label->setText("Der Gegner ist am Zug");
        this->isTurn = false;
        ui->skip_button->setVisible(false);
        ui->draw_count->setText("");
    }
}

/*
* Lässt die Auswahl der Farbe erscheinen
* durchs ändern des Index vom QStackedWidget
*/
void MainWindow::pickColor() {
    ui->main_screen->setCurrentIndex(2);
}

/*
* Schickt Event zum Server mit der Ausgewählten Farbe
* ändert zurück zum spiel durch das ändern des Index vom QStackedWidget
*/
void MainWindow::selectColor(QChar color) {
    ui->main_screen->setCurrentIndex(1);

    this->mainController->selectColor(color);
}

/*
* Zeigt die Ausgewählte Farbe an
*/
void MainWindow::displayColor(QChar color) {
    QString colorText = "";
    if (color == 'r') {
        colorText = QString("Rot");
    } else if (color == 'b') {
        colorText = QString("Blau");
    } else if (color == 'g') {
        colorText = QString("Grün");
    } else if (color == 'y') {
        colorText = QString("Gelb");
    }

    ui->current_color->setText("Die nächstes Karte muss die Farbe " + colorText + " haben.");
}

/*
* Zeigt den Gewonnen / Verloren Screen an
* durch ändern des Index vom QStackedWidget
*/
void MainWindow::wonScreen(bool won) {
    ui->primary_card_holder->setVisible(false);

    if (won) {
        ui->main_screen->setCurrentIndex(3);
    } else {
        ui->main_screen->setCurrentIndex(4);
    }
}

/*
* Zeigt an wie viele Karten gezogen werden müssen
*/
void MainWindow::countDraw(QString toDraw) {
    if (toDraw != "0") {
        ui->draw_count->setVisible(true);
        ui->draw_count->setText("Du musst noch " + toDraw + " " + (toDraw != "1" ? "Karten" : "Karte") + " ziehen");
        return;
    }

    ui->draw_count->setVisible(false);
    ui->draw_count->setText("");
}

/*
* Schickt Event an den Server draw
* (zum ziehen von Karten)
*/
void MainWindow::onCardStackButtonClicked()
{
    mainController->drawCard();
}

/*
* Zeigt den Fehler vom Server an
*/
void MainWindow::showServerError(QString error) {
    ui->server_error_label->show();
    ui->server_error_label->setText(error);
}

/*
* Schickt Spieler chat nachricht an server
*/
void MainWindow::sendUserMessage() {
    QString message = this->ui->chat_input->text();

    this->mainController->sendUserMessage(message);

    this->ui->chat_input->clear();
}

/*
* Display Chat Nachrichten
*/
void MainWindow::displayMessage(QString message) {

    QLabel *label = new QLabel;
    label->setText(message);
    label->setWordWrap(true);
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    label->setTextFormat(Qt::AutoText);
    label->setMaximumWidth(ui->chat_text->width());

    this->ui->chat_text_layout->addWidget(label);

    this->ui->chat_text->verticalScrollBar()->setValue(this->ui->chat_text->verticalScrollBar()->maximum());
}


/*
* Initaliert / Ändert alle Komponenten zu dem gewünschten start Wert im UI
*/
void MainWindow::configureUi() {
    this->setFixedSize(1000, 563);
    ui->card_stack_button->setCursor(Qt::PointingHandCursor);
    ui->start_button->setCursor(Qt::PointingHandCursor);

    ui->primary_card_holder->setParent(ui->centralwidget);
    ui->primary_card_holder->setVisible(false);
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


    ui->chat_text->setWidgetResizable(true);
    ui->chat_text->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->chat_text_layout->setStretch(1,0);
    ui->chat_text_layout->setAlignment(Qt::AlignTop);
    ui->chat_text_layout->setContentsMargins(0, 0, 0, 0);
    ui->chat_text_area->setFixedWidth(ui->chat_text->width());
    ui->chat_text->widget()->setLayout(ui->chat_text_layout);


    ui->skip_button->setVisible(false);

    ui->main_screen->setCurrentIndex(0);
    ui->server_error_label->hide();
}


/*
* Löscht das UI
*/
MainWindow::~MainWindow()
{
    delete ui;
}
