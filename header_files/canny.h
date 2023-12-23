#ifndef CANNY_H
#define CANNY_H
#include <QImage>
#include <QFileDialog>
#include <QDebug>
#include <iostream>
#include <cmath>
#include "processing_methods.h"

#define PI 3.1415926535898

class canny
{
public:
    canny();

    int upperThreshold = 70;
    int lowerThreshold = 35;

public slots:
    QImage processImage();
    void setImage(QImage *img);

private:
    void initializeImage();
    void calc_filter(double kernel[][5]);
    void creating_kernel(double kernel[][5]);
    void findEdge(int rowShift, int colShift, int row, int col, int dir);
    void suppressNonMax(int rowShift, int colShift, int row, int col, int dir);

    QSize imageSize;
    QImage grayImage;
    std::vector<std::vector<int>> ImageData;
    std::vector<std::vector<int>> gradient;
    std::vector<std::vector<int>> angle;

    bool edgeEnd;
};

#endif // CANNY_H
