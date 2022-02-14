#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sobel.h"
#include "prewitt.h"
#include "laplacian.h"
#include "canny.h"
#include "deriche.h"

enum detectionType
{
    NONE = 0,
    SOBEL,
    PREWITT,
    LAPLACIAN,
    CANNY,
    DERICHE
};



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_sobelButton_clicked();

    void on_prewittButton_clicked();

    void on_laplacianButton_clicked();

    void on_cannyButton_clicked();

    void on_dericheButton_clicked();

    void on_saveButton_clicked();

    void on_actionOpen_triggered();

    void on_processButton_clicked();

    void on_alphaSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QImage *img;
    sobel sobelFilter;
    prewitt prewittFilter;
    laplacian laplacianFilter;
    canny cannyFilter;
    deriche dericheFilter;
    detectionType detection = NONE;
};
#endif // MAINWINDOW_H
