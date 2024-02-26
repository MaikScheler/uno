#include "cardmodel.h"

CardModel::CardModel(QString name, QString assetUrl = "")
{
    this->name = name;

    if (assetUrl == "") {
        this->assetUrl = ":/assets/" + name + ".png";
    } else {
        this->assetUrl = assetUrl;
    }
}
