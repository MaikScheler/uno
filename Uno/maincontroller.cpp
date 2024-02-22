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
    QString str("GET \r\n \r\n");
    QByteArray ba = str.toLocal8Bit();
    const char *c_str = ba.data();

    client->write( c_str, str.length()+1 );
}


void MainController::startRead(){

    char buffer[1024] = {0};
    QTcpSocket *sender = (QTcpSocket* ) QObject::sender();
    sender->read(buffer, sender->bytesAvailable());

    //Empfangenen String zum Debuggen ausgeben
    qDebug() << buffer;

    //Empfangenen String auswerten
    //ToDo
}

void MainController::onError(QAbstractSocket::SocketError) {
    QTcpSocket* socket = (QTcpSocket*)sender();
    qDebug() << "Socket Error: " << socket->errorString();
}
