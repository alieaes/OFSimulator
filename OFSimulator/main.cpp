#include "stdafx.h"
#include "OFSimulator.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OFSimulator w;
    w.show();
    return a.exec();
}
