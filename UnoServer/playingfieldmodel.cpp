#include "playingfieldmodel.h"

/*
* Legt pro Spieltfeld einen neuen Log und einen Stack an
*/
PlayingFieldModel::PlayingFieldModel()
{
    stack = new StackModel();
    logger = new Logger();
}


/*
* Fügt dem Spielefeld einen Spieler hinzu
*/
void PlayingFieldModel::addPlayer(PlayerModel *player) {
    this->players.push_back(player);
}

/*
* Gibt einen Spieler anhand seiner id zurück
*/
PlayerModel* PlayingFieldModel::getPlayer(int clientId) {
    auto it = find_if(this->players.begin(), this->players.end(), [&clientId](PlayerModel *player) {return player->getId() == clientId;});

    if (it != this->players.end())
    {
        // found element. it is an iterator to the first matching element.
        // if you really need the index, you can also get it:
        auto index = std::distance(this->players.begin(), it);

        return this->players.at(index);
    }
}

/*
* Schaut ob in dem Spielfeld noch platz für einen weiteren Spieler ist
*/
bool PlayingFieldModel::hasSpace()
{
    return players.size() < 2;
}

/*
* Spieler zieht eine Karte
* Überprüft ob spieler Karten ziehen darf und schickt event an spieler
*/
void PlayingFieldModel::drawCard(PlayerModel *player)
{
    if (this->currentPlayer != player || this->drawCardLimit == this->drawenCards) {
        return;
    }

    CardModel* card = stack->getCard();
    player->addCard(card);

    logger->logToFile(player->getId(), "draw", card->getName());

    for(PlayerModel* p : players)
    {
        QTcpSocket* pSocket = p->getSocket();
        QTextStream os(pSocket);

        if(player == p)
        {
            os << "card::" + card->getName() + ":" + QString::number(card->getId()) + "\n";
        } else {
            os << "card::back:0000\n";
        }
    }

    this->drawenCards++;

    if (this->drawCardLimit > 1) {
        this->notifyDrawCards(player, this->drawCardLimit - this->drawenCards);
    }

    if (this->drawCardLimit == this->drawenCards) {
        if (this->drawCardLimit > 1) {
            this->switchPlayer(player);
            this->drawenCards = 0;
        }

        this->drawCardLimit = 1;
        this->notifyDrawCards(player, 0);
    }
}

/*
* Spieler legt Karte
* Überprüft ob spieler die Karte legen darf und schickt event an spieler
*/
void PlayingFieldModel::playCard(QString cardId, PlayerModel *player)
{
    this->drawenCards = 0;
    if (this->currentPlayer != player || this->isPicking) {
        return;
    }

    CardModel *playedCard = player->getCard(cardId);

    logger->logToFile(player->getId(), "play", playedCard->getName());

    if(playedCard == NULL)
    {
        return;
    }

    QChar prevCardColor = card->getName().at(0);
    QChar prevCardNumber = card->getName().at(1);

    QChar newCardColor = playedCard->getName().at(0);
    QChar newCardNumber = playedCard->getName().at(1);


    if (this->currentColor != NULL) {
        if (prevCardColor == "s" && newCardNumber == "+" && this->currentColor != newCardColor) return;
        prevCardColor = this->currentColor;
    }

    if (prevCardColor != newCardColor && prevCardNumber != newCardNumber && newCardColor != 's') return;

    if (drawCardLimit > 1 && this->drawCardLimit != this->drawenCards) return;


    this->card = playedCard;

    for(PlayerModel* p : players)
    {
        QTcpSocket* pSocket = p->getSocket();

        QTextStream os(pSocket);
        os << "play:" + QString::number(player->getId()) + ":" + playedCard->getName() + ":" + cardId + "\n";
        pSocket->flush();
    }

    player->removeCard(cardId);
    this->checkWin(player);
    this->currentColor = QChar();

    if (newCardNumber == '+') {
        this->drawCardLimit = newCardColor == 's' ? 4 : 2;

        for(PlayerModel* p : players)
        {
            if (p != player) {
                this->notifyDrawCards(p, this->drawCardLimit - this->drawenCards);
            }
        }
    }

    if (newCardColor == 's') {
        QTcpSocket* pSocket = player->getSocket();

        QTextStream os(pSocket);
        os << "color:" << "\n";
        pSocket->flush();

        return;
    }

    if (newCardNumber != 'o' && newCardNumber != 'u') {
        this->switchPlayer(player);
    }
}

