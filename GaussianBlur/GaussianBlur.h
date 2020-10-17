#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_GaussianBlur.h"

extern "C" bool _stdcall initAsm();

class GaussianBlur : public QMainWindow
{
    Q_OBJECT

public:
    GaussianBlur(QWidget *parent = Q_NULLPTR);

private:
    Ui::GaussianBlurClass ui;
    bool isInputPath = false, isOutputPath = false;

private slots:
    void checkPaths();
    void exit();
    void on_toolButton_openInput_clicked();
    void on_toolButton_openOutput_clicked();
};
