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
    void setImage(QImage *img);
public slots:
    QImage processImage();

private:
    QImage grayImage;
    QSize imageSize;
};

#endif // PREWITT_H
