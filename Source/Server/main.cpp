#include "ServerTCP.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerTCP w;
    w.show();
    return a.exec();
}
