#include "DeviceController.h"

DeviceController::DeviceController(QObject* parent)
    : QObject{ parent }
{
    connect(&_socket, &QTcpSocket::connected, this, &DeviceController::connected);
    connect(&_socket, &QTcpSocket::disconnected, this, &DeviceController::disconnected);
    connect(&_socket, &QTcpSocket::errorOccurred, this, &DeviceController::errorOccurred);
    connect(&_socket, &QTcpSocket::stateChanged, this, &DeviceController::socket_stateChanged);
    connect(&_socket, &QTcpSocket::readyRead, this, &DeviceController::socket_readyRead);
}

//DeviceController::~DeviceController()
//{
//    delete _dataBuffer;
//}

void DeviceController::connectToDevice(QString ip, int port)
{
    if (_socket.isOpen()) {
        if (ip == _ip && port == _port) {
            return;
        }
        _socket.close();
    }
    _ip = ip;
    _port = port;
    _socket.connectToHost(_ip, _port);

}

void DeviceController::disconnect()
{
    _socket.close();
}

QAbstractSocket::SocketState DeviceController::state()
{
    return _socket.state();
}

bool DeviceController::isConnected()
{
    return _socket.state() == QAbstractSocket::ConnectedState;
}

void DeviceController::send(QString message)
{
    _socket.write(message.toUtf8());
}

void DeviceController::socket_stateChanged(QAbstractSocket::SocketState state)
{
    if (state == QAbstractSocket::UnconnectedState) {
        _socket.close();
    }
    emit stateChanged(state);
}

void DeviceController::socket_readyRead()
{
    auto data = _socket.readAll();
    emit dataReady(data);
}

void DeviceController::sendEvents(int a, int x, int y) {
    QByteArray data;
    QDataStream _mstream(&data, QIODevice::WriteOnly);
    _mstream << a;
    _mstream << x;
    _mstream << y;
    emit cc(a);
    //MessageBox(NULL, TEXT("Da gui du lieu cua chuot va ban phim di"), TEXT("Title of the Message Box"), MB_OK);

    _socket.write(data);
}

void DeviceController::sendimagesignal() {
    QByteArray data;
    int x = 9;
    QDataStream _mstream(&data, QIODevice::WriteOnly);
    _mstream << x;
    //MessageBox(NULL, TEXT("This is the message"), TEXT("Title of the Message Box"), MB_OK);
    _socket.write(data);

}