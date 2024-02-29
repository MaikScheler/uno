#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include <stdlib.h>
#include <QTcpSocket>

#include "cardmodel.h"

using namespace std;


class PlayerModel
{
public:
    PlayerModel(QTcpSocket *socket);

    int getId();
    QTcpSocket *getSocket();
    void addCard(CardModel* card);

private:
    int id;
    QTcpSocket *socket;
    vector<CardModel*> cards;
};

#endif // PLAYERMODEL_H
