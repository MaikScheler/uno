#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QString>
#include <QTcpSocket>
#include <QHostAddress>
#include <vector>

using namespace std;

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
    void selectColor(QChar color);

    void sendUserMessage(QString message);

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
    void pickColor();
    void displayColor(QChar color);
    void countDraw(QString toDraw);

    void displayMessage(QString message);

private:
    QTcpSocket *client;
    int clientId;
    void sendMessage(QString& type, vector<QString>& params);
};

#endif // MAINCONTROLLER_H
