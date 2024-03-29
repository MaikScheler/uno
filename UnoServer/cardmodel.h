#ifndef CARDMODEL_H
#define CARDMODEL_H

#include <QString>


class CardModel
{
public:
    CardModel(QString name);
    QString getName();
    int getId() const;

private:
    QString name;
    int id;
};

#endif // CARDMODEL_H
