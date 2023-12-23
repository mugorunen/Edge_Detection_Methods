#include "../header_files/canny.h"

canny::canny()
{
}

void canny::setImage(QImage *img)
{
    imageSize = img->size();
    grayImage = img->convertToFormat(QImage::Format_Grayscale8);
    initializeImage();
}

void canny::initializeImage()
{
    ImageData = std::vector<std::vector<int>>(imageSize.height(), std::vector<int>(imageSize.width(), 0));
    gradient = std::vector<std::vector<int>>(imageSize.height(), std::vector<int>(imageSize.width(), 0));
    angle = std::vector<std::vector<int>>(imageSize.height(), std::vector<int>(imageSize.width(), 0));
}

QImage canny::processImage()
{
    std::vector<std::vector<int>> img2d_ver(imageSize.height(), std::vector<int>(imageSize.width(), 0));
    std::vector<std::vector<int>> img2d_hor(imageSize.height(), std::vector<int>(imageSize.width(), 0));
    std::vector<std::vector<uchar>> saveData(imageSize.height(), std::vector<uchar>(imageSize.width(), 0));

    // Read QImage into 2D vector
    for (int y = 0; y < imageSize.height(); ++y)
    {
        const uchar *scanLine = grayImage.scanLine(y);
        for (int x = 0; x < imageSize.width(); ++x)
        {
            ImageData[y][x] = (int)scanLine[x];
        }
    }

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
                    curr += kernel[k1][k2] * ImageData[i + (k1 - 2)][j + (k2 - 2)];
                }
            }
            ImageData[i][j] = curr;
        }
    }

    // horizontal and vertical
    for (int i = 1; i < imageSize.height() - 1; i++)
    {
        for (int j = 1; j < imageSize.width() - 1; j++)
        {
            img2d_hor[i][j] = ImageData[i - 1][j - 1] + 2 * ImageData[i - 1][j] + ImageData[i - 1][j + 1] -
                              ImageData[i + 1][j - 1] - 2 * ImageData[i + 1][j] - ImageData[i + 1][j + 1];

            img2d_ver[i][j] = ImageData[i - 1][j - 1] + 2 * ImageData[i][j - 1] + ImageData[i + 1][j - 1] -
                              ImageData[i - 1][j + 1] - 2 * ImageData[i][j + 1] - ImageData[i + 1][j + 1];
        }
    }

    double thisAngle, newAngle = 0;
    // magnitude
    for (int i = 0; i < imageSize.height(); i++)
    {
        for (int j = 0; j < imageSize.width(); j++)
        {
            gradient[i][j] = static_cast<int>(sqrt(pow(img2d_hor[i][j], 2) + pow(img2d_ver[i][j], 2)));
            gradient[i][j] = std::max(0, std::min(gradient[i][j], 255));
            thisAngle = (atan2(img2d_ver[i][j], img2d_hor[i][j]) / PI) * 180;
            /* Convert actual edge direction to approximate value */
            if (((thisAngle < 22.5) && (thisAngle > -22.5)) || (thisAngle > 157.5) || (thisAngle < -157.5))
                newAngle = 0;
            if (((thisAngle > 22.5) && (thisAngle < 67.5)) || ((thisAngle < -112.5) && (thisAngle > -157.5)))
                newAngle = 45;
            if (((thisAngle > 67.5) && (thisAngle < 112.5)) || ((thisAngle < -67.5) && (thisAngle > -112.5)))
                newAngle = 90;
            if (((thisAngle > 112.5) && (thisAngle < 157.5)) || ((thisAngle < -22.5) && (thisAngle > -67.5)))
                newAngle = 135;

            angle[i][j] = newAngle;
        }
    }

    /* Trace along all the edges in the image */
    for (int row = 1; row < imageSize.height() - 1; row++)
    {
        for (int col = 1; col < imageSize.width() - 1; col++)
        {
            edgeEnd = false;
            if (gradient[row][col] > upperThreshold)
            {
                /* Switch based on current pixel's edge direction */
                switch (angle[row][col])
                {
                case 0:
                    findEdge(0, 1, row, col, 0);
                    break;
                case 45:
                    findEdge(1, 1, row, col, 45);
                    break;
                case 90:
                    findEdge(1, 0, row, col, 90);
                    break;
                case 135:
                    findEdge(1, -1, row, col, 135);
                    break;
                default:
                    ImageData[row][col] = 0;
                    break;
                }
            }
            else
            {
                ImageData[row][col] = 0;
            }
        }
    }

    /* Non-maximum Suppression */
    for (int row = 1; row < imageSize.height() - 1; row++)
    {
        for (int col = 1; col < imageSize.width() - 1; col++)
        {
            if (ImageData[row][col] == 255)
            { // Check to see if current pixel is an edge
                /* Switch based on current pixel's edge direction */
                switch (angle[row][col])
                {
                case 0:
                    suppressNonMax(1, 0, row, col, 0);
                    break;
                case 45:
                    suppressNonMax(1, -1, row, col, 45);
                    break;
                case 90:
                    suppressNonMax(0, 1, row, col, 90);
                    break;
                case 135:
                    suppressNonMax(1, 1, row, col, 135);
                    break;
                default:
                    break;
                }
            }
        }
    }

    // Invoke constructor of QImage class
    QImage img(imageSize, QImage::Format_Grayscale8);

    for (int y = 0; y < imageSize.height(); ++y)
    {
        for (int x = 0; x < imageSize.width(); ++x)
        {
            saveData[y][x] = static_cast<uchar>(ImageData[y][x]);
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

void canny::calc_filter(double kernel[][5])
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

void canny::findEdge(int rowShift, int colShift, int row, int col, int dir)
{
    int W = imageSize.width();
    int H = imageSize.height();
    int newRow = 0;
    int newCol = 0;
    edgeEnd = false;

    /* Find the row and column values for the next possible pixel on the edge */
    if (colShift < 0)
    {
        if (col > 0)
            newCol = col + colShift;
        else
            edgeEnd = true;
    }
    else if (col < W - 1)
    {
        newCol = col + colShift;
    }
    else
        edgeEnd = true; // If the next pixel would be off image, don't do the while loop
    if (rowShift < 0)
    {
        if (row > 0)
            newRow = row + rowShift;
        else
            edgeEnd = true;
    }
    else if (row < H - 1)
    {
        newRow = row + rowShift;
    }
    else
        edgeEnd = true;

    /* Determine edge directions and gradient strengths */
    while ((angle[newRow][newCol] == dir) && !edgeEnd && (gradient[newRow][newCol] > lowerThreshold))
    {
        /* Set the new pixel as white to show it is an edge */
        ImageData[newRow][newCol] = 255;
        if (colShift < 0)
        {
            if (newCol > 0)
                newCol = newCol + colShift;
            else
                edgeEnd = true;
        }
        else if (newCol < W - 1)
        {
            newCol = newCol + colShift;
        }
        else
            edgeEnd = true;

        if (rowShift < 0)
        {
            if (newRow > 0)
                newRow = newRow + rowShift;
            else
                edgeEnd = true;
        }
        else if (newRow < H - 1)
        {
            newRow = newRow + rowShift;
        }
        else
            edgeEnd = true;
    }
}

void canny::suppressNonMax(int rowShift, int colShift, int row, int col, int dir)
{
    int W = imageSize.width();
    int H = imageSize.height();
    int newRow = 0;
    int newCol = 0;
    bool edgeEnd = false;
    float nonMax[W][3]; // Temporarily stores gradients and positions of pixels in parallel edges
    int pixelCount = 0; // Stores the number of pixels in parallel edges
    int count;
    int max[3]; // Maximum point in a wide edge

    if (colShift < 0)
    {
        if (col > 0)
            newCol = col + colShift;
        else
            edgeEnd = true;
    }
    else if (col < W - 1)
    {
        newCol = col + colShift;
    }
    else
        edgeEnd = true; // If the next pixel would be off image, don't do the while loop
    if (rowShift < 0)
    {
        if (row > 0)
            newRow = row + rowShift;
        else
            edgeEnd = true;
    }
    else if (row < H - 1)
    {
        newRow = row + rowShift;
    }
    else
        edgeEnd = true;
    /* Find non-maximum parallel edges tracing up */
    while ((angle[newRow][newCol] == dir) && !edgeEnd && (ImageData[newRow][newCol] == 255))
    {
        if (colShift < 0)
        {
            if (newCol > 0)
                newCol = newCol + colShift;
            else
                edgeEnd = true;
        }
        else if (newCol < W - 1)
        {
            newCol = newCol + colShift;
        }
        else
            edgeEnd = true;
        if (rowShift < 0)
        {
            if (newRow > 0)
                newRow = newRow + rowShift;
            else
                edgeEnd = true;
        }
        else if (newRow < H - 1)
        {
            newRow = newRow + rowShift;
        }
        else
            edgeEnd = true;
        nonMax[pixelCount][0] = newRow;
        nonMax[pixelCount][1] = newCol;
        nonMax[pixelCount][2] = gradient[newRow][newCol];
        pixelCount++;
    }

    /* Find non-maximum parallel edges tracing down */
    edgeEnd = false;
    colShift *= -1;
    rowShift *= -1;
    if (colShift < 0)
    {
        if (col > 0)
            newCol = col + colShift;
        else
            edgeEnd = true;
    }
    else if (col < W - 1)
    {
        newCol = col + colShift;
    }
    else
        edgeEnd = true;
    if (rowShift < 0)
    {
        if (row > 0)
            newRow = row + rowShift;
        else
            edgeEnd = true;
    }
    else if (row < H - 1)
    {
        newRow = row + rowShift;
    }
    else
        edgeEnd = true;
    while ((angle[newRow][newCol] == dir) && !edgeEnd && (ImageData[newRow][newCol] == 255))
    {
        if (colShift < 0)
        {
            if (newCol > 0)
                newCol = newCol + colShift;
            else
                edgeEnd = true;
        }
        else if (newCol < W - 1)
        {
            newCol = newCol + colShift;
        }
        else
            edgeEnd = true;
        if (rowShift < 0)
        {
            if (newRow > 0)
                newRow = newRow + rowShift;
            else
                edgeEnd = true;
        }
        else if (newRow < H - 1)
        {
            newRow = newRow + rowShift;
        }
        else
            edgeEnd = true;
        nonMax[pixelCount][0] = newRow;
        nonMax[pixelCount][1] = newCol;
        nonMax[pixelCount][2] = gradient[newRow][newCol];
        pixelCount++;
    }

    /* Suppress non-maximum edges */
    max[0] = 0;
    max[1] = 0;
    max[2] = 0;
    for (count = 0; count < pixelCount; count++)
    {
        if (nonMax[count][2] > max[2])
        {
            max[0] = nonMax[count][0];
            max[1] = nonMax[count][1];
            max[2] = nonMax[count][2];
        }
    }
    for (count = 0; count < pixelCount; count++)
    {
        ImageData[(int)nonMax[count][0]][(int)nonMax[count][1]] = 0;
    }
}
