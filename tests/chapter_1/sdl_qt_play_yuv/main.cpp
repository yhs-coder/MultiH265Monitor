#include "playyuv.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlayYUV w;
    w.show();
    return a.exec();
}
