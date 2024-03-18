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
    QString getName();
    QTcpSocket *getSocket();
    void addCard(CardModel* card);
    void removeCard(QString cardName);
    CardModel* getCard(QString cardId);
    int getCardCount();

private:
    int id;
    QTcpSocket *socket;
    vector<CardModel*> cards;
    QString name;
};

#endif // PLAYERMODEL_H
