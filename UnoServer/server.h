#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include "playermodel.h"
#include "playingfieldmodel.h"

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
    void startRead(PlayerModel* player, PlayingFieldModel* pf);

private:
    QTcpServer *server;
    QTcpSocket* client;
    vector<PlayingFieldModel*> playingFields;

    PlayingFieldModel* assignOrCreateLobby(PlayerModel *player);
};

#endif // SERVER_H
