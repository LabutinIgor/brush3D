#include <QApplication>

#include "brush_ui/src/ui/mainwindow.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
