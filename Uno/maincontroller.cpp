#include "maincontroller.h"

/*
* Erstellt den TCP Socket und verbindet die TCP Events
*/
MainController::MainController(QObject* parent): QObject( parent ) {
    client=new QTcpSocket;

    // verbindet die functionien zu dem TCP Events
    connect(client, SIGNAL( connected()), this, SLOT( startTransfer()));
    connect(client, SIGNAL(readyRead()), this, SLOT(startRead()));
}

/*
* Schließt die verbindung zum Server
*/
MainController::~MainController() {
    client->close();
    client->deleteLater();
}


/*
* Verbindet sich mit dem Server über die Addresse
*/
bool MainController::start( QString address, quint16 port )
{
    QHostAddress addr( address );
    client->connectToHost( addr, port );

    return client->waitForConnected(1500);
}

/*
* Schick initalie Nachricht an den Server
*/
void MainController::startTransfer(){
    QString str("connect");
    QByteArray ba = str.toLocal8Bit();
    const char *c_str = ba.data();

    client->write( c_str, str.length()+1 );
}

/*
* Empfäng die Nachrichten vom Server und verarbeitet diese.
* Leitet die Nachrichten an die richtige funktion weiter
*/
void MainController::startRead(){
    char buffer[1024] = {0};
    QTcpSocket *sender = (QTcpSocket* ) QObject::sender();
    sender->read(buffer, sender->bytesAvailable());
    // Teilt die nachrichten in die einzelnen events auf
    QStringList responses = QString::fromLatin1(buffer).split(QRegExp("\n"));
    reverse(responses.begin(), responses.end());

    // geht alle Events die vom Server geschickt wurden durch
    for(QString _response : responses)
    {
        // Teil die nachricht in ihre einzelene bestandteile auf
        QStringList response = _response.split(QRegExp(":"));

        if(_response.size() == 0 || response.size() < 2)
        {
            continue;
        }

        QString type = response[0];
        QString _clientId = response[1];
        QString message = response.count() > 2 ? response[2] : "";

        //Empfangenen String zum Debuggen ausgeben (zum Testen)
        qDebug() << response;

        // Bekommt die seine Cilent id vom Server
        if(type == "connected")
        {
            clientId = _clientId.toInt();

        }
        // Event für die Karte die auf dem Stapel liegt
        else if(type == "card")
        {
            // Card id on index 3 and card name on index 2 (message)
            emit drawCardSignal(response[3], message);
        }
        // Event fürs Entfernen von Gespielten karten vom Gegener oder der eigenen
        else if(type == "play")
        {
            // Card id on index 3 and card name on index 2 (message)
            if(_clientId.toInt() != clientId)
            {
                emit removeEnemyCardSignal();
            } else {
                emit removePlayedCardSignal(response[3]);
            }

            emit playCardSignal(response[3], message);
        }
        // Event wer am zug ist
        else if (type == "turn") {
            int sendClientId = _clientId.toInt();
            bool turn = sendClientId == clientId;
            emit changeTurn(turn);
        }
        // Event falls man gewonnen hat
        else if (type == "won") {
            int sendClientId = _clientId.toInt();
            bool won = sendClientId == clientId;
            emit displayWonScreen(won);
        }
        // Event zum auswählen der Farbe von der Farbwechselkarte
        else if (type == "color") {
            emit pickColor();
        }
        // Event zum Anzeigen der Aktuellen Farbe von der Farbwechselkarte
        else if (type == "displayColor") {
            emit displayColor(response[1].at(0));
        }
        // Event zum Anzeige wie viel Karten gezogen werden müssen
        else if (type == "toDraw") {
            emit countDraw(response[1]);
        }
        // Event für chat Nachrichten
        else if (type == "message") {
            emit displayMessage((clientId == _clientId.toInt()), response[2]);
        }
    }
}

/*
* Schick Event zum Server zum Karten ziehen
*/
void MainController::drawCard()
{
    QString type = QString("draw");
    vector<QString> params = {QString::number(clientId)};
    sendMessage(type, params);
}

/*
* Schick Event zum Server fürs Spielen einer Karte
*/
void MainController::playCard(int cardId)
{
    QString type = QString("play");
    vector<QString> params = {QString::number(cardId)};
    sendMessage(type, params);
}

/*
* Schick Event zum Server zum Spielzug beenden
*/
void MainController::skipTurn() {
    QString type = QString("skip");
    vector<QString> params = {QString::number(clientId)};
    sendMessage(type, params);
}

/*
* Gibt Error aus falls welche von der TCP Connection entstehen
*/
void MainController::onError(QAbstractSocket::SocketError) {
    QTcpSocket* socket = (QTcpSocket*)sender();
    qDebug() << "Socket Error: " << socket->errorString();
}

/*
* Schickt Event zum Server welche Farbe ausgewählt wurde (Farbwechselkarte)
*/
void MainController::selectColor(QChar color) {
    QString type = QString("color");
    vector<QString> params = {color};
    sendMessage(type, params);
}

/*
* Utitlity funktion zum schickten von Nachrichten zum Server
*/
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

/*
* Schickt Chat Nachricht an den Server
*/
void MainController::sendUserMessage(QString message) {
    QString type = QString("message");
    vector<QString> params = {message};
    sendMessage(type, params);
}
