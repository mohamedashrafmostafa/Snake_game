#include <QApplication>
#include "GUI/Mainwindow.h"
#include "Core/Game.h"

int main(int argc, char *argv[]) {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
}