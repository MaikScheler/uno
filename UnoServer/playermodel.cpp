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

QTcpSocket* PlayerModel::getSocket()
{
    return socket;
}

void PlayerModel::addCard(CardModel *card)
{
    cards.push_back(card);
}
