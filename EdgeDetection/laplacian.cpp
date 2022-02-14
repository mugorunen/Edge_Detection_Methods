#include "laplacian.h"

laplacian::laplacian()
{

}

void laplacian::setImage(QImage *image)
{
    processingImage =QImage();
    processingImage = *image;
    imageSize = processingImage.size();
    processingImage.convertTo(QImage::Format_Grayscale8);
}

void laplacian::processImage()
{
    double **imageData = new double*[imageSize.height()];
    for(int i=0; i<imageSize.height();i++)
        imageData[i] = new double[imageSize.width()];


    for (int y = 0; y < imageSize.height(); ++y)
    {
        uint8_t *line = reinterpret_cast<uint8_t*>(processingImage.scanLine(y));
        for (int x = 0; x < imageSize.width(); ++x)
        {
            imageData[y][x] = (double)(line[x] );
        }
    }
    double **img2d = new double*[imageSize.height()];
    for(int i=0; i<imageSize.height();i++)
        img2d[i] = new double[imageSize.width()];


    double kernel[5][5];
    calc_filter(kernel);

    double curr=0;
    for (int i=2; i<imageSize.height()-2; i++)
    {
        for (int j=2; j<imageSize.width()-2; j++)
        {
            curr=0;
            for (int k1=0; k1<5; k1++)
            {
                for (int k2=0; k2<5; k2++)
                {
                    curr += kernel[k1][k2]*imageData[i+(k1-2)][j+(k2-2)];
                }
            }
            imageData[i][j] = curr;
        }
    }


    for (int i=1; i<imageSize.height()-1; i++)
    {
        for (int j=1; j<imageSize.width()-1; j++)
        {
            img2d[i][j]=-imageData[i-1][j-1] - imageData[i-1][j] - imageData[i-1][j+1]-imageData[i][j-1]+8*imageData[i][j]-imageData[i][j+1]
                     -imageData[i+1][j-1]-imageData[i+1][j]-imageData[i+1][j+1];
            img2d[i][j] = img2d[i][j] > 255 ? 255 : img2d[i][j];
            img2d[i][j] = img2d[i][j] < 0   ? 0   : img2d[i][j];
        }
    }
    saveData = new uchar*[imageSize.height()];
    for (int i = 0; i < imageSize.height(); i++)
        saveData[i] = new uchar[imageSize.width()];
    for (int y = 0; y < imageSize.height(); ++y)
    {
        for (int x = 0; x < imageSize.width(); ++x)
        {
            saveData[y][x] = (uchar)(img2d[y][x]);
        }
    }

    for(int i = 0; i < imageSize.height(); ++i)
    {
        delete[] img2d[i];
        delete[] imageData[i];
    }
    delete[] img2d;
    delete[] imageData;
}

QImage laplacian::getFilteredImage()
{
    QImage img(imageSize, QImage::Format_Grayscale8);
    for (int y = 0; y < img.height(); y++)
    {
        memcpy(img.scanLine(y),saveData[y],img.bytesPerLine());
    }
    for(int i = 0; i < imageSize.height(); ++i)
    {
        delete[] saveData[i];
    }
    delete[] saveData;
    return img;
}


void laplacian::calc_filter(double kernel[][5]) {
    double sigma = 1.0;
    double p, q = 2.0 * sigma * sigma;
    double sum = 0.0;
    for (int x = -2; x <= 2; x++) {
        for (int y = -2; y <= 2; y++) {
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
}
