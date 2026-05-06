//
// Created by DELL on 5/6/2026.
//

#ifndef SNAKE_GAME_MAINWINDOW1_H
#define SNAKE_GAME_MAINWINDOW1_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class Mainwindow1;
}

QT_END_NAMESPACE

class Mainwindow1 : public QWidget {
    Q_OBJECT

public:
    explicit Mainwindow1(QWidget *parent = nullptr);

    ~Mainwindow1() override;

private:
    Ui::Mainwindow1 *ui{};
};


#endif //SNAKE_GAME_MAINWINDOW1_H