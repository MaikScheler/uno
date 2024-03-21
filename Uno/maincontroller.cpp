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
    QStringList responses = QString::fromLatin1(buffer).split(QRegExp("\n"));
    reverse(responses.begin(), responses.end());

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

        if(type == "connected")
        {
            clientId = _clientId.toInt();

        } else if(type == "card")
        {
            // Card id on index 3 and card name on index 2 (message)
            emit drawCardSignal(response[3], message);
        } else if(type == "play")
        {
            // Card id on index 3 and card name on index 2 (message)
            if(_clientId.toInt() != clientId)
            {
                emit removeEnemyCardSignal();
            } else {
                emit removePlayedCardSignal(response[3]);
            }

            emit playCardSignal(response[3], message);
        } else if (type == "turn") {
            int sendClientId = _clientId.toInt();
            bool turn = sendClientId == clientId;
            emit changeTurn(turn);
        } else if (type == "won") {
            int sendClientId = _clientId.toInt();
            bool won = sendClientId == clientId;
            emit displayWonScreen(won);
        } else if (type == "color") {
            emit pickColor();
        } else if (type == "displayColor") {
            emit displayColor(response[1].at(0));
        } else if (type == "toDraw") {
            emit countDraw(response[1]);
        }
    }
}

void MainController::drawCard()
{
    QString type = QString("draw");
    vector<QString> params = {QString::number(clientId)};
    sendMessage(type, params);
}

void MainController::playCard(int cardId)
{
    QString type = QString("play");
    vector<QString> params = {QString::number(cardId)};
    sendMessage(type, params);
}

void MainController::skipTurn() {
    QString type = QString("skip");
    vector<QString> params = {QString::number(clientId)};
    sendMessage(type, params);
}

void MainController::onError(QAbstractSocket::SocketError) {
    QTcpSocket* socket = (QTcpSocket*)sender();
    qDebug() << "Socket Error: " << socket->errorString();
}

void MainController::selectColor(QChar color) {
    QString type = QString("color");
    vector<QString> params = {color};
    sendMessage(type, params);
}


void MainController::sendMessage(QString& type, vector<QString>& params) {
  QString data = type;
  for (const QString& param : params) {
    data += ":";
    data += param;
  }

  QByteArray ba = data.toLocal8Bit();
  const char *c_str = ba.data();

  client->write( c_str, data.length()+1 );
}
