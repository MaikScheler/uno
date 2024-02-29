#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QString>
#include <QTcpSocket>
#include <QHostAddress>
#include "playingfieldmodel.h"
#include "cardmodel.h"
#include "playermodel.h"

class MainController: public QObject
{
Q_OBJECT
public:
    MainController(QObject* parent = 0);
    ~MainController();
    bool start( QString address, quint16 port );
    void drawCard();
    void playCard(int cardId);

public slots:
    void startTransfer();
    void startRead();
    void onError(QAbstractSocket::SocketError);

signals:
    void drawCardSignal(CardModel *card, PlayerModel* player);

private:
    QTcpSocket *client;
    int clientId;
    PlayingFieldModel *playingField;
};

#endif // MAINCONTROLLER_H
