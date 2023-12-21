#ifndef CANNY_H
#define CANNY_H
#include <QImage>
#include <QFileDialog>
#include <QDebug>
#include <iostream>
#include <cmath>

#define PI 3.1415926535898


class canny
{
public:
    canny();
    void setImage(QImage *image);
    int upperThreshold=70;
    int lowerThreshold=35;

public slots:
    void processImage();
    QImage getFilteredImage();

private:
    void calc_filter(double kernel[][5]);
    void creating_kernel(double kernel[][5]);
    void findEdge(int rowShift, int colShift, int row, int col, int dir);
    void suppressNonMax(int rowShift, int colShift, int row, int col, int dir);

    QSize imageSize;
    QImage processingImage;
    double **imageData;
    uchar **saveData;

    int **angle;
    double **gradient;


    bool edgeEnd;
};

#endif // CANNY_H
