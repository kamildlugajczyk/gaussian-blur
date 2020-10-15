#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_GaussianBlur.h"

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
