#include "ised_win.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ised_win w;
    w.show();
    
    return a.exec();
}
