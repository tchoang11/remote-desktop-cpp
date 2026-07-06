#pragma once




#include <QObject>
#include <QtNetwork/qtcpserver.h>
#include <QtNetwork/qtcpsocket.h>
#include <Windows.h>


class MyTCPServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTCPServer(int port, QObject* parent = nullptr);
    bool isStarted() const;
    //int _mport;
    void serverDisconnect();
signals:
    void newClientConnected();
    void clientDisconnect();
    void dataReceived();
    void Events(int a, int b, int c);

public slots:
    void sendToAll(QByteArray message);
    void on_client_connecting();
    //void startServer();
    void clientDisconnected();

    void clientDataReady();


public:
    QTcpServer* _server;
    QList<QTcpSocket*> _socketsList;
    bool _isStarted;
};