#include "maincontroller.h"

MainController::MainController(QObject* parent): QObject( parent ) {
    client=new QTcpSocket;

    this->playingField = new PlayingFieldModel();

    connect(client, SIGNAL( connected()), this, SLOT( startTransfer()));
    connect(client, SIGNAL(readyRead()), this, SLOT(startRead()));
}

MainController::~MainController() {
    client->close();
    client->deleteLater();
}

bool MainController::start( QString address, quint16 port )
{
    QHostAddress addr( address );
    client->connectToHost( addr, port );

    return client->waitForConnected(1500);
}


void MainController::startTransfer(){
    QString str("connect");
    QByteArray ba = str.toLocal8Bit();
    const char *c_str = ba.data();

    client->write( c_str, str.length()+1 );
}


void MainController::startRead(){

    char buffer[1024] = {0};
    QTcpSocket *sender = (QTcpSocket* ) QObject::sender();
    sender->read(buffer, sender->bytesAvailable());
    QStringList responses = QString::fromLatin1(buffer).split(QRegExp("\n"));

    for(QString _response : responses)
    {
        QStringList response = _response.split(QRegExp(":"));

        if(_response.size() == 0 || response.size() < 2)
        {
            continue;
        }

        QString type = response[0];
        QString _clientId = response[1];
        QString message = response.count() > 2 ? response[2] : "";

        //Empfangenen String zum Debuggen ausgeben
        qDebug() << response;
        qDebug() << "Type: " << type;
        qDebug() << "Message: " << message;

        if(type == "connected")
        {
            clientId = _clientId.toInt();
            this->playingField->addPlayer(clientId, true);

        } else if(type == "card")
        {
            CardModel *card = new CardModel(message);
            emit drawCardSignal(card, this->playingField->getPlayer(clientId));
        } else if(type == "play")
        {
            qDebug() << "Card played (controller)!";
            emit playCardSignal(message);
        }
    }



    //Empfangenen String auswerten
    //ToDo
}

void MainController::drawCard()
{
    QString _clientId = QString::number(clientId);
    QString str("draw:" + _clientId);
    QByteArray ba = str.toLocal8Bit();
    const char *c_str = ba.data();

    client->write( c_str, str.length()+1 );
}

void MainController::playCard(int cardId)
{
    QString _cardId = QString::number(cardId);
    QString str("play:" + _cardId);

    QByteArray ba = str.toLocal8Bit();
    const char *c_str = ba.data();

    client->write( c_str, str.length()+1 );
}

void MainController::onError(QAbstractSocket::SocketError) {
    QTcpSocket* socket = (QTcpSocket*)sender();
    qDebug() << "Socket Error: " << socket->errorString();
}
