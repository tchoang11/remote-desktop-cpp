#pragma once

#include <QTimer>
#include <QScreen>
#include <Qpixmap>
#include <QBuffer>
#include <QGuiApplication>

#include <QtWidgets/QMainWindow>
#include <qstyle.h>
#include <QtNetwork/qhostaddress.h>
#include <QtNetwork/qabstractsocket.h>
#include "MyTCPServer.h"
#include "ui_ServerTCP.h"
#include <qelapsedtimer.h>
#include <QCursor>
#include <QMouseEvent>
#include <iostream>
#include <windows.h>
#include <QImage>
#include <WinUser.h>
#include "processImage.h"
#include <qthread.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ServerTCP : public QMainWindow
{
    Q_OBJECT

public:
    ServerTCP(QWidget* parent = nullptr);
    ~ServerTCP();

public slots:

    void on_btnClear_clicked();
    void on_btnStartServer_clicked();

    void newClientConnected();
    //void sendImage();
    void on_btnSendToAll_clicked();

    void clientDisconnected();

    //void clientDataReceived(QString message);

    void EventRecieve(int a, int b, int c);
    //QByteArray captureScreen();

    //void keyPressEvent(QKeyEvent* event);
signals:
    void rawImageDataIsReady(const QByteArray& rawBitmap, int width, int height, int quality);

public:
    int count = 0;
    int DataEvent[3] = { 0 };
    QPoint mousePos;
    //QTimer* mouseUpdateTimer;
    QElapsedTimer _timer;
    QLabel imageLabel;
    Ui::ServerTCPClass* ui;
    MyTCPServer* _server;
    processImage _processImage;
    QThread _processImageThread;
    //QThread _socketThread;
};