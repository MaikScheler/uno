#include "cardmodel.h"

using namespace std;

CardModel::CardModel(QString name)
{
    this->name = name;
    this->id = rand();
}

QString CardModel::getName()
{
    return name;
}

int CardModel::getId() const
{
    return id;
}
