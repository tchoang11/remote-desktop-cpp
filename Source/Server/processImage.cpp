#include "processImage.h"

processImage::processImage(QObject* parent)
    : QObject(parent)
{

}

processImage::~processImage()
{


}



void processImage::createImage(const QByteArray& rawBitmap, int width, int height, int quality) {
    QImage image(reinterpret_cast<const uchar*>(rawBitmap.constData()), width, height, QImage::Format_ARGB32);

    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPEG", quality);
    buffer.close();
    int datasize = imageData.size();
    QByteArray imagesize;
    QDataStream _stream1(&imagesize, QIODevice::WriteOnly);
    _stream1 << datasize;
    //MessageBox(NULL, TEXT("Du lieu san sang de gui di"), TEXT("Title of the Message Box"), MB_OK);

    emit imagedatareadytosent(imagesize + imageData);
}