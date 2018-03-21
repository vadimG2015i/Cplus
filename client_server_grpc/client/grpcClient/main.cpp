#include "grpctest.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    grpcTest w;
    w.show();

    return a.exec();
}
