#pragma once

#include <QObject>
#include <QImage>
#include <QBuffer>
#include <QByteArray>
#include <Windows.h>


class processImage : public QObject
{
	Q_OBJECT

public:
	processImage(QObject* parent = nullptr);
	~processImage();
public slots:
	void createImage(const QByteArray& rawBitmap, int width, int height, int quality);

signals:
	void imagedatareadytosent(QByteArray imageData);


};