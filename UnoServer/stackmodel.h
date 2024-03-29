#ifndef STACKMODEL_H
#define STACKMODEL_H

#include <vector>
#include <QRegExp>

using namespace std;

#include "cardmodel.h"

class StackModel
{
public:
    StackModel();
    CardModel* getCard();
    CardModel* getFirstCard();

private:
    vector<CardModel*> cards;
    void createCard(QString cardName);
};

#endif // STACKMODEL_H
