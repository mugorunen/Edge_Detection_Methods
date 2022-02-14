#ifndef SOBEL_H
#define SOBEL_H
#include <QImage>
#include <QFileDialog>
#include <QDebug>
#include <iostream>
#include <cmath>

class sobel
{
public:
    sobel();
    void setImage(QImage *image);
public slots:
    void processImage();
    QImage getFilteredImage();

private:
    QImage processingImage;
    uchar **saveData;
    QSize imageSize;
};

#endif // SOBEL_H
