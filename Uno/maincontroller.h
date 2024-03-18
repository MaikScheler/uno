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
    void skipTurn();

public slots:
    void startTransfer();
    void startRead();
    void onError(QAbstractSocket::SocketError);

signals:
    void drawCardSignal(QString cardId, QString cardName);
    void playCardSignal(QString cardId, QString cardName);
    void removeEnemyCardSignal();
    void removePlayedCardSignal(QString cardId);
    void changeTurn(bool isTurn);
    void displayWonScreen(bool won);

private:
    QTcpSocket *client;
    int clientId;
    PlayingFieldModel *playingField;
    void sendMessage(QString& type, vector<QString>& params);
};

#endif // MAINCONTROLLER_H
