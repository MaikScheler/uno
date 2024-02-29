#include <iostream>

#include "cardmodel.h"

using namespace std;

CardModel::CardModel(QString name)
{
    this->name = name;

    srand(time(NULL));
    this->id = rand();

    cout << this->id;
}

QString CardModel::getName()
{
    return name;
}

int CardModel::getId()
{
    return id;
}
