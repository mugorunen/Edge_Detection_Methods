#ifndef PREWITT_H
#define PREWITT_H
#include <QImage>
#include <QFileDialog>
#include <QDebug>
#include <iostream>
#include <cmath>

class prewitt
{
public:
    prewitt();
    void setImage(QImage *image);
public slots:
    void processImage();
    QImage getFilteredImage();

private:
    QImage processingImage;
    QSize imageSize;
    uchar **saveData;
};

#endif // PREWITT_H
