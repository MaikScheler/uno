#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include "cardmodel.h";
#include <QObject>

using namespace std;

class PlayerModel
{
public:
    PlayerModel(int clientId, bool primary = false);
    int getAmount();
    vector<CardModel> getCards();
    void addCard();
    int getClientId();
    bool getPrimary();

signals:

private:
    vector<CardModel> cards;
    int clientId;
    bool primary = false;

};

#endif // PLAYERMODEL_H
