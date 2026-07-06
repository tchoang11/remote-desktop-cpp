#include "MyTCPServer.h"

MyTCPServer::MyTCPServer(int port, QObject* parent)
    : QObject{ parent }
{
    _server = new QTcpServer(this);
    connect(_server, &QTcpServer::newConnection, this, &MyTCPServer::on_client_connecting);
    _isStarted = _server->listen(QHostAddress::Any, port);
    if (!_isStarted) {
        qDebug() << "Server could not start";
    }
    else {
        qDebug() << "Server started...";
    }

}

void MyTCPServer::on_client_connecting()
{
    qDebug() << "a client connected to server";
    auto socket = _server->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &MyTCPServer::clientDataReady);
    connect(socket, &QTcpSocket::disconnected, this, &MyTCPServer::clientDisconnected);
    _socketsList.append(socket);
    //socket->write("Welcome to this Server");
    emit newClientConnected();
}

void MyTCPServer::clientDisconnected()
{
    emit clientDisconnect();
}

void MyTCPServer::clientDataReady()
{
    auto socket = qobject_cast<QTcpSocket*>(sender());
    auto data = socket->readAll();
    QDataStream _mstream(&data, QIODevice::ReadOnly);
    int x = 0;
    _mstream >> x;
    if (x == 9) {
        emit dataReceived();
    }
    else {
        //MessageBox(NULL, TEXT("Da nhan tin hieu chuot ban phim "), TEXT("Title of the Message Box"), MB_OK);

        int y = 0;
        int z = 0;
        _mstream >> y;
        _mstream >> z;
        emit Events(x, y, z);
    }

    //foreach(auto s, _socketsList) {
    //    if (s != socket) {
    //        s->write(data.toUtf8());
    //    }
    //}
}

void MyTCPServer::serverDisconnect() {
    foreach(auto s, _socketsList) {
        s->close();
    }
}

bool MyTCPServer::isStarted() const
{
    return _isStarted;
}

void MyTCPServer::sendToAll(QByteArray message)
{
    //MessageBox(NULL, TEXT("Du lieu da duoc gui di"), TEXT("Title of the Message Box"), MB_OK);
    foreach(auto socket, _socketsList) {
        socket->write(message);
    }
}