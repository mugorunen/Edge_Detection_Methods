#include "sobel.h"

sobel::sobel()
{

}

void sobel::setImage(QImage *image)
{
    processingImage =QImage();
    processingImage = *image;
    imageSize = processingImage.size();
    processingImage.convertTo(QImage::Format_Grayscale8);
}

void sobel::processImage()
{
    int **imageData = new int*[imageSize.height()];
    for(int i=0; i<imageSize.height();i++)
        imageData[i] = new int[imageSize.width()];

    for (int y = 0; y < imageSize.height(); ++y)
    {
        uint8_t *line = reinterpret_cast<uint8_t*>(processingImage.scanLine(y));
        for (int x = 0; x < imageSize.width(); ++x)
        {
            imageData[y][x] = (int)(line[x] );
        }
    }

    int **img2dhororg = new int*[imageSize.height()];
    for(int i=0; i<imageSize.height();i++)
        img2dhororg[i] = new int[imageSize.width()];

    int **img2dverorg = new int*[imageSize.height()];
    for(int i=0; i<imageSize.height();i++)
        img2dverorg[i] = new int[imageSize.width()];

    int **img2dmag = new int*[imageSize.height()];
    for(int i=0; i<imageSize.height();i++)
        img2dmag[i] = new int[imageSize.width()];

    //horizontal
    for (int i=1; i<imageSize.height()-1; i++)
    {
        for (int j=1; j<imageSize.width()-1; j++)
        {
            img2dhororg[i][j]=imageData[i-1][j-1] + 2*imageData[i-1][j]+imageData[i-1][j+1]-imageData[i+1][j-1]-2*imageData[i+1][j]-imageData[i+1][j+1];
        }
    }


    //vertical:
    for (int i=1; i<imageSize.height()-1; i++)
    {
        for (int j=1; j<imageSize.width()-1; j++)
        {
            img2dverorg[i][j]=imageData[i-1][j-1]+2*imageData[i][j-1]+imageData[i+1][j-1]-imageData[i-1][j+1]-2*imageData[i][j+1]-imageData[i+1][j+1];
        }
    }

    for(int i = 0; i < imageSize.height(); ++i)
    {
        delete[] imageData[i];
    }
    delete[] imageData;

    //magnitude
    for (int i=0; i<imageSize.height(); i++)
    {
        for (int j=0; j<imageSize.width(); j++)
        {
            img2dmag[i][j] = sqrt(pow(img2dhororg[i][j], 2)+pow(img2dverorg[i][j], 2));
            img2dmag[i][j] = img2dmag[i][j] > 255 ? 255:img2dmag[i][j];
            img2dmag[i][j] = img2dmag[i][j] < 0 ? 0 : img2dmag[i][j];
        }
    }

    for(int i = 0; i < imageSize.height(); ++i)
    {
        delete[] img2dverorg[i];
        delete[] img2dhororg[i];
    }
    delete[] img2dverorg;
    delete[] img2dhororg;


    saveData = new uchar*[imageSize.height()];
    for (int i = 0; i < imageSize.height(); i++)
        saveData[i] = new uchar[imageSize.width()];

    for (int y = 0; y < imageSize.height(); ++y)
    {
        for (int x = 0; x < imageSize.width(); ++x)
        {
            saveData[y][x] = (uchar)(img2dmag[y][x]);
        }
    }

    for(int i = 0; i < imageSize.height(); ++i)
    {
        delete[] img2dmag[i];
    }
    delete[] img2dmag;
}

QImage sobel::getFilteredImage()
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
