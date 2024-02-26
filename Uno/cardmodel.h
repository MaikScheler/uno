#ifndef CARDMODEL_H
#define CARDMODEL_H

#include <QObject>

class CardModel
{
public:
    CardModel(QString name, QString assetUrl);
    QString getName();
    QString getAssetUrl();

signals:

private:
    QString name;
    QString assetUrl;


};

#endif // CARDMODEL_H
