#include <QImage>

class Processing_Methods
{
public:
    Processing_Methods();

    void setImage(QImage *image);

    QImage getFilteredImage(QSize Image_size, QImage::Format format);

private:
    QSize imageSize;
    QImage processingImage;
    std::vector<std::vector<uchar>> image_after_filter;
};