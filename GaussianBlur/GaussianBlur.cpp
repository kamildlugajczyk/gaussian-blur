#include "GaussianBlur.h"
//#include <qfiledialog.h>
#include <QFileDialog>


GaussianBlur::GaussianBlur(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton_exit, SIGNAL(clicked()), this, SLOT(exit()));
    
}


void GaussianBlur::exit()
{
    QApplication::exit();
}


void GaussianBlur::on_toolButton_open_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), "C://", "BMP Image (*.BMP)");
    ui.filePath->setText(fileName);

    if(fileName != NULL)
        ui.pushButton->setEnabled(true);
    else
        ui.pushButton->setEnabled(false);
}