/*
* Startet das spiel und gibt spieler 7 start karten
*/
void PlayingFieldModel::start()
{
    if(players.size() == 2)
    {
        this->currentPlayer = players.at(1);

        CardModel* card = stack->getFirstCard();
        this->card = card;
        logger->logToFile(-1, "play", card->getName());
        for(PlayerModel* p : players)
        {
            QTcpSocket* pSocket = p->getSocket();
            QTextStream os(pSocket);
            os << "play:" + QString::number(p->getId()) + ":" + card->getName() + ":" + QString::number(card->getId()) << "\n";
            pSocket->flush();

            for(int i = 1; i <= 7; i++)
            {
                CardModel* playerCard = stack->getCard();
                p->addCard(playerCard);
                os << "card::" + playerCard->getName() + ":" + QString::number(playerCard->getId()) + "\n";
                logger->logToFile(p->getId(), "draw", playerCard->getName());
                os << "card::back:0000\n";
                pSocket->flush();
            }
        }
    }

    this->notifyTurn();
}

/*
* Schaut ob spieler seinen Spielzug überspringen darf
* und schickt event
*/
void PlayingFieldModel::skip(PlayerModel* player) {
    if (this->drawCardLimit != 1) {
        return;
    }

    this->drawenCards = 0;
    this->switchPlayer(player);
}

/*
* Schaut ob spieler gewonnen hat und schickt event an alle Spieler wenn Gewonnen
*/
void PlayingFieldModel::checkWin(PlayerModel *player) {
    if (player->getCardCount() != 0) {
        return;
    }

    logger->logToFile(player->getId(), "won");

    for(PlayerModel* p : players)
    {
        QTcpSocket* pSocket = p->getSocket();

        QTextStream os(pSocket);
        os << "won:" + QString::number(player->getId()) +  "\n";
        pSocket->flush();
    }
}

/*
* Tauscht den spielzug und senden Event an Spieler
*/
void PlayingFieldModel::switchPlayer(PlayerModel* currentPlayer) {
    for(PlayerModel* p : players)
    {
        if (p != currentPlayer) {
            this->currentPlayer = p;
        }
    }

    this->notifyTurn();
}

/*
* Spieler pickt Farbe senden Event welche Farbe an alle Spieler
* und wechselt den Spieler
*/
void PlayingFieldModel::pickColor(QString color, PlayerModel *player) {
    this->currentColor = color.at(0);

    this->switchPlayer(player);


    for(PlayerModel* p : players)
    {
        QTcpSocket* pSocket = p->getSocket();

        QTextStream os(pSocket);
        os << "displayColor:" + color +  "\n";
        pSocket->flush();
    }
}

/*
* Schickt Event an Spieler wie viele Karten noch gezogen werden
*/
void PlayingFieldModel::notifyDrawCards(PlayerModel *player, int toDraw) {
    QTcpSocket* pSocket = player->getSocket();

    QTextStream os(pSocket);
    os << "toDraw:" + QString::number(toDraw) << "\n";
    pSocket->flush();
}

/*
* Schickt Event an Spieler wer am Zug ist
*/
void PlayingFieldModel::notifyTurn() {
    for(PlayerModel* p : players)
    {
        QTcpSocket* pSocket = p->getSocket();

        QTextStream os(pSocket);
        os << "turn:" + QString::number(currentPlayer->getId()) +  "\n";
        pSocket->flush();
    }
}

