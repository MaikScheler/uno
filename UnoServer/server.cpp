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
    connect(client, SIGNAL(readyRead()), this, SLOT(startRead()));
}

void Server::startRead(){

    // Dieser Slot wird aufgerufen, sobald der Client Daten an den Server sendet
    // Der Server überprüft, ob es sich um einen GET-Request handelt und sendet ein sehr
    // einfaches HTML-Dokument zurück

    QTcpSocket *socket = (QTcpSocket* ) QObject::sender();

    if ( socket->canReadLine() )
    {
        QStringList tokens = QString( socket->readLine() ).split( QRegExp( "[ \r\n][ \r\n]*" ) );
        if ( tokens[0] == "GET" )
        {
            QTextStream os( socket );
            os.setAutoDetectUnicode( true );
            os << "HTTP/1.0 200 Ok\r\n"
                  "Content-Type: text/html; charset=\"utf-8\"\r\n"
                  "\r\n"
                  "<h1>Hallo!</h1>\n"
               << QDateTime::currentDateTime().toString() << "\n";
            socket->close();

            if ( socket->state() == QTcpSocket::UnconnectedState )
            {
                delete socket;
            }
        }
    }
}

