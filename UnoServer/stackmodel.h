#ifndef STACKMODEL_H
#define STACKMODEL_H

#include <vector>

using namespace std;

#include "cardmodel.h"

class StackModel
{
public:
    StackModel();
    CardModel* getCard();

private:
    vector<CardModel*> cards;
};

#endif // STACKMODEL_H
