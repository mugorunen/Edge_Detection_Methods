#include "../header_files/processing_methods.h"

void Processing_Methods::setImage(QImage *image)
{
    processingImage = *image;
    imageSize = processingImage.size();
    processingImage.convertTo(QImage::Format_Grayscale8);
}

QImage Processing_Methods::getFilteredImage(QSize Image_size, QImage::Format format)
{
    // Invoke constructor of QImage class
    QImage img(Image_size, format);

    // Assign the pixel values after operation to QImage object
    for (int i = 0; i < img.height(); i++)
    {
        for (int j = 0; j < img.width(); j++)
        {
            img.setPixel(j, i, image_after_filter[i][j]);
        }
    }

    return img;
}