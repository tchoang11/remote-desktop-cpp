#include "Client.h"

Client::Client(QWidget* parent)
    : QMainWindow(parent)
{
    mouseUpdateTimer = new QTimer(this);
    connect(mouseUpdateTimer, &QTimer::timeout, this, &Client::updateMousePosition);
    mouseUpdateTimer->start(40); 

}

Client::~Client()
{
}
void Client::keyPressEvent(QKeyEvent* event)
{
    int k = 0;
    if ((event->key() >= 0x30 && event->key() <= 0x39) || (event->key() >= 0x41 && event->key() <= 0x5A)) {
        k = event->key();
    }
    else {
        switch (event->key())
        {
        case Qt::Key_Tab:
            k = VK_TAB;
            break;
        case Qt::Key_Escape:
            k = VK_ESCAPE;
            break;
        case Qt::Key_Shift:
            k = VK_SHIFT;
            break;
        case Qt::Key_Space:
            k = VK_SPACE;
            break;
        case Qt::Key_Control:
            k = VK_CONTROL;
            break;
        case Qt::Key_CapsLock:
            k = VK_CAPITAL;
            break;
        case Qt::Key_Return:
            k = VK_RETURN;
            break;
        case Qt::Key_Alt:
            k = VK_MENU;
            break;
        case Qt::Key_Backspace:
            k = VK_BACK;
            break;
        case Qt::Key_Delete:
            k = VK_DELETE;
            break;
        case Qt::Key_Semicolon:
            k = VK_OEM_1;
            break;
        case Qt::Key_Comma:
            k = VK_OEM_COMMA;
            break;
        case Qt::Key_Period:
            k = VK_OEM_PERIOD;
            break;
        case Qt::Key_Slash:
            k = VK_OEM_2;
            break;
        case Qt::Key_Apostrophe:
            k = VK_OEM_7;
            break;
        default:
            break;
        }
    }
    emit send_Event(KEY_DOWN, k, 0);
}
void Client::keyReleaseEvent(QKeyEvent* event) {
    int k = 0;
    if ((event->key() >= 0x30 && event->key() <= 0x39) || (event->key() >= 0x41 && event->key() <= 0x5A)) {
        k = event->key();
    }
    else {
        switch (event->key())
        {
        case Qt::Key_Tab:
            k = VK_TAB;
            break;
        case Qt::Key_Escape:
            k = VK_ESCAPE;
            break;
        case Qt::Key_Shift:
            k = VK_SHIFT;
            break;
        case Qt::Key_Space:
            k = VK_SPACE;
            break;
        case Qt::Key_Control:
            k = VK_CONTROL;
            break;
        case Qt::Key_CapsLock:
            k = VK_CAPITAL;
            break;
        case Qt::Key_Return:
            k = VK_RETURN;
            break;
        case Qt::Key_Alt:
            k = VK_MENU;
            break;
        case Qt::Key_Backspace:
            k = VK_CLEAR;
            break;
        case Qt::Key_Delete:
            k = VK_DELETE;
            break;
        case Qt::Key_Semicolon:
            k = VK_OEM_1;
            break;
        case Qt::Key_Comma:
            k = VK_OEM_COMMA;
            break;
        case Qt::Key_Period:
            k = VK_OEM_PERIOD;
            break;
        case Qt::Key_Slash:
            k = VK_OEM_2;
            break;
        case Qt::Key_Apostrophe:
            k = VK_OEM_7;
            break;
        default:
            break;
        }
    }
    emit send_Event(KEY_UP, k, 0);
}
void Client::updateMousePosition()
{
    QPoint pos = QCursor::pos();
    emit send_Event(0, pos.x(), pos.y());
}
void Client::mousePressEvent(QMouseEvent* event)
{
    QPoint pos = QCursor::pos();
    if (event->button() == Qt::LeftButton) {
        emit send_Event(1, pos.x(), pos.y());
    }
    if (event->button() == Qt::RightButton) {
        emit send_Event(2, pos.x(), pos.y());
    }
}
void Client::mouseReleaseEvent(QMouseEvent* event) {
    QPoint pos = QCursor::pos();
    if (event->button() == Qt::LeftButton) {
        emit send_Event(3, pos.x(), pos.y());
    }
    if (event->button() == Qt::RightButton) {
        emit send_Event(4, pos.x(), pos.y());
    }
}
void Client::mouseDoubleClickEvent(QMouseEvent* event) {
    QPoint pos = QCursor::pos();
    if (event->button() == Qt::LeftButton) {
    }
    if (event->button() == Qt::RightButton) {
    }
}
