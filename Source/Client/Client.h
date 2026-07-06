#pragma once

#include <QtWidgets/QMainWindow>
//#include "ui_CLIENT.h"
#include <QApplication>
#include <QWidget>
#include <QMouseEvent>
//#include "socket.h"
#include <QKeyEvent>
#include <Windows.h>
#include <QTimer>

#define KEY_DOWN 7
#define KEY_UP 8
static int x = 1;

class Client : public QMainWindow
{
    Q_OBJECT

public:
    int countPress = 0;
    int countRelease = 0;
    Client(QWidget* parent = nullptr);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    ~Client();
public slots:
    void updateMousePosition();
signals:
    void send_Event(int a, int k, int y);
private:
    QTimer* mouseUpdateTimer;
};