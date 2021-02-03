#pragma once

#include <QtWidgets/QMainWindow>
#include <QtCharts>
#include <vector>
#include "ui_GaussianBlur.h"

extern "C" void _stdcall gauss();

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
    unsigned char* inputArrayWithFrame;
    unsigned char* outputArray;
    uint64_t* inputR = new uint64_t[256]();
    uint64_t* inputG = new uint64_t[256]();
    uint64_t* inputB = new uint64_t[256]();
    uint64_t* outputR = new uint64_t[256]();
    uint64_t* outputG = new uint64_t[256]();
    uint64_t* outputB = new uint64_t[256]();
    QChart* chartInput = nullptr;
    QBarSeries* series = nullptr;
    float* kernel;
    float sigma;
    int size;
    int threads;


    void checkPaths();
    void generateKernel(float* &kernel, char size, float sigma, float & sum);
    void StartCounter();
    double GetCounter();
    void showHistogram(uint64_t*& R, uint64_t*& G, uint64_t*& B, bool input);

private slots:
    void on_toolButton_openInput_clicked();
    void on_toolButton_openOutput_clicked();
    void on_pushButton_exit_clicked();
    void on_pushButton_start_clicked();
};
