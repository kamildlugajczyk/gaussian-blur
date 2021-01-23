#pragma once

#include <QtWidgets/QMainWindow>
#include <vector>
#include "ui_GaussianBlur.h"

extern "C" bool _stdcall gauss();
//extern "C" bool _stdcall gauss(unsigned char* inputArray, unsigned char* outputArray, float** kernel, int32_t width, int32_t startHeight, int32_t stopHeight, char size, float sum);

class GaussianBlur : public QMainWindow
{
    Q_OBJECT

public:
    GaussianBlur(QWidget *parent = Q_NULLPTR);

private:
    Ui::GaussianBlurClass ui;
    //QString outputFileName;
    QString outputFileName = "D://Desktop//JA bitmapy//bitmap1.bmp";
    //QString inputFileName;
    QString inputFileName = "D://Desktop//JA bitmapy//test.bmp";
    //bool isInputPath = false, isOutputPath = false;       // WARNING TODO ZMIENIC POTEM
    bool isInputPath = true, isOutputPath = true;
    unsigned char* inputArray;
    unsigned char* inputArrayWithFrame;
    unsigned char* outputArray;
    float** kernel;
    float sigma;
    int size;
    int threads;


    void checkPaths();
    void generateKernel(float** &kernel, char size, float sigma, float & sum);
    void StartCounter();
    double GetCounter();

private slots:
    void on_toolButton_openInput_clicked();
    void on_toolButton_openOutput_clicked();
    void on_pushButton_exit_clicked();
    void on_pushButton_start_clicked();
};
