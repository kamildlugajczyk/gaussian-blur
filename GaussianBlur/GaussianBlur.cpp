#include "GaussianBlur.h"
//#include <qfiledialog.h>
#include <QFileDialog>
#include <QPixmap>


GaussianBlur::GaussianBlur(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    
    connect(ui.pushButton_exit, SIGNAL(clicked()), this, SLOT(exit()));
}

void GaussianBlur::checkPaths()
{
    if (isInputPath && isOutputPath)
        ui.pushButton->setEnabled(true);
    else
        ui.pushButton->setEnabled(false);
}

void GaussianBlur::exit()
{
    QApplication::exit();
}


void GaussianBlur::on_toolButton_openInput_clicked()
{
    QString inputFileName = QFileDialog::getOpenFileName(this, tr("Open file"), "C://", "BMP Image (*.bmp)");
    
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
    QString outputFileName = QFileDialog::getSaveFileName(this, tr("Save file"), "C://", "BMP Image (*.bmp)");

    if (outputFileName != NULL)
    {
        ui.outputFilePath->setText(outputFileName);

        isOutputPath = true;

        checkPaths();
    }
}