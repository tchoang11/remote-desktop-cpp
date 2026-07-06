#pragma once

#include <QObject>
#include <QtNetwork/qtcpsocket.h>
#include <QtNetwork/qabstractsocket.h>
#include <qdatastream.h>
#include <thread>
#include <Windows.h>


class DeviceController : public QObject
{
    Q_OBJECT
public:
    explicit DeviceController(QObject* parent = nullptr);
    void connectToDevice(QString ip, int port);
    void disconnect();
    QAbstractSocket::SocketState state();
    bool isConnected();
    void send(QString message);
    QTcpSocket _socket;
    //~DeviceController();

signals:
    void cc(int a);
    void connected();
    void disconnected();
    void stateChanged(QAbstractSocket::SocketState);
    void errorOccurred(QAbstractSocket::SocketError);
    void dataReady(QByteArray);


public slots:
    void socket_stateChanged(QAbstractSocket::SocketState state);
    void socket_readyRead();
    void sendimagesignal();
    void sendEvents(int a, int x, int y);
private:

    QString _ip;
    QByteArray _dataBuffer;
    int _port;
public:
    std::thread readData;
};