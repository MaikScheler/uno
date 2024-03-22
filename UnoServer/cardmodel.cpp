#include "cardmodel.h"

using namespace std;

/*
* Gibt der Karte ihren Namen und eine Unique Id
*/
CardModel::CardModel(QString name)
{
    this->name = name;
    this->id = rand() % 10000;
}

/*
* Getter für Name
*/
QString CardModel::getName()
{
    return name;
}

/*
* Getter für Id
*/
int CardModel::getId() const
{
    return id;
}
