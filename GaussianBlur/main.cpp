#include "GaussianBlur.h"
#include <QtWidgets/QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GaussianBlur w;
    w.show();

    return a.exec();
}
