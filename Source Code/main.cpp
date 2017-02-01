#include "loa.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LOA w;
    w.initial();
    w.show();
    return a.exec();
}
