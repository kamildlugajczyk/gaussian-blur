#pragma once

#include <QtWidgets/QMainWindow>
#include <vector>
#include "ui_GaussianBlur.h"

extern "C" bool _stdcall init();

class GaussianBlur : public QMainWindow
{
    Q_OBJECT

public:
    GaussianBlur(QWidget *parent = Q_NULLPTR);

private:
    Ui::GaussianBlurClass ui;
    QString outputFileName;
    QString inputFileName;
    bool isInputPath = false, isOutputPath = false;
    unsigned char* inputArray;
    unsigned char* outputArray;
    double** kernel;


    void checkPaths();
    void generateKernel(double** &kernel, char size, double sigma, double & sum);


private slots:
    void on_toolButton_openInput_clicked();
    void on_toolButton_openOutput_clicked();
    void on_pushButton_exit_clicked();
    void on_pushButton_start_clicked();
};
