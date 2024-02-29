#include "playermodel.h"

PlayerModel::PlayerModel(QTcpSocket *socket)
{
    id = rand() % 100;
    this->socket = socket;
}

int PlayerModel::getId()
{
    return id;
}

QString PlayerModel::getName()
{
    return name;
}

QTcpSocket* PlayerModel::getSocket()
{
    return socket;
}

void PlayerModel::addCard(CardModel *card)
{
    cards.push_back(card);
}

void PlayerModel::removeCard(QString cardId)
{
    auto it = find_if(this->cards.begin(), this->cards.end(), [&cardId](CardModel *card) {return card->getId() == cardId;});

    if (it != this->cards.end())
    {
        // found element. it is an iterator to the first matching element.
        // if you really need the index, you can also get it:
        auto index = std::distance(this->cards.begin(), it);

        cards.erase(this->cards.begin() + index);
    }
}
