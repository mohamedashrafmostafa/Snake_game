#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
#include <QPainter>
#include "../Core/Game.h"

class BoardWidget : public QWidget {
    Q_OBJECT
public:
    explicit BoardWidget(Game* game, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Game* m_game;

    void drawGrid(QPainter& painter, int cols, int rows, int cellW, int cellH);
    void drawSnake(QPainter& painter, int cellW, int cellH);
    void drawFood(QPainter& painter, int cellW, int cellH);
    void drawObstacles(QPainter& painter, int cellW, int cellH);
    void drawPowerUps(QPainter& painter, int cellW, int cellH);
};

#endif // BOARDWIDGET_H
