#pragma once

#include <QtWidgets/QMainWindow>
#include <qstyle.h>
#include <QtNetwork/qhostaddress.h>
#include <QtNetwork/qabstractsocket.h>
#include "ui_ClientTCP.h"
#include "DeviceController.h"
#include <QGuiApplication>
#include <QScreen>
#include <qevent.h>
#include <QThread>
#include <qmetaobject.h>
#include "Client.h"
#include <qelapsedtimer.h>
#include <Windows.h>

QT_BEGIN_NAMESPACE
namespace Ui { class ClientTCP; }
QT_END_NAMESPACE

class ClientTCP : public QMainWindow
{
    Q_OBJECT

public:
    ClientTCP(QWidget* parent = nullptr);
    ~ClientTCP();
    Client mouse;
    DeviceController _controller;
    QByteArray imageData;
signals:
    void video_streaming(QByteArray data);
    void sendimage();

public slots:
    void kk(int a);
    void receivesignal(int x, int y, int z);
    void on_btnClear_clicked();
    void on_lnIPAddress_textChanged(const QString& arg1);

    void on_btnConnect_clicked();

    void device_connected();
    void device_disconnected();
    void device_stateChanged(QAbstractSocket::SocketState);
    void device_errorOccurred(QAbstractSocket::SocketError);
    void device_dataReady(QByteArray data);
    void on_btnSend_clicked();
    void my_video_streaming(QByteArray data);
public:
    int count = 0;
    //QPixmap currentPixmap;
    QLabel* imageLabel;
    QVBoxLayout* layout;
    Ui::ClientTCPClass* ui;

    QElapsedTimer timer;
    int imageSize = 0;
    int buffer = 0;
    void mousePressEvent(QMouseEvent* event);
    void setDeviceController();
    void mouseReleaseEvent(QMouseEvent* event);
    //void mouseDoubleClickEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
};