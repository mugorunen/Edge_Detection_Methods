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
    sobel(QImage* img);

public slots:
    QImage processImage();

private:
    QImage grayImage;
    QSize imageSize;
    
    
};

#endif // SOBEL_H
