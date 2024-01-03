#include "AIS_Login.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AIS_Login w;
    w.show();
    return a.exec();
}
