#include "mainwindow.h"
#include "ui_mainwindow.h"

// TODO: Process edilen image sev et
// TODO: Base class olustur

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sobelButton->setEnabled(false);
    ui->prewittButton->setEnabled(false);
    ui->laplacianButton->setEnabled(false);
    ui->cannyButton->setEnabled(false);
    ui->dericheButton->setEnabled(false);
    ui->alphaLabel->hide();
    ui->alphaSlider->hide();
    ui->alphaSpinBox->hide();

    ui->lowerLabel->hide();
    ui->lowerSlider->hide();
    ui->lowerSpinBox->hide();

    ui->upperLabel->hide();
    ui->upperSlider->hide();
    ui->upperSpinBox->hide();
    showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_sobelButton_clicked()
{
    ui->alphaLabel->hide();
    ui->alphaSlider->hide();
    ui->alphaSpinBox->hide();

    ui->lowerLabel->hide();
    ui->lowerSlider->hide();
    ui->lowerSpinBox->hide();

    ui->upperLabel->hide();
    ui->upperSlider->hide();
    ui->upperSpinBox->hide();

    sobel sobelFilter(img);
    QPixmap pixmap_2;
    pixmap_2.convertFromImage(sobelFilter.processImage());
    QPixmap scaledPixmap = pixmap_2.scaled(maxWidth, maxHeight, Qt::KeepAspectRatio);
    ui->processedImageLabel->setPixmap(pixmap_2);
    ui->processedImageLabel->setFixedSize(scaledPixmap.size());
    detection = SOBEL;
}


void MainWindow::on_prewittButton_clicked()
{
    ui->alphaLabel->hide();
    ui->alphaSlider->hide();
    ui->alphaSpinBox->hide();

    ui->lowerLabel->hide();
    ui->lowerSlider->hide();
    ui->lowerSpinBox->hide();

    ui->upperLabel->hide();
    ui->upperSlider->hide();
    ui->upperSpinBox->hide();

    prewitt prewittFilter(img);
    QPixmap pixmap_2;
    pixmap_2.convertFromImage(prewittFilter.processImage());
    QPixmap scaledPixmap = pixmap_2.scaled(maxWidth, maxHeight, Qt::KeepAspectRatio);
    ui->processedImageLabel->setPixmap(pixmap_2);
    ui->processedImageLabel->setFixedSize(scaledPixmap.size());
    detection = PREWITT;
}


void MainWindow::on_laplacianButton_clicked()
{
    ui->alphaLabel->hide();
    ui->alphaSlider->hide();
    ui->alphaSpinBox->hide();

    ui->lowerLabel->hide();
    ui->lowerSlider->hide();
    ui->lowerSpinBox->hide();

    ui->upperLabel->hide();
    ui->upperSlider->hide();
    ui->upperSpinBox->hide();

    laplacian laplacianFilter(img);
    QPixmap pixmap_2;
    pixmap_2.convertFromImage(laplacianFilter.processImage());
    QPixmap scaledPixmap = pixmap_2.scaled(maxWidth, maxHeight, Qt::KeepAspectRatio);
    ui->processedImageLabel->setPixmap(pixmap_2);
    ui->processedImageLabel->setFixedSize(scaledPixmap.size());
    detection = LAPLACIAN;
}


void MainWindow::on_cannyButton_clicked()
{
    ui->alphaLabel->hide();
    ui->alphaSlider->hide();
    ui->alphaSpinBox->hide();

    ui->lowerLabel->show();
    ui->lowerSlider->show();
    ui->lowerSpinBox->show();

    ui->upperLabel->show();
    ui->upperSlider->show();
    ui->upperSpinBox->show();


    canny cannyFilter(img);
    cannyFilter.upperThreshold = upper_canny;
    cannyFilter.lowerThreshold = lower_canny;    
    QPixmap pixmap_2;
    pixmap_2.convertFromImage(cannyFilter.processImage());
    QPixmap scaledPixmap = pixmap_2.scaled(maxWidth, maxHeight, Qt::KeepAspectRatio);
    ui->processedImageLabel->setPixmap(pixmap_2);
    ui->processedImageLabel->setFixedSize(scaledPixmap.size());
    detection = CANNY;
}


