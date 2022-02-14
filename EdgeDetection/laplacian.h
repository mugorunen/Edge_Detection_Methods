#ifndef LAPLACIAN_H
#define LAPLACIAN_H
#include <QImage>
#include <QFileDialog>
#include <QDebug>
#include <iostream>
#include <cmath>

#define PI 3.1415926535898

class laplacian
{
public:
    laplacian();
    void setImage(QImage *image);
public slots:
    void processImage();
    QImage getFilteredImage();

private:
    void calc_filter(double kernel[][5]);
    void creating_kernel(double kernel[][5]);
    QImage processingImage;
    QSize imageSize;
    uchar **saveData;
};

#endif // LAPLACIAN_H
