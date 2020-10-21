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
    bool isInputPath = false, isOutputPath = false;

    void checkPaths();
    void generateKernel(double kernel[5][5], int size, double sigma);


private slots:
    void on_toolButton_openInput_clicked();
    void on_toolButton_openOutput_clicked();
    void on_pushButton_exit_clicked();
    void on_pushButton_start_clicked();
};
