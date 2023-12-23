#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication My_Application(argc, argv);
    MainWindow My_Window;
    My_Window.show();
    return My_Application.exec();
}
