#include "server.h"

#include <QDateTime>

/*
* Opens Connection on port 8888
*/
Server::Server(QObject* parent): QObject(parent)
{
    client=NULL;
    server=new QTcpServer;

    connect(server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    server->listen(QHostAddress::Any, 8888);
}

/*
* Schließt die Verbindung
*/
Server::~Server()
{
    server->close();
    if(client != NULL)
        client->close();
    server->deleteLater();
}

/*
* Weißt den Spieler seine id zu und packt ihn in eine lobby (Spielfeld)
*/
void Server::acceptConnection()
{
    //Verbindung annehmen
    client = server->nextPendingConnection();
    PlayerModel *p = new PlayerModel(client);

    PlayingFieldModel* pf = assignOrCreateLobby(p);

    connect(client, &QTcpSocket::readyRead, this, [this, p, pf]() {
        startRead(p, pf);
    });
}

/*
* Liest und verabrebeitet Nachrichten von den Spielern
*/
void Server::startRead(PlayerModel* player, PlayingFieldModel* pf){

    // Dieser Slot wird aufgerufen, sobald der Client Daten an den Server sendet
    // Der Server überprüft, ob es sich um einen GET-Request handelt und sendet ein sehr
    // einfaches HTML-Dokument zurück

    QTcpSocket *socket = player->getSocket();
    qDebug() << "Data incoming";
    qDebug() << socket->canReadLine();

    QTextStream os(socket);
    QStringList data = QString::fromLatin1(socket->readAll()).split(QRegExp(":"));
    QString event = data[0];

    qDebug() << data;
    qDebug() << "Event: " << event;
    qDebug() << "Requesting player id: " << player->getId();

    // Event für erste verbinden
    if(event == "connect")
    {
        os << "connected:" << player->getId();

        socket->flush();

        if(pf->hasSpace() == false)
        {
            pf->start();
        }
    }
    // Event fürs Karte ziehen
    else if(event == "draw")
    {
        pf->drawCard(player);
    }
    // Event fürs Spielen einer Karte
    else if(event == "play")
    {
        pf->playCard(data[1], player);
    }
    // Event für überspringen des zuges
    else if (event == "skip") {
        pf->skip(player);
    }
    // Event für den Farbwechsel (Farbwechselkarte)
    else if (event == "color") {
        pf->pickColor(data[1], player);
    }
    // Event für Chat funktion
    else if (event == "message") {
        pf->chatMessage(data[1], player);
    }

    if ( socket->state() == QTcpSocket::UnconnectedState )
    {
        delete socket;
    }
}

/*
* Erstellt oder weißt den Spieler eine Lobby zu
*/
PlayingFieldModel* Server::assignOrCreateLobby(PlayerModel *player)
{

    for(PlayingFieldModel* pf : playingFields)
    {
        if(pf->hasSpace())
        {
            pf->addPlayer(player);

            return pf;
        }
    }

    PlayingFieldModel *playingField = new PlayingFieldModel();
    playingField->addPlayer(player);
    playingFields.push_back(playingField);

    return playingField;
}
