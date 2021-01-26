#include "GaussianBlur.h"
#include "BmpManager.h"
#include <QFileDialog>
#include <QPixmap>
#include <QtCharts>
#include <Windows.h>
#include <cmath>
#include <vector>
#include <thread> 
#define M_PI 3.14159265358979323846

typedef void(__cdecl* pGauss)(unsigned char* inputArray, unsigned char* outputArray, float* kernel,
    int32_t width, int32_t startHeight, int32_t stopHeight, int32_t size, float sum);

double PCFreq = 0.0;
__int64 CounterStart = 0;

GaussianBlur::GaussianBlur(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.spinBox_threads->setValue(std::thread::hardware_concurrency());
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
        ui.inputImage->setAlignment(Qt::AlignCenter);
        ui.progressBar->setValue(0);

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
        ui.progressBar->setValue(0);
    }
}

void GaussianBlur::on_pushButton_start_clicked()
{
    int lastValue = 0;
    ui.progressBar->setValue(40);
    size = 5;

    if (ui.radioButton_5x5->isChecked()) size = 5;
    else if (ui.radioButton_7x7->isChecked()) size = 7;
    else if (ui.radioButton_9x9->isChecked()) size = 9;
    else if (ui.radioButton_11x11->isChecked()) size = 11;
    else if (ui.radioButton_13x13->isChecked()) size = 13;
    else if (ui.radioButton_17x17->isChecked()) size = 17;
    else if (ui.radioButton_25x25->isChecked()) size = 25;
    else if (ui.radioButton_33x33->isChecked()) size = 33;

    sigma = size / 7.0;

    threads = ui.spinBox_threads->value();

    float sum = 0.0, time = 0.0;
    HMODULE hModule;
    BmpManager bmp(inputFileName.toStdString(), outputFileName.toStdString());

    if (bmp.loadBitmap(inputArray, inputArrayWithFrame, outputArray, size) == -1)
    {
        QMessageBox::critical(this, "Error", "Nastapil problem z plikiem wejsciowym! Dalsze czynnosci zostaja przerwane.");
        ui.progressBar->setValue(0);
        return;
    }

    for (int i = 0; i < 10; i++)
        ui.progressBar->setValue(++lastValue);
   
    //bmp.mirrorBoundaries(inputArray, size);

    // TODO Devide file if necessary

    generateKernel(kernel, size, sigma, sum);

    std::vector<std::thread> threadsVector;

    int rowsForThread = bmp.getHeight() / threads;

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
        QMessageBox::critical(this, "Error", "Nastapil blad podczas wczytywania funkcji dll!");
        ui.progressBar->setValue(0);
    }
    else
    {
        pGauss gauss = (pGauss)GetProcAddress(hModule, "gauss");

        if (gauss == NULL)
        {
            QMessageBox::critical(this, "Error", "Nastapil blad podczas wczytywania funkcji dll!");
            ui.progressBar->setValue(0);
        }
        else
        {
            for (int i = 0; i < 10; i++)
                ui.progressBar->setValue(++lastValue);

            StartCounter();

            for (int i = 0; i < threads - 1; i++)
            {
                std::thread thread(gauss, inputArrayWithFrame, outputArray, kernel, bmp.getWidth(), i * rowsForThread, i * rowsForThread + rowsForThread, size, sum);
                threadsVector.push_back(std::move(thread));
            }

            std::thread thread(gauss, inputArrayWithFrame, outputArray, kernel, bmp.getWidth(), (threads - 1) * rowsForThread, bmp.getHeight(), size, sum);
            threadsVector.push_back(std::move(thread));

            for (int i = 0; i < threadsVector.size(); i++)
            {
                threadsVector[i].join();
            }

            time = GetCounter();

            for (int i = 0; i < 50; i++)
                ui.progressBar->setValue(++lastValue);

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
    if (bmp.saveBitmap(outputArray) == -1)
    {
        QMessageBox::critical(this, "Error", "Nastapil problem z plikiem wynikowym!");
        ui.progressBar->setValue(0);
        return;
    }

    if (chartInput != nullptr)
    {
        chartInput->removeSeries(series);
        series->clear();
    }

    bmp.makeHistogram(outputArray, outputR, outputG, outputB, bmp.getWidth(), 0, bmp.getHeight());
    showHistogram(outputR, outputG, outputB, false);
    

    bmp.makeHistogram(inputArray, inputR, inputG, inputB, bmp.getWidth(), 0, bmp.getHeight());
    showHistogram(inputR, inputG, inputB, true);

    for (int i = 0; i < 30; i++)
        ui.progressBar->setValue(++lastValue);

    QPixmap pix(outputFileName);
    int width = ui.inputImage->width();
    int height = ui.inputImage->height();

    ui.outputImage->setPixmap(pix.scaled(width, height, Qt::KeepAspectRatio));
    ui.outputImage->setAlignment(Qt::AlignCenter);

    for (int i = 0; i < 10; i++)
        ui.progressBar->setValue(++lastValue);
}

void GaussianBlur::generateKernel(float* &kernel, char size, float sigma, float& sum)
{
    float r, s = 2.0 * sigma * sigma;
    int boundary = size / 2;

    kernel = new float[size * size];

    for (int x = -boundary; x <= boundary; x++)
    {
        for (int y = -boundary; y <= boundary; y++)
        {
            kernel[(x + boundary) * size + y + boundary] = (exp(-(x * x + y * y) / s)) / (M_PI * s);
            sum += kernel[(x + boundary) * size + y + boundary];
        }
    }

    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            kernel[i * size + j] /= sum;
}

void GaussianBlur::StartCounter()
{
    LARGE_INTEGER li;

    if (!QueryPerformanceFrequency(&li))
    {
        QMessageBox::critical(this, "Error", "Nastapil blad podczas zliczania czasu!");
        ui.progressBar->setValue(0);
        return;
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

void GaussianBlur::showHistogram(uint64_t*& R, uint64_t*& G, uint64_t*& B, bool input)
{
    QBarSet* setRed = new QBarSet("Red");
    QBarSet* setGreen = new QBarSet("Green");
    QBarSet* setBlue = new QBarSet("Blue");
    series = new QBarSeries();

    for (int i = 0; i < 256; i++) {
        *setRed << R[i];
        *setGreen << G[i];
        *setBlue << B[i];
        int a = 4;
    }

    setRed->setColor(QColor(255, 0, 0));
    setGreen->setColor(QColor(0, 255, 0));
    setBlue->setColor(QColor(0, 0, 255));
    series->append(setRed);
    series->append(setGreen);
    series->append(setBlue);

    chartInput = new QChart();
    chartInput->addSeries(series);
    chartInput->setAnimationOptions(QChart::SeriesAnimations);

    chartInput->legend()->setVisible(false);
    series->setBarWidth(3);

    if (input)
    {
        ui.inputHistogram->setChart(chartInput);
        ui.inputHistogram->setRenderHint(QPainter::Antialiasing);
        ui.inputHistogram->show();
    }
    else
    {
        ui.outputHistogram->setChart(chartInput);
        ui.outputHistogram->setRenderHint(QPainter::Antialiasing);
        ui.outputHistogram->show();
    }
}