void MainWindow::on_dericheButton_clicked()
{
    ui->alphaLabel->show();
    ui->alphaSlider->show();
    ui->alphaSpinBox->show();

    ui->lowerLabel->show();
    ui->lowerSlider->show();
    ui->lowerSpinBox->show();

    ui->upperLabel->show();
    ui->upperSlider->show();
    ui->upperSpinBox->show();

    deriche dericheFilter(img);
    dericheFilter.upperThreshold = upper_deriche;
    dericheFilter.lowerThreshold = lower_deriche;
    dericheFilter.alpha = alpha_deriche;
    QPixmap pixmap_2;
    pixmap_2.convertFromImage(dericheFilter.processImage());
    QPixmap scaledPixmap = pixmap_2.scaled(maxWidth, maxHeight, Qt::KeepAspectRatio);
    ui->processedImageLabel->setPixmap(pixmap_2);
    ui->processedImageLabel->setFixedSize(scaledPixmap.size());
    detection = DERICHE;
}



void MainWindow::on_saveButton_clicked()
{

    QString dirName = QFileDialog::getExistingDirectory();
    if(dirName.isEmpty() == false)
    {
        if (detection == PREWITT)
            img->save(dirName+"/prewittFilter.jpeg");
        else if(detection == SOBEL)
            img->save(dirName+"/sobelFilter.jpeg");
        else if(detection == LAPLACIAN)
            laplacianFilter.processImage().save(dirName+"/laplacianFilter.jpeg");
        else if(detection == CANNY)
            img->save(dirName+"/cannyFilter.jpeg");
        else if(detection == DERICHE)
            img->save(dirName+"/dericheFilter.jpeg");
    }
}


void MainWindow::on_actionOpen_triggered()
{
    QString filter = QString("Supported Files (*.jpg *.png *.jpeg);;All files (*)");
    QString filename = QFileDialog::getOpenFileName(this, tr("Select File"), QDir::homePath(), filter);

    if(!filename.isEmpty())
    {
        ui->originalImageLabel->clear();
        ui->processedImageLabel->clear();
        img = new QImage(filename);

        // Convert QImage to QPixmap
        QPixmap pixmap;
        pixmap.convertFromImage(*img);

        // Scale QPixmap while maintaining aspect ratio
        QPixmap scaledPixmap = pixmap.scaled(maxWidth, maxHeight, Qt::KeepAspectRatio);

        // Set the scaled QPixmap to the QLabel
        ui->originalImageLabel->setPixmap(scaledPixmap);

        // Set the fixed size of the QLabel to match the scaled image size
        ui->originalImageLabel->setFixedSize(scaledPixmap.size());

        // Enable buttons
        ui->sobelButton->setEnabled(true);
        ui->prewittButton->setEnabled(true);
        ui->laplacianButton->setEnabled(true);
        ui->cannyButton->setEnabled(true);
        ui->dericheButton->setEnabled(true);
    }
}


void MainWindow::on_actionClose_triggered()
{
    ui->sobelButton->setEnabled(false);
    ui->prewittButton->setEnabled(false);
    ui->laplacianButton->setEnabled(false);
    ui->cannyButton->setEnabled(false);
    ui->dericheButton->setEnabled(false);
    ui->originalImageLabel->clear();
    ui->processedImageLabel->clear();
}


void MainWindow::on_processButton_clicked()
{
    if(detection == CANNY)
    {
        upper_canny = ui->upperSpinBox->value();
        lower_canny = ui->lowerSpinBox->value();
        on_cannyButton_clicked();
    }
    else if(detection == DERICHE)
    {
        alpha_deriche = ui->alphaSpinBox->value();
        upper_deriche = ui->upperSpinBox->value();
        lower_deriche = ui->lowerSpinBox->value();
        on_dericheButton_clicked();
    }
}


void MainWindow::on_alphaSlider_valueChanged(int value)
{
    double value_new = static_cast<double>(value) / 10;
    ui->alphaSpinBox->blockSignals(true);
    ui->alphaSpinBox->setValue(value_new);
    ui->alphaSpinBox->blockSignals(false);
    alpha_deriche = value_new;
}


void MainWindow::on_alphaSpinBox_valueChanged(double spinBox_value)
{
    double update_value = spinBox_value*10;
    ui->alphaSlider->blockSignals(true);
    ui->alphaSlider->setValue(update_value);
    ui->alphaSlider->blockSignals(false);
    alpha_deriche = spinBox_value;
}



