#include "cardmodel.h"

CardModel::CardModel(QString name, QString assetUrl)
{
    this->name = name;

    if (assetUrl == "") {
        this->assetUrl = ":/assets/" + name + ".png";
    } else {
        this->assetUrl = assetUrl;
    }
}

QString CardModel::getAssetUrl() {
    return this->assetUrl;
}

QString CardModel::getName() {
    return this->name;
}
