#include "ClientTCP.h"


#include <QMetaEnum>


QThread EventThread;

ClientTCP::ClientTCP(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientTCPClass)
{

    ui->setupUi(this);
    imageLabel = new QLabel(this);
        setDeviceController();

}

ClientTCP::~ClientTCP()
{
    delete imageLabel;
    delete ui;
}

void ClientTCP:: receivesignal(int x, int y, int z) {
    QString kk = QString::number(x);
    if(x > 0) ui->lstConsole->addItem("Type: " + kk);
}

void ClientTCP::on_lnIPAddress_textChanged(const QString& arg1)
{
    QString state = "0";
    if (arg1 == "...") {
        state = "";
    }
    else {
        QHostAddress address(arg1);
        if (QAbstractSocket::IPv4Protocol == address.protocol()) {
            state = "1";
        }
    }
    ui->lnIPAddress->setProperty("state", state);
    style()->polish(ui->lnIPAddress);
}


void ClientTCP::on_btnConnect_clicked()
{
    if (_controller.isConnected()) {
        _controller.disconnect();
    }
    else {
        auto ip = ui->lnIPAddress->text();
        auto port = ui->spnPort->value();
        _controller.connectToDevice(ip, port);
    }
}

void ClientTCP::device_connected()
{
    ui->lstConsole->addItem("Connected To Device");
    connect(&mouse, &Client::send_Event, &_controller, &DeviceController::sendEvents);
    connect(&mouse, &Client::send_Event, this, &ClientTCP::receivesignal);
    ui->btnConnect->setText("Disconnect");
    ui->grpSendData->setEnabled(true);
}

void ClientTCP::device_disconnected()
{
    ui->lstConsole->addItem("Disconnected from Device");
    ui->btnConnect->setText("Connect");
    ui->grpSendData->setEnabled(false);
    _controller.disconnect();
    imageLabel->clear();
    imageLabel->hide();
}

void ClientTCP::device_stateChanged(QAbstractSocket::SocketState state)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketState >();
    ui->lstConsole->addItem(metaEnum.valueToKey(state));
}

void ClientTCP::device_errorOccurred(QAbstractSocket::SocketError error)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketError >();
    ui->lstConsole->addItem(metaEnum.valueToKey(error));

}

void ClientTCP::device_dataReady(QByteArray data) {

    QDataStream stream(&data, QIODevice::ReadOnly);

    if (imageSize == 0) {

        timer.start();
        stream >> imageSize;

        imageData.resize(imageSize);
        buffer = data.size() - sizeof(int);
        memcpy(imageData.data(), data.data() + sizeof(int), buffer);
    }
    else {
        int currentSize = buffer;
        buffer += data.size();
        memcpy(imageData.data() + currentSize, data.data(), data.size());
    }

    if (buffer >= imageSize) {

        qint64 elapsed = timer.elapsed();
        QString kkk = QString::number(elapsed);
        ui->lstConsole->addItem("Thoi gian nhan du liue" + kkk + "ms");
        emit video_streaming(imageData);


        imageSize = 0;
        buffer = 0;
        imageData.clear();
    }
}

void ClientTCP::mousePressEvent(QMouseEvent* event) {
    mouse.mousePressEvent(event);
}
void ClientTCP::mouseReleaseEvent(QMouseEvent* event) {
    mouse.mouseReleaseEvent(event);
}
void ClientTCP::keyPressEvent(QKeyEvent* event) {
    mouse.keyPressEvent(event);
}
void ClientTCP::keyReleaseEvent(QKeyEvent* event) {
    mouse.keyReleaseEvent(event);
}


void ClientTCP::my_video_streaming(QByteArray data) {
    emit sendimage();

    timer.start();

    QPixmap pixmap;
    pixmap.loadFromData(data);

    imageLabel->setGeometry(this->rect());
    imageLabel->setPixmap(pixmap);
    imageLabel->setScaledContents(true);
    imageLabel->show();
    imageLabel->update();
    qint64 kekeke = timer.elapsed();
    QString kkk = QString::number(kekeke);
    ui->lstConsole->addItem("Thoi gian hien anh len man hinh :" + kkk + "ms");
    QString count1 = QString::number(count);
    ui->lstConsole->addItem("yeu cau ben kia gui hinh anh lan : " + count1);
    ui->lstConsole->addItem("Da gui tin hieu keu gui hinh anh ");

}
void ClientTCP::setDeviceController()
{
    connect(&_controller, &DeviceController::connected, this, &ClientTCP::device_connected);
    connect(&_controller, &DeviceController::disconnected, this, &ClientTCP::device_disconnected);
    connect(&_controller, &DeviceController::stateChanged, this, &ClientTCP::device_stateChanged);
    connect(&_controller, &DeviceController::errorOccurred, this, &ClientTCP::device_errorOccurred);
    connect(&_controller, &DeviceController::cc, this, &ClientTCP::kk);
    connect(&_controller, &DeviceController::dataReady, this, &ClientTCP::device_dataReady);
    connect(this, &ClientTCP::sendimage, &_controller, &DeviceController::sendimagesignal);
    connect(this, &ClientTCP::video_streaming, this, &ClientTCP::my_video_streaming);
}

void ClientTCP::kk(int a) {
    if (a > 0) {
        QString kkk = QString::number(a);
        ui->lstConsole->addItem("du lieu duoc gui di: " + kkk);
    }
}

void ClientTCP::on_btnSend_clicked()
{
    auto message = ui->lnMessage->text().trimmed();
    _controller.send(message);
}
void ClientTCP::on_btnClear_clicked() {
    ui->lstConsole->clear();

}