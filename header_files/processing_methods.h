#ifndef PROCESSING_METHODS_H
#define PROCESSING_METHODS_H
#include <QImage>

#define PI 3.1415926535898
using Myarray = std::array<std::array<double, 5>, 5>;

class Processing_Methods
{
public:
    Processing_Methods();

    void setImage(QImage *image);

    Myarray calc_filter(Myarray &kernel);

    QSize imageSize;
    QImage grayImage;

private:
};

#endif // PROCESSING_METHODS_H