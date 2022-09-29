#include "bingowindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BingoWindow w;
    w.show();
    return a.exec();
}
