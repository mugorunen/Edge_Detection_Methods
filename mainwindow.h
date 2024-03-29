#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <utility>
#include "header_files/sobel.h"
#include "header_files/prewitt.h"
#include "header_files/laplacian.h"
#include "header_files/canny.h"
#include "header_files/deriche.h"

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

    void on_actionClose_triggered();

    void on_processButton_clicked();

    void on_alphaSlider_valueChanged(int value);

    void on_alphaSpinBox_valueChanged(double spinBox_value);

private:
    Ui::MainWindow *ui;
    QImage *img;
    sobel sobelFilter;
    prewitt prewittFilter;
    laplacian laplacianFilter;
    canny cannyFilter;
    deriche dericheFilter;
    detectionType detection = NONE;
    int upper_canny=70;
    int lower_canny=35;
    int upper_deriche = 70;
    int lower_deriche = 35;
    double alpha_deriche = 0.5;
    int maxWidth = 600;
    int maxHeight = 400;

};
#endif // MAINWINDOW_H
