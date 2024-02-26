#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include "cardmodel.h";
#include <QObject>

using namespace std;

class PlayerModel
{
public:
    PlayerModel();
    int getAmount();
    vector<CardModel> getCards();
    void addCard();

signals:

private:
    vector<CardModel> cards;

};

#endif // PLAYERMODEL_H
