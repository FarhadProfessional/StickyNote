#include "stickynote.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StickyNote w;
    w.show();
    return a.exec();
}
