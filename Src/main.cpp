#include <QApplication>
#include <iostream>
#include "GUI/Mainwindow.h"
#include "core/Game.h"

int main(int argc, char *argv[]) {
    std::cout << "========================================\n";
    std::cout << "         SNAKE++ MODE SELECTION         \n";
    std::cout << "========================================\n";
    std::cout << " 1. Console Mode\n";
    std::cout << " 2. GUI Mode (Qt)\n";
    std::cout << "========================================\n";
    std::cout << "Choice: ";

    int choice;
    if (!(std::cin >> choice)) {
        std::cin.clear(); // Clear error state
        choice = 2;
    }

    if (choice == 1) {
        Game game;
        game.start();
        return 0;
    } else {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    }
}