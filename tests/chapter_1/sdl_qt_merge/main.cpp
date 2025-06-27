#include "imagemerge.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageMerge w;
    w.show();
    return a.exec();
}
