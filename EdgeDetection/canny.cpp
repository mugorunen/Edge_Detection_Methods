#include "canny.h"

canny::canny()
{

}

void canny::setImage(QImage *image)
{
    imageSize = image->size();

    processingImage =QImage();
    processingImage = *image;
    imageSize = processingImage.size();
    processingImage.convertTo(QImage::Format_Grayscale8);


    imageData = new double*[imageSize.height()];
    for(int i=0; i< imageSize.height(); i++)
        imageData[i] = new double[imageSize.width()];


    for (int y = 0; y < imageSize.height(); ++y)
    {
        uint8_t *line = reinterpret_cast<uint8_t*>(processingImage.scanLine(y));
        for (int x = 0; x < imageSize.width(); ++x)
        {
            imageData[y][x] = (double)(line[x] );
        }
    }
}

void canny::processImage()
{
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


    int **img2dhororg = new int*[imageSize.height()];
    for(int i=0; i<imageSize.height();i++)
        img2dhororg[i] = new int[imageSize.width()];

    int **img2dverorg = new int*[imageSize.height()];
    for(int i=0; i<imageSize.height();i++)
        img2dverorg[i] = new int[imageSize.width()];

    //horizontal - Gy
    for (int i=1; i<imageSize.height()-1; i++)
    {
        for (int j=1; j<imageSize.width()-1; j++)
        {
            img2dhororg[i][j]=imageData[i-1][j-1] + 2*imageData[i-1][j]+imageData[i-1][j+1]-imageData[i+1][j-1]-2*imageData[i+1][j]-imageData[i+1][j+1];
        }
    }
    //vertical - Gx
    for (int i=1; i<imageSize.height()-1; i++)
    {
        for (int j=1; j<imageSize.width()-1; j++)
        {
            img2dverorg[i][j]=imageData[i-1][j-1]+2*imageData[i][j-1]+imageData[i+1][j-1]-imageData[i-1][j+1]-2*imageData[i][j+1]-imageData[i+1][j+1];
        }
    }

    gradient = new double*[imageSize.height()];
    for(int i=0; i<imageSize.height();i++)
        gradient[i] = new double[imageSize.width()];


    angle = new int*[imageSize.height()];
    for(int i=0; i<imageSize.height();i++)
        angle[i] = new int[imageSize.width()];

    double thisAngle, newAngle=0;
    //magnitude
    for (int i=0; i<imageSize.height(); i++)
    {
        for (int j=0; j<imageSize.width(); j++)
        {
            gradient[i][j] = sqrt(pow(img2dhororg[i][j], 2)+pow(img2dverorg[i][j], 2));
            gradient[i][j] = gradient[i][j] > 255 ? 255:gradient[i][j];
            gradient[i][j] = gradient[i][j] < 0 ? 0 : gradient[i][j];
            thisAngle = (atan2(img2dverorg[i][j],img2dhororg[i][j])/PI)*180;
            /* Convert actual edge direction to approximate value */
            if ( ( (thisAngle < 22.5) && (thisAngle > -22.5) ) || (thisAngle > 157.5) || (thisAngle < -157.5) )
                newAngle = 0;
            if ( ( (thisAngle > 22.5) && (thisAngle < 67.5) ) || ( (thisAngle < -112.5) && (thisAngle > -157.5) ) )
                newAngle = 45;
            if ( ( (thisAngle > 67.5) && (thisAngle < 112.5) ) || ( (thisAngle < -67.5) && (thisAngle > -112.5) ) )
                newAngle = 90;
            if ( ( (thisAngle > 112.5) && (thisAngle < 157.5) ) || ( (thisAngle < -22.5) && (thisAngle > -67.5) ) )
                newAngle = 135;

            angle[i][j] = newAngle;
        }
    }
    for(int i = 0; i < imageSize.height(); ++i)
    {
        delete[] img2dverorg[i];
        delete[] img2dhororg[i];
    }
    delete[] img2dverorg;
    delete[] img2dhororg;


    /* Trace along all the edges in the image */
    for (int row = 1; row < imageSize.height() - 1; row++) {
        for (int col = 1; col < imageSize.width() - 1; col++) {
            edgeEnd = false;
            if (gradient[row][col] > upperThreshold) {
                /* Switch based on current pixel's edge direction */
                switch (angle[row][col]){
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
                    default :
                        imageData[row][col]= 0;
                        break;
                    }
                }
            else {
                imageData[row][col]= 0;
            }
        }
    }

    /* Non-maximum Suppression */
    for (int row = 1; row < imageSize.height() - 1; row++) {
        for (int col = 1; col < imageSize.width() - 1; col++) {
            if (imageData[row][col] == 255) {		// Check to see if current pixel is an edge
                /* Switch based on current pixel's edge direction */
                switch (angle[row][col]) {
                    case 0:
                        suppressNonMax( 1, 0, row, col, 0);
                        break;
                    case 45:
                        suppressNonMax( 1, -1, row, col, 45);
                        break;
                    case 90:
                        suppressNonMax( 0, 1, row, col, 90);
                        break;
                    case 135:
                        suppressNonMax( 1, 1, row, col, 135);
                        break;
                    default :
                        break;
                }
            }
        }
    }

    saveData = new uchar*[imageSize.height()];
    for (int i = 0; i < imageSize.height(); i++)
        saveData[i] = new uchar[imageSize.width()];

    for (int y = 0; y < imageSize.height(); ++y)
    {
        for (int x = 0; x < imageSize.width(); ++x)
        {
            saveData[y][x] = (uchar)(imageData[y][x]);
        }
    }

    for(int i = 0; i < imageSize.height(); ++i)
    {
        delete[] gradient[i];
        delete[] imageData[i];
        delete[] angle[i];
    }
    delete[] gradient;
    delete[] imageData;
    delete []angle;

}


