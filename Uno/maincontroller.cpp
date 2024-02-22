#include "maincontroller.h"

MainController::MainController(QObject* parent): QObject( parent ) {
    client=new QTcpSocket;

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

    QStringList response = QString::fromLatin1(buffer).split(QRegExp(":"));
    QString type = response[0];
    QString _clientId = response[1];
    QString message = response.count() > 2 ? response[2] : "";

    //Empfangenen String zum Debuggen ausgeben
    qDebug() << "Type: " << type;
    qDebug() << "Message: " << message;

    if(type == "connected")
    {
        clientId = _clientId.toInt();
    } else if(type == "card")
    {
        qDebug() << message;
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

void MainController::onError(QAbstractSocket::SocketError) {
    QTcpSocket* socket = (QTcpSocket*)sender();
    qDebug() << "Socket Error: " << socket->errorString();
}
