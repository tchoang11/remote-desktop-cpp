#include "ServerTCP.h"
#include "ui_ServerTCP.h"

ServerTCP::ServerTCP(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerTCPClass)
{
    ui->setupUi(this);
    mousePos = QCursor::pos();
    _server = nullptr;
}

ServerTCP::~ServerTCP()
{
    _processImageThread.quit();
    _processImageThread.wait();
    delete _server;
    _server = nullptr;
    delete ui;
}

void ServerTCP::on_btnClear_clicked() {
    ui->lstConsole->clear();

}

void ServerTCP::on_btnStartServer_clicked()
{
    if (_server == nullptr) {
        auto port = ui->spnServerPort->value();
        _server = new MyTCPServer(port);
        connect(_server, &MyTCPServer::newClientConnected, this, &ServerTCP::newClientConnected);
        connect(_server, &MyTCPServer::newClientConnected, this, &ServerTCP::on_btnSendToAll_clicked);
        connect(_server, &MyTCPServer::dataReceived, this, &ServerTCP::on_btnSendToAll_clicked);
        connect(_server, &MyTCPServer::clientDisconnect, this, &ServerTCP::clientDisconnected);
        connect(_server, &MyTCPServer::Events, this, &ServerTCP::EventRecieve);
        auto state = (_server->isStarted()) ? "1" : "0";
        ui->lblConnectionStatus->setProperty("state", state);
        style()->polish(ui->lblConnectionStatus);
        ui->lstConsole->addItem("Server Start");
    }
    else {
        _server->serverDisconnect();
        ui->lstConsole->addItem("Disconnected");
        _server = nullptr;
    }



}

void ServerTCP::newClientConnected()
{
    ui->lstConsole->addItem("New Client connected");
    ui->btnStartServer->setText("Disconnect");
    _processImage.moveToThread(&_processImageThread);
    connect(&_processImageThread, &QThread::finished, &_processImage, &QObject::deleteLater);
    connect(&_processImage, &processImage::imagedatareadytosent, _server, &MyTCPServer::sendToAll);
    connect(this, &ServerTCP::rawImageDataIsReady, &_processImage, &processImage::createImage);
    _processImageThread.start();
}
void ServerTCP::EventRecieve(int a, int b, int c) {
    QPoint currentPos = QCursor::pos();
    DataEvent[0] = a;
    DataEvent[1] = b;
    DataEvent[2] = c;
    /* ui.movePos->setText("Mouse Move Pos: X: " + QString::number(DataEvent[1]) + "Y: " + QString::number(DataEvent[2]));*/
    int type = DataEvent[0];
    QString kkk = QString::number(type);
    if (type == 0) {
    }
    else {
        if (type == 1) {
            mouse_event(MOUSEEVENTF_LEFTDOWN, currentPos.x(), currentPos.y(), 0, 0);
        }
        else if (type == 2) {
             mouse_event(MOUSEEVENTF_RIGHTDOWN, currentPos.x(), currentPos.y(), 0, 0);
        }
        else if (type == 3) {
             mouse_event(MOUSEEVENTF_LEFTUP, currentPos.x(), currentPos.y(), 0, 0);
        }
        else if (type == 4) {
             mouse_event(MOUSEEVENTF_RIGHTUP, currentPos.x(), currentPos.y(), 0, 0);
        }
        else if (type == 7 || type == 8) {
            keybd_event(DataEvent[1], 0, 0, 0);
            keybd_event(DataEvent[1], 0, KEYEVENTF_KEYUP, 0);
        }
    }
}


//void ClientTCP::device_disconnected()
//{
//    ui->lstConsole->addItem("Disconnected from Device");
//    ui->btnConnect->setText("Connect");
//    ui->grpSendData->setEnabled(false);
//
//}
void ServerTCP::on_btnSendToAll_clicked() {
    _timer.start();

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int quality = 40;

    HDC hScreenDC = GetDC(NULL);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, screenWidth, screenHeight);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);

    BitBlt(hMemoryDC, 0, 0, screenWidth, screenHeight, hScreenDC, 0, 0, SRCCOPY);

    //_timer.start();
    hBitmap = (HBITMAP)SelectObject(hMemoryDC, hOldBitmap);

    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = screenWidth;
    bi.biHeight = -screenHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    std::vector<BYTE> buf(screenWidth * screenHeight * 4);
    GetDIBits(hMemoryDC, hBitmap, 0, screenHeight, &buf[0], (BITMAPINFO*)&bi, DIB_RGB_COLORS);
    //qint64 elapsed = _timer.elapsed();
    //QString elapsedStr = QString::number(elapsed);
    //ui->lstConsole->addItem("Thoi gian chuyen doi anh thanh QByteArray: " + elapsedStr + "ms");

    DeleteObject(hBitmap);
    DeleteDC(hMemoryDC);
    ReleaseDC(NULL, hScreenDC);
    QByteArray byteArray(reinterpret_cast<char*>(buf.data()), buf.size());
    qint64 khangg = _timer.elapsed();
    QString quansat = QString::number(khangg);
    ui->lstConsole->addItem("Thoi gian chup anh la: " + quansat + " ms");
    /*MessageBox(NULL, TEXT("Da chup va chuan bi tha tin hieu"), TEXT("Title of the Message Box"), MB_OK);*/

    emit rawImageDataIsReady(byteArray, screenWidth, screenHeight, quality);
    //QImage image(reinterpret_cast<const uchar*>(byteArray.constData()), screenWidth, screenHeight, QImage::Format_ARGB32);

    //QByteArray imageData;
    //QBuffer buffer(&imageData);
    //buffer.open(QIODevice::WriteOnly);
    //image.save(&buffer, "JPEG", quality);
    //int datasize = imageData.size();
    //QByteArray imagesize;
    //QDataStream _stream1(&imagesize, QIODevice::WriteOnly);
    //_stream1 << datasize;
    //_server->sendToAll(imagesize + imageData);
    //_timer.start();
    //QImage image(reinterpret_cast<const uchar*>(buf.data()), screenWidth, screenHeight, QImage::Format_ARGB32);
    //QByteArray jpegData;
    //QBuffer buffer(&jpegData);
    //buffer.open(QIODevice::WriteOnly);
    //image.save(&buffer, "JPEG",40);
    //qint64 kkk = _timer.elapsed();
    //QString kekeke = QString::number(kkk);
    //ui->lstConsole->addItem("Thoi gian chuyen thanh anh server: " + kekeke + " ms");


    //return byteArray;
}


