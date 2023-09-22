#include "widget.h"
#include <QtWidgets/QApplication>
#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    QPalette pal = QPalette();
    QColor color(25, 25, 100);
    pal.setColor(QPalette::Window, color);

    w.setAutoFillBackground(true);
    w.setPalette(pal);
    w.move(500, 10);
    w.show();
    return a.exec();
}
