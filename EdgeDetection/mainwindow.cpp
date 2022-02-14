#include "mainwindow.h"
#include "ui_mainwindow.h"

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

    sobelFilter.setImage(img);
    QPixmap pixmap_2;
    sobelFilter.processImage();
    pixmap_2.convertFromImage(sobelFilter.getFilteredImage());
    ui->processedImageLabel->setPixmap(pixmap_2);
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

    prewittFilter.setImage(img);
    QPixmap pixmap_2;
    prewittFilter.processImage();
    pixmap_2.convertFromImage(prewittFilter.getFilteredImage());
    ui->processedImageLabel->setPixmap(pixmap_2);
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

    laplacianFilter.setImage(img);
    QPixmap pixmap_2;
    laplacianFilter.processImage();
    pixmap_2.convertFromImage(laplacianFilter.getFilteredImage());
    ui->processedImageLabel->setPixmap(pixmap_2);
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

    cannyFilter.setImage(img);
    QPixmap pixmap_2;
    cannyFilter.processImage();
    pixmap_2.convertFromImage(cannyFilter.getFilteredImage());
    ui->processedImageLabel->setPixmap(pixmap_2);
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

    dericheFilter.setImage(img);
    QPixmap pixmap_2;
    dericheFilter.processImage();
    pixmap_2.convertFromImage(dericheFilter.getFilteredImage());
    ui->processedImageLabel->setPixmap(pixmap_2);
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
            img->save(dirName+"/laplacianFilter.jpeg");
        else if(detection == CANNY)
            img->save(dirName+"/cannyFilter.jpeg");
        else if(detection == DERICHE)
            img->save(dirName+"/dericheFilter.jpeg");
    }
}


void MainWindow::on_actionOpen_triggered()
{
    QString filter = QString("Supported Files (*.jpg *.png *.jpeg);;All files (*)");
    QString filename = QFileDialog::getOpenFileName(this, tr("Select File(s)"), QDir::homePath(), filter);

    if(filename.isEmpty() == false)
    {
        img = new QImage(filename);

        QPixmap pixmap_1;
        pixmap_1.convertFromImage(*img);
        ui->originalImageLabel->setPixmap(pixmap_1);

        ui->sobelButton->setEnabled(true);
        ui->prewittButton->setEnabled(true);
        ui->laplacianButton->setEnabled(true);
        ui->cannyButton->setEnabled(true);
        ui->dericheButton->setEnabled(true);
    }
}


void MainWindow::on_processButton_clicked()
{
    if(detection == CANNY)
    {
        cannyFilter.upperThreshold = ui->upperSpinBox->value();
        cannyFilter.lowerThreshold = ui->lowerSpinBox->value();
        on_cannyButton_clicked();
    }
    else if(detection == DERICHE)
    {
        dericheFilter.alpha = ui->alphaSpinBox->value();
        dericheFilter.upperThreshold = ui->upperSpinBox->value();
        dericheFilter.lowerThreshold = ui->lowerSpinBox->value();
        on_dericheButton_clicked();
    }
}


void MainWindow::on_alphaSlider_valueChanged(int value)
{
    double value_db = (double) value / 20;
    ui->alphaSpinBox->setValue(value_db);
}

