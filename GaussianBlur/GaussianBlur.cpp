#include "GaussianBlur.h"
#include "BmpManager.h"
#include <QFileDialog>
#include <QPixmap>
#include <Windows.h>
#include <cmath>

#define M_PI 3.14159265358979323846

typedef void(__cdecl* pGauss)(unsigned char* bmpArray, unsigned char* outputArray, double kernel[15][15],
                              int32_t width, int32_t height, char size, double& sum);


GaussianBlur::GaussianBlur(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

void GaussianBlur::on_pushButton_exit_clicked()
{
    QApplication::exit();
}

void GaussianBlur::checkPaths()
{
    if (isInputPath && isOutputPath)
        ui.pushButton_start->setEnabled(true);
    else
        ui.pushButton_start->setEnabled(false);
}

void GaussianBlur::on_toolButton_openInput_clicked()
{
    inputFileName = QFileDialog::getOpenFileName(this, tr("Open file"), "C://", "BMP Image (*.bmp)");
    
    if (inputFileName != NULL)
    {
        ui.inputFilePath->setText(inputFileName);

        isInputPath = true;

        QPixmap pix(inputFileName);
        int width = ui.inputImage->width();
        int height = ui.inputImage->height();

        ui.inputImage->setPixmap(pix.scaled(width, height, Qt::KeepAspectRatio));

        checkPaths();
    }
}

void GaussianBlur::on_toolButton_openOutput_clicked()
{
    outputFileName = QFileDialog::getSaveFileName(this, tr("Save file"), "C://", "BMP Image (*.bmp)");

    if (outputFileName != NULL)
    {
        ui.outputFilePath->setText(outputFileName);

        isOutputPath = true;

        checkPaths();
    }
}

void GaussianBlur::on_pushButton_start_clicked()
{
    double kernel[15][15];
    double sum = 0.0;
    HMODULE hModule;
    BmpManager bmp(inputFileName.toStdString(), outputFileName.toStdString());

    // TODO Load bitmap
    bmp.loadBitmap(inputArray, outputArray);
    //outputArray[0] = 1;

    // TODO Devide file if necessary

    // TODO Copy bitmap to result file

    // TODO Make gaussian kernel using parameters

    generateKernel(kernel, 5, 0.8, sum);
    //double kernel[5][5]{ {1,4,7,4,1}, {4,16,26,16,4}, {7,26,41,26,7}, {4,16,26,16,4}, {1,4,7,4,1} };
    //sum = 273;
    // TODO Make threads

    /*if (ui.radioButton_assembler->isChecked())
    {
        hModule = LoadLibrary(TEXT("C:\\Users\\kamil\\source\\repos\\GaussianBlur\\x64\\Debug\\AsmLib.dll"));
    }
    else if (ui.radioButton_cpp->isChecked())
    {*/
        hModule = LoadLibrary(TEXT("C:\\Users\\kamil\\source\\repos\\GaussianBlur\\x64\\Debug\\CppLib.dll"));
    //}
    
    if (hModule == NULL)
    {
        // TODO - dialog with error - GetLastError()
    }
    else
    {
        pGauss gauss = (pGauss)GetProcAddress(hModule, "gauss");

        if (gauss == NULL)
        {
            // TODO - dialog with error - GetLastError()
        }
        else
        {
            gauss(inputArray, outputArray, kernel, bmp.getWidth(), bmp.getHeight(), 15, sum);
            FreeLibrary(hModule);
        }
    }
    bmp.saveBitmap(outputArray);
}

void GaussianBlur::generateKernel(double kernel[15][15], char size, double sigma, double & sum)
{
    double r, s = 2.0 * sigma * sigma;
    //double sum = 0.0;

    for (int x = -7; x <= 7; x++) 
    {
        for (int y = -7; y <= 7; y++) 
        {
            kernel[x + 7][y + 7] = (exp(-(x * x + y * y) / s)) / (M_PI * s);
            sum += kernel[x + 7][y + 7];
        }
    }
    for (int i = 0; i < 15; ++i)
        for (int j = 0; j < 15; ++j)
            kernel[i][j] /= sum;
}