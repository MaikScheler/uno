#include "playingfieldmodel.h"

PlayingFieldModel::PlayingFieldModel()
{
    stack = new StackModel();
}

void PlayingFieldModel::addPlayer(PlayerModel *player) {
    this->players.push_back(player);
}

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


bool PlayingFieldModel::hasSpace()
{
    return players.size() < 2;
}

void PlayingFieldModel::drawCard(PlayerModel *player)
{
    if (this->currentPlayer != player || this->drawCardLimit == this->drawenCards) {
        return;
    }

    CardModel* card = stack->getCard();
    player->addCard(card);

    for(PlayerModel* p : players)
    {
        QTcpSocket* pSocket = p->getSocket();
        QTextStream os(pSocket);

        if(player == p)
        {
            os << "card::" + card->getName() + ":" + QString::number(card->getId());
        } else {
            os << "card::back:0000";
        }
    }

    this->drawenCards++;

    if (this->drawCardLimit > 1 && this->drawCardLimit == this->drawenCards) {
        this->drawCardLimit = 1;
        this->drawenCards = 1;
    }
}

void PlayingFieldModel::playCard(QString cardId, PlayerModel *player)
{
    this->drawenCards = 0;
    if (this->currentPlayer != player || this->isPicking) {
        return;
    }

    CardModel *playedCard = player->getCard(cardId);

    if(playedCard == NULL)
    {
        return;
    }

    QChar prevCardColor = card->getName().at(0);
    QChar prevCardNumber = card->getName().at(1);

    QChar newCardColor = playedCard->getName().at(0);
    QChar newCardNumber = playedCard->getName().at(1);

    if (this->currentColor != NULL) {
        prevCardColor = this->currentColor;
    }

    if (prevCardColor != newCardColor && prevCardNumber != newCardNumber && newCardColor != 's') return;

    if (drawCardLimit > 1 && this->drawCardLimit != this->drawenCards && newCardNumber != '+') return;

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

    if (prevCardNumber == '+' && newCardNumber == '+' && this->drawCardLimit != 1) {
        this->drawCardLimit += newCardColor == 's' ? 4 : 2;
    } else if (newCardNumber == '+') {
        this->drawCardLimit = newCardColor == 's' ? 4 : 2;
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

void PlayingFieldModel::start()
{
    if(players.size() == 2)
    {
        this->currentPlayer = players.at(1);

        CardModel* card = stack->getCard();
        this->card = card;
        for(PlayerModel* p : players)
        {
            QTcpSocket* pSocket = p->getSocket();

            QTextStream os(pSocket);
            os << "play:" + QString::number(p->getId()) + ":" + card->getName() + ":" + QString::number(card->getId()) + "\n";
            pSocket->flush();

            for(int i = 1; i <= 7; i++)
            {
                CardModel* playerCard = stack->getCard();
                p->addCard(playerCard);
                os << "card::" + playerCard->getName() + ":" + QString::number(playerCard->getId()) + "\n";
                os << "card::back:0000\n";
                pSocket->flush();
            }
        }
    }

    this->notifyTurn();
}

void PlayingFieldModel::skip(PlayerModel* player) {
    if (this->drawCardLimit != 1) {
        return;
    }

    this->drawenCards = 0;
    this->switchPlayer(player);
}

void PlayingFieldModel::checkWin(PlayerModel *player) {
    if (player->getCardCount() != 0) {
        return;
    }


    for(PlayerModel* p : players)
    {
        QTcpSocket* pSocket = p->getSocket();

        QTextStream os(pSocket);
        os << "won:" + QString::number(player->getId()) +  "\n";
        pSocket->flush();
    }
}

void PlayingFieldModel::switchPlayer(PlayerModel* currentPlayer) {
    for(PlayerModel* p : players)
    {
        if (p != currentPlayer) {
            this->currentPlayer = p;
        }
    }

    this->notifyTurn();
}

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

void PlayingFieldModel::notifyTurn() {
    for(PlayerModel* p : players)
    {
        QTcpSocket* pSocket = p->getSocket();

        QTextStream os(pSocket);
        os << "turn:" + QString::number(currentPlayer->getId()) +  "\n";
        pSocket->flush();
    }
}

