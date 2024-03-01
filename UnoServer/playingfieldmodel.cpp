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
}

void PlayingFieldModel::playCard(QString cardId, PlayerModel *player)
{
    CardModel *playedCard = player->getCard(cardId);

    if(playedCard == NULL)
    {
        return;
    }

    QChar prevCardColor = card->getName().at(0);
    QChar prevCardNumber = card->getName().at(1);

    QChar newCardColor = playedCard->getName().at(0);
    QChar newCardNumber = playedCard->getName().at(1);

    if(prevCardColor != newCardColor && prevCardNumber != newCardNumber) return;

    this->card = playedCard;

    for(PlayerModel* p : players)
    {
        QTcpSocket* pSocket = p->getSocket();

        QTextStream os(pSocket);
        os << "play:" + QString::number(player->getId()) + ":" + playedCard->getName() + ":" + cardId + "\n";
        pSocket->flush();
    }

    player->removeCard(cardId);
}

void PlayingFieldModel::start()
{
    if(players.size() == 2)
    {
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
}


