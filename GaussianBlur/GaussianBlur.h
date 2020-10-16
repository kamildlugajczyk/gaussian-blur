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

private slots:
    void exit();
    void on_toolButton_open_clicked();
};
