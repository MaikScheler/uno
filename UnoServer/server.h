#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include "player.h"

#include <vector>

using namespace std;

class Server: public QObject
{
Q_OBJECT
public:
    Server(QObject * parent = 0);
    ~Server();

public slots:
    void acceptConnection();
    void startRead(int clientId);

private:
    QTcpServer *server;
    QTcpSocket* client;
    vector<tuple<Player*,int>> clients;
};

#endif // SERVER_H
