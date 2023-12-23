#include "../header_files/laplacian.h"

laplacian::laplacian()
{
}

laplacian::laplacian(QImage *img)
{
    imageSize = img->size();
    grayImage = img->convertToFormat(QImage::Format_Grayscale8);
}


QImage laplacian::processImage()
{
    std::vector<std::vector<double>> imageData(imageSize.height(), std::vector<double>(imageSize.width(), 0));
    std::vector<std::vector<double>> img2d(imageSize.height(), std::vector<double>(imageSize.width(), 0));
    std::vector<std::vector<uchar>> saveData(imageSize.height(), std::vector<uchar>(imageSize.width(), 0));

    // Read QImage into 2D vector
    for (int y = 0; y < imageSize.height(); ++y)
    {
        uint8_t *line = reinterpret_cast<uint8_t*>(grayImage.scanLine(y));
        for (int x = 0; x < imageSize.width(); ++x)
        {
            imageData[y][x] = (double)line[x];
        }
    }


    // Since laplacian is noisier, it is better to smooth image with a Gaussian filter
    // We define a 5x5 Gaussian kernel
    double kernel[5][5];
    calc_filter(kernel);

    double curr = 0;
    for (int i = 2; i < imageSize.height() - 2; i++)
    {
        for (int j = 2; j < imageSize.width() - 2; j++)
        {
            curr = 0;
            for (int k1 = 0; k1 < 5; k1++)
            {
                for (int k2 = 0; k2 < 5; k2++)
                {
                    curr += kernel[k1][k2] * imageData[i + (k1 - 2)][j + (k2 - 2)];
                }
            }
            imageData[i][j] = curr;
        }
    }

    for (int i = 1; i < imageSize.height() - 1; i++)
    {
        for (int j = 1; j < imageSize.width() - 1; j++)
        {
            // Apply the laplacian operator [[-1, -1, -1], [-1, 8, -1], [-1, -1 -1]]
            img2d[i][j] = -imageData[i - 1][j - 1] - imageData[i - 1][j] - imageData[i - 1][j + 1] - 
                           imageData[i][j - 1] + 8 * imageData[i][j] - imageData[i][j + 1] - 
                           imageData[i + 1][j - 1] - imageData[i + 1][j] - imageData[i + 1][j + 1];
            img2d[i][j] = std::min(std::max(static_cast<int>(img2d[i][j]), 0), 255);
        }
    }

    // Invoke constructor of QImage class
    QImage img(imageSize, QImage::Format_Grayscale8);

    for (int y = 0; y < imageSize.height(); ++y)
    {
        for (int x = 0; x < imageSize.width(); ++x)
        {
            saveData[y][x] = static_cast<uchar>(img2d[y][x]);
        }
    }

    for (int i = 0; i < img.height(); i++)
    {
        const uint8_t *src = saveData[i].data();

        void *dest = img.scanLine(i);

        std::memcpy(dest, src, img.bytesPerLine());
    }

    return img;
}

// Calculate gaussian filter
void laplacian::calc_filter(double kernel[][5])
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
}
