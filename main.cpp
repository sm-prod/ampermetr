#include "amp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AMP w;
    w.show();

    return a.exec();
}
