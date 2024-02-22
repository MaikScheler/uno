#include "server.h"

#include <QDateTime>

Server::Server(QObject* parent): QObject(parent)
{
    client=NULL;
    server=new QTcpServer;

    connect(server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    server->listen(QHostAddress::Any, 8888);
}

Server::~Server()
{
    server->close();
    if(client != NULL)
        client->close();
    server->deleteLater();
}

void Server::acceptConnection()
{
    //Verbindung annehmen
    client = server->nextPendingConnection();
    Player *p = new Player();
    tuple<Player*,int> playerTuple = { p, 1 };
    clients.push_back(playerTuple);
    int clientId = p->getId();
    // connect(client, SIGNAL(readyRead()), this, SLOT(startRead(5)));
    connect(client, &QTcpSocket::readyRead, this, [this, clientId]() {
        startRead(clientId);
    });
}

void Server::startRead(int clientId){

    // Dieser Slot wird aufgerufen, sobald der Client Daten an den Server sendet
    // Der Server überprüft, ob es sich um einen GET-Request handelt und sendet ein sehr
    // einfaches HTML-Dokument zurück

    QTcpSocket *socket = (QTcpSocket* ) QObject::sender();

    qDebug() << "Data incoming";
    qDebug() << socket->canReadLine();

    QTextStream os(socket);
    QStringList data = QString::fromLatin1(socket->readAll()).split(QRegExp(":"));

    qDebug() << "Data 0: " << data[0];

    if(data[0] == "connect")
    {
        os << "connected:" << clientId;
    } else if(data[0] == "draw")
    {
        os << "card::b+";
    }

    if ( socket->state() == QTcpSocket::UnconnectedState )
    {
        delete socket;
    }
}

