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

CardModel* PlayerModel::getCard(QString cardId)
{
    auto isValueMatch = [&cardId](const CardModel* c) {
        return c->getId() == cardId.toInt();
    };

    auto it = find_if(cards.begin(), cards.end(), isValueMatch);
    auto index = std::distance(this->cards.begin(), it);

    return cards.at(index);
}

void PlayerModel::removeCard(QString cardId)
{
    auto it = find_if(this->cards.begin(), this->cards.end(), [&cardId](CardModel *card) {return card->getId() == cardId.toInt();});

    if (it != this->cards.end())
    {
        // found element. it is an iterator to the first matching element.
        // if you really need the index, you can also get it:
        auto index = std::distance(this->cards.begin(), it);

        cards.erase(this->cards.begin() + index);
    }
}
