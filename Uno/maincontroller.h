#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QString>
#include <QTcpSocket>
#include <QHostAddress>

class MainController: public QObject
{
Q_OBJECT
public:
    MainController(QObject* parent = 0);
    ~MainController();
    bool start( QString address, quint16 port );

public slots:
    void startTransfer();
    void startRead();
    void onError(QAbstractSocket::SocketError);

private:
    QTcpSocket *client;
};

#endif // MAINCONTROLLER_H