QImage canny::getFilteredImage()
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


void canny::calc_filter(double kernel[][5]) {
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

void canny::findEdge(int rowShift, int colShift, int row, int col, int dir)
{
    int W = imageSize.width();
    int H = imageSize.height();
    int newRow=0;
    int newCol=0;
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
        edgeEnd = true;		// If the next pixel would be off image, don't do the while loop
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
    while ( (angle[newRow][newCol]==dir) && !edgeEnd && (gradient[newRow][newCol] > lowerThreshold) )
    {
        /* Set the new pixel as white to show it is an edge */
        imageData[newRow][newCol] = 255;
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
        } else
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
        } else
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
    float nonMax[W][3];		// Temporarily stores gradients and positions of pixels in parallel edges
    int pixelCount = 0;		// Stores the number of pixels in parallel edges
    int count;
    int max[3];			// Maximum point in a wide edge

    if (colShift < 0) {
        if (col > 0)
            newCol = col + colShift;
        else
            edgeEnd = true;
    } else if (col < W - 1) {
        newCol = col + colShift;
    } else
        edgeEnd = true;		// If the next pixel would be off image, don't do the while loop
    if (rowShift < 0) {
        if (row > 0)
            newRow = row + rowShift;
        else
            edgeEnd = true;
    } else if (row < H - 1) {
        newRow = row + rowShift;
    } else
        edgeEnd = true;
    /* Find non-maximum parallel edges tracing up */
    while ((angle[newRow][newCol] == dir) && !edgeEnd && (imageData[newRow][newCol] == 255)) {
        if (colShift < 0) {
            if (newCol > 0)
                newCol = newCol + colShift;
            else
                edgeEnd = true;
        } else if (newCol < W - 1) {
            newCol = newCol + colShift;
        } else
            edgeEnd = true;
        if (rowShift < 0) {
            if (newRow > 0)
                newRow = newRow + rowShift;
            else
                edgeEnd = true;
        } else if (newRow < H - 1) {
            newRow = newRow + rowShift;
        } else
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
    if (colShift < 0) {
        if (col > 0)
            newCol = col + colShift;
        else
            edgeEnd = true;
    } else if (col < W - 1) {
        newCol = col + colShift;
    } else
        edgeEnd = true;
    if (rowShift < 0) {
        if (row > 0)
            newRow = row + rowShift;
        else
            edgeEnd = true;
    } else if (row < H - 1) {
        newRow = row + rowShift;
    } else
        edgeEnd = true;
    while ((angle[newRow][newCol] == dir) && !edgeEnd && (imageData[newRow][newCol]== 255)) {
        if (colShift < 0) {
            if (newCol > 0)
                newCol = newCol + colShift;
            else
                edgeEnd = true;
        } else if (newCol < W - 1) {
            newCol = newCol + colShift;
        } else
            edgeEnd = true;
        if (rowShift < 0) {
            if (newRow > 0)
                newRow = newRow + rowShift;
            else
                edgeEnd = true;
        } else if (newRow < H - 1) {
            newRow = newRow + rowShift;
        } else
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
    for (count = 0; count < pixelCount; count++) {
        if (nonMax[count][2] > max[2]) {
            max[0] = nonMax[count][0];
            max[1] = nonMax[count][1];
            max[2] = nonMax[count][2];
        }
    }
    for (count = 0; count < pixelCount; count++) {
        imageData[(int)nonMax[count][0]][(int)nonMax[count][1]] = 0;
    }


}
