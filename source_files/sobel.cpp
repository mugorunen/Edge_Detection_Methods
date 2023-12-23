#include "../header_files/sobel.h"

sobel::sobel()
{
}

sobel::sobel(QImage *img)
{
    imageSize = img->size();
    grayImage = img->convertToFormat(QImage::Format_Grayscale8);
}

QImage sobel::processImage()
{
    std::vector<std::vector<int>> img2d(imageSize.height(), std::vector<int>(imageSize.width(), 0));
    std::vector<std::vector<int>> img2d_hor(imageSize.height(), std::vector<int>(imageSize.width(), 0));
    std::vector<std::vector<int>> img2d_ver(imageSize.height(), std::vector<int>(imageSize.width(), 0));
    std::vector<std::vector<int>> img2d_mag(imageSize.height(), std::vector<int>(imageSize.width(), 0));
    std::vector<std::vector<uchar>> saveData(imageSize.height(), std::vector<uchar>(imageSize.width(), 0));
    // Read QImage into 2D vector
    for (int y = 0; y < imageSize.height(); ++y)
    {
        const uchar *scanLine = grayImage.scanLine(y);
        for (int x = 0; x < imageSize.width(); ++x)
        {
            img2d[y][x] = scanLine[x];
        }
    }

    // horizontal and vertical
    for (int i = 1; i < imageSize.height() - 1; i++)
    {
        for (int j = 1; j < imageSize.width() - 1; j++)
        {
            img2d_hor[i][j] = img2d[i - 1][j - 1] + 2 * img2d[i - 1][j] + img2d[i - 1][j + 1] -
                              img2d[i + 1][j - 1] - 2 * img2d[i + 1][j] - img2d[i + 1][j + 1];

            img2d_ver[i][j] = img2d[i - 1][j - 1] + 2 * img2d[i][j - 1] + img2d[i + 1][j - 1] -
                              img2d[i - 1][j + 1] - 2 * img2d[i][j + 1] - img2d[i + 1][j + 1];
        }
    }

    // magnitude
    for (int i = 0; i < imageSize.height(); i++)
    {
        for (int j = 0; j < imageSize.width(); j++)
        {
            img2d_mag[i][j] = static_cast<int>(sqrt(pow(img2d_hor[i][j], 2) + pow(img2d_ver[i][j], 2)));
            img2d_mag[i][j] = std::min(255, std::max(0, img2d_mag[i][j])); // Clamp to [0, 255]
        }
    }

    // Invoke constructor of QImage class
    QImage img(imageSize, QImage::Format_Grayscale8);

    for (int y = 0; y < imageSize.height(); ++y)
    {
        for (int x = 0; x < imageSize.width(); ++x)
        {
            saveData[y][x] = static_cast<uchar>(img2d_mag[y][x]);
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