//void ServerTCP::on_btnSendToAll_clicked()
//{
//    count++;
//    QString count1 = QString::number(count);
//    ui->lstConsole->addItem("Phai gui hinh anh di lan "+count1);
//    //QScreen* screen = QGuiApplication::primaryScreen();
//    //QPixmap screenshot = screen->grabWindow(0);
//    //QByteArray byteArray;
//    //QBuffer buffer(&byteArray);
//
//    //buffer.open(QIODevice::WriteOnly);
//    //if (screenshot.save(&buffer, "PNG")) {
//    //    buffer.close();
//    //    QString pstring = QString::number(byteArray.size());
//    //    ui->lstConsole->addItem(pstring);
//    //    int totalSize = byteArray.size();
//    //    int largePacketSize = 8192; // 65536 bits
//    //    int numOfLargePackets = totalSize / largePacketSize + (totalSize % largePacketSize != 0 ? 1 : 0);
//    //    int smallPacketSize = largePacketSize / 10;
//
//    //    for (int i = 0; i < numOfLargePackets; ++i) {
//    //        int currentPacketSize = qMin(largePacketSize, totalSize - i * largePacketSize);
//    //        QByteArray largePacket = byteArray.mid(i * largePacketSize, currentPacketSize);
//
//    //        // Here we send 10 smaller packets
//    //        for (int j = 0; j < 10; ++j) {
//    //            int currentSmallPacketSize = qMin(smallPacketSize, currentPacketSize - j * smallPacketSize);
//    //            QByteArray smallPacket = largePacket.mid(j * smallPacketSize, currentSmallPacketSize);
//
//    //            // Prepare packet with header [packetIndex, totalPackets, currentPacketSize, totalSize]
//    //            QByteArray header;
//    //            QDataStream headerStream(&header, QIODevice::WriteOnly);//nay phari xuoosng dofng
//    //            headerStream << i * 10 + j << numOfLargePackets * 10 << currentSmallPacketSize << totalSize;
//
//    //            _server->sendToAll(header + smallPacket);
//    //        }
//    //    }
//    //}
//    QElapsedTimer timer;
//    timer.start();
//    QScreen* screen = QGuiApplication::primaryScreen();
//    QPixmap screenshot = screen->grabWindow(0);
//    QByteArray byteArray;
//    QBuffer buffer(&byteArray);
//    buffer.open(QIODevice::WriteOnly);
//    screenshot.save(&buffer, "JPEG", 40);
//    buffer.close();
//    qint64 elapsed = timer.elapsed();
//    QString elapsedStr = QString::number(elapsed);
//    ui->lstConsole->addItem(elapsedStr + "ms");
//    //QElapsedTimer timer1;
//    timer.start();
//    QByteArray sizeArray;
//    QDataStream sizeStream(&sizeArray, QIODevice::WriteOnly);
//    timer.elapsed();
//    elapsed = timer.elapsed();
//    QString elapsedStr1 = QString::number(elapsed);
//    ui->lstConsole->addItem(elapsedStr1 + "ms");
//    //qint64 elapsed = timer.elapsed();
//    int dataSize1 = byteArray.size();
//    sizeStream << dataSize1;
//    QString p1string = QString::number(byteArray.size());
//       ui->lstConsole->addItem(p1string + "bytes");
//       //if (!byteArray.isEmpty()) {
//       //    _server->sendToAll(sizeArray + byteArray);
//       //}
//
//    //QString kkk = QString::number(byteArray.size());
//    //ui->lstConsole->addItem(kkk + "bytes");
//    //QString elapsedStr3 = QString::number(elapsed);
//    //ui->lstConsole->addItem(elapsedStr3 + "ms");
//
//    int dataSize = byteArray.size();
//    const int packetSize = 1400;
//    int offset = 0;
//
//    _server->sendToAll(sizeArray);
//
//    //ui->lstConsole->addItem(elapsedStr + "ms");
//
//    while (offset < dataSize) {
//        QByteArray packet = byteArray.mid(offset, packetSize);
//        _server->sendToAll(packet);
//        offset += packetSize;
//    }
//
//
//
//
//    //QScreen* screen = QGuiApplication::primaryScreen();
//    //QPixmap screenshot = screen->grabWindow(0);
//    //QByteArray byteArray;
//    //QBuffer buffer(&byteArray);
//    //buffer.open(QIODevice::WriteOnly);
//    //screenshot.save(&buffer, "PNG");
//    //buffer.close();
///*    QString sizeString = QString::number(byteArray.size());
//    if (!byteArray.isEmpty()) {
//        _server->sendToAll(sizeString);
//    }
//    */
//    byteArray.clear();
//    //auto message = ui->lnMessage->text().trimmed();
//    //_server->sendToAll(message);
//}


void ServerTCP::clientDisconnected()
{
    ui->lstConsole->addItem("Client Disconnected");
}

//void ServerTCP::clientDataReceived(QString message)
//{
//    ui->lstConsole->addItem(message);
//}