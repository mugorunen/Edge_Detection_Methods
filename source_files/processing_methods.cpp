#include "../header_files/processing_methods.h"

Processing_Methods::Processing_Methods()
{
}
void Processing_Methods::setImage(QImage *img)
{
    imageSize = img->size();
    grayImage = img->convertToFormat(QImage::Format_Grayscale8);
}

Myarray Processing_Methods::calc_filter(Myarray &kernel)
{
    double sigma = 1.0;
    double p, q = 2.0 * sigma * sigma;
    double sum = 0.0;
    for (int x = -2; x <= 2; x++)
    {
        for (int y = -2; y <= 2; y++)
        {
            p = sqrt(x * x + y * y);
            kernel[x + 2][y + 2] = (exp(-(p * p) / q)) / (PI * q);
            sum += kernel[x + 2][y + 2];
        }
    }
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            kernel[i][j] /= sum;
        }
    }

    return kernel;
}
