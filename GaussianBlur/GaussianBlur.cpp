#include "GaussianBlur.h"
#include "BmpManager.h"
#include <QFileDialog>
#include <QPixmap>
#include <Windows.h>
#include <cmath>
#include <vector>
#include <thread> 

#define M_PI 3.14159265358979323846

typedef void(__cdecl* pGauss)(unsigned char* bmpArray, unsigned char* outputArray, double** kernel,
                                int32_t width, int32_t startHeight, int32_t stopHeight, char size, double& sum);

double PCFreq = 0.0;
__int64 CounterStart = 0;

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
    int size = 21;
    double sigma = size / 7.0;

    int threads = 1;

    double sum = 0.0, time = 0.0;
    HMODULE hModule;
    BmpManager bmp(inputFileName.toStdString(), outputFileName.toStdString());

    // TODO Load bitmap
    bmp.loadBitmap(inputArray, inputArrayWithFrame, outputArray, size);
    //bmp.mirrorBoundaries(inputArray, size);

    // TODO Devide file if necessary

    // TODO Make gaussian kernel using parameters
    generateKernel(kernel, size, sigma, sum);

    // TODO Make threads
    std::vector<std::thread> threadsVector;

    int rowsForThread = bmp.getHeight() / threads;


    //---------------------------------------------------------------------------------------------------------------------

    if (ui.radioButton_assembler->isChecked())
    {
        hModule = LoadLibrary(TEXT("C:\\Users\\kamil\\source\\repos\\GaussianBlur\\x64\\Debug\\AsmLib.dll"));
    }
    else if (ui.radioButton_cpp->isChecked())
    {
        hModule = LoadLibrary(TEXT("C:\\Users\\kamil\\source\\repos\\GaussianBlur\\x64\\Debug\\CppLib.dll"));
    }
    
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
            StartCounter();

            //gauss(inputArrayWithFrame, outputArray, kernel, bmp.getWidth(), 50, bmp.getHeight(), size, sum);
            for (int i = 0; i < threads - 1; i++)
            {
                std::thread thread(gauss, inputArrayWithFrame, outputArray, kernel, bmp.getWidth(), i * rowsForThread, i * rowsForThread + rowsForThread, size, std::ref(sum));
                threadsVector.push_back(std::move(thread));
            }

            std::thread thread(gauss, inputArrayWithFrame, outputArray, kernel, bmp.getWidth(), (threads - 1) * rowsForThread, bmp.getHeight(), size, std::ref(sum));
            threadsVector.push_back(std::move(thread));

            for (int i = 0; i < threadsVector.size(); i++)
            {
                threadsVector[i].join();
            }

            time = GetCounter();

            if (ui.radioButton_assembler->isChecked())
            {
                ui.timeAsm->setText(QString::number(time));
            }
            else if (ui.radioButton_cpp->isChecked())
            {
                ui.timeCpp->setText(QString::number(time));
            }

            FreeLibrary(hModule);
        }
    }
    bmp.saveBitmap(outputArray);
}

void GaussianBlur::generateKernel(double** &kernel, char size, double sigma, double & sum)
{
    double r, s = 2.0 * sigma * sigma;
    int boundary = size / 2;

    kernel = new double*[size];
    for (int i = 0; i < size; i++)
        kernel[i] = new double[size];

    for (int x = -boundary; x <= boundary; x++)
    {
        for (int y = -boundary; y <= boundary; y++)
        {
            kernel[x + boundary][y + boundary] = (exp(-(x * x + y * y) / s)) / (M_PI * s);
            sum += kernel[x + boundary][y + boundary];
        }
    }
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            kernel[i][j] /= sum;
}

void GaussianBlur::StartCounter()
{
    LARGE_INTEGER li;

    if (!QueryPerformanceFrequency(&li))
    {
        //TODO error
    }

    PCFreq = double(li.QuadPart) / 1000.0;
    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}

double GaussianBlur::GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);

    return double(li.QuadPart - CounterStart) / PCFreq;
}
