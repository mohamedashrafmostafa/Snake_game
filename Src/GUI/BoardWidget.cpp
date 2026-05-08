#include "BoardWidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QFile>

BoardWidget::BoardWidget(Game* game, QWidget *parent)
    : QWidget(parent), m_game(game) {
    // Avoid focus stealing
    setFocusPolicy(Qt::NoFocus);
}

void BoardWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Board dimensions (fixed at 20x20 logically based on Game.cpp constants)
    int cols = 20;
    int rows = 20;

    int cellW = width() / cols;
    int cellH = height() / rows;

    // Center the board if window aspect ratio isn't perfect square
    int xOffset = (width() - (cols * cellW)) / 2;
    int yOffset = (height() - (rows * cellH)) / 2;

    painter.translate(xOffset, yOffset);

    drawGrid(painter, cols * cellW, rows * cellH, cellW, cellH);
    drawObstacles(painter, cellW, cellH);
    drawFood(painter, cellW, cellH);
    drawSnake(painter, cellW, cellH);
}

void BoardWidget::drawGrid(QPainter& painter, int w, int h, int cellW, int cellH) {
    // Background
    painter.fillRect(0, 0, w, h, QColor("#181825"));

    // Grid lines
    painter.setPen(QColor("#313244"));
    for (int i = 0; i <= w; i += cellW) {
        painter.drawLine(i, 0, i, h);
    }
    for (int i = 0; i <= h; i += cellH) {
        painter.drawLine(0, i, w, i);
    }
    
    // Border
    painter.setPen(QPen(QColor("#89B4FA"), 2));
    painter.drawRect(0, 0, w, h);
}

void BoardWidget::drawSnake(QPainter& painter, int cellW, int cellH) {
    if (m_game->getSnake().getBody().empty()) return;

    auto body = m_game->getSnake().getBody();
    
    // Draw Tail and Body
    painter.setPen(Qt::NoPen);
    for (size_t i = 1; i < body.size(); ++i) {
        Position p = body[i];
        QRect rect(p.x * cellW + 2, p.y * cellH + 2, cellW - 4, cellH - 4);
        
        QLinearGradient grad(rect.topLeft(), rect.bottomRight());
        grad.setColorAt(0, QColor("#A6E3A1")); // Green
        grad.setColorAt(1, QColor("#87C282")); // Darker Green
        
        painter.setBrush(grad);
        painter.drawRoundedRect(rect, 4, 4);
    }

    // Draw Head
    Position head = body.front();
    QRect headRect(head.x * cellW + 1, head.y * cellH + 1, cellW - 2, cellH - 2);
    
    QPixmap headPix("assets/snake_head.png");
    if (!headPix.isNull()) {
        painter.drawPixmap(headRect, headPix);
    } else {
        // Fallback: Custom QPainter Head
        QLinearGradient headGrad(headRect.topLeft(), headRect.bottomRight());
        headGrad.setColorAt(0, QColor("#94E2D5")); // Teal
        headGrad.setColorAt(1, QColor("#74C7EC")); // Blueish
        painter.setBrush(headGrad);
        painter.drawRoundedRect(headRect, 6, 6);
        
        // Draw eyes
        painter.setBrush(Qt::black);
        painter.drawEllipse(headRect.x() + cellW/4, headRect.y() + cellH/4, cellW/5, cellH/5);
        painter.drawEllipse(headRect.x() + cellW*2/3, headRect.y() + cellH/4, cellW/5, cellH/5);
    }
}

void BoardWidget::drawFood(QPainter& painter, int cellW, int cellH) {
    Position fPos = m_game->getFood().getPosition();
    QRect foodRect(fPos.x * cellW + 2, fPos.y * cellH + 2, cellW - 4, cellH - 4);

    QPixmap foodPix("assets/food.png");
    if (!foodPix.isNull()) {
        painter.drawPixmap(foodRect, foodPix);
    } else {
        // Fallback: Red Apple
        QRadialGradient grad(foodRect.center(), cellW/2);
        grad.setColorAt(0, QColor("#F38BA8")); // Light Red
        grad.setColorAt(1, QColor("#EBA0AC")); // Darker Red
        
        painter.setBrush(grad);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(foodRect);
        
        // Stem
        painter.setPen(QPen(QColor("#A6E3A1"), 2));
        painter.drawLine(foodRect.center().x(), foodRect.top() + 2, foodRect.center().x() + 3, foodRect.top() - 2);
    }
}

void BoardWidget::drawObstacles(QPainter& painter, int cellW, int cellH) {
    QPixmap obsPix("assets/obstacle.png");
    
    for (int y = 0; y < 20; ++y) {
        for (int x = 0; x < 20; ++x) {
            if (m_game->getBoard().isObstacle({x, y})) {
                QRect obsRect(x * cellW, y * cellH, cellW, cellH);
                
                if (!obsPix.isNull()) {
                    painter.drawPixmap(obsRect, obsPix);
                } else {
                    // Fallback: Steel block
                    painter.setPen(QColor("#11111B"));
                    QLinearGradient grad(obsRect.topLeft(), obsRect.bottomRight());
                    grad.setColorAt(0, QColor("#585B70"));
                    grad.setColorAt(1, QColor("#45475A"));
                    
                    painter.setBrush(grad);
                    painter.drawRect(obsRect);
                    
                    // Rivets
                    painter.setBrush(QColor("#181825"));
                    painter.drawEllipse(obsRect.topLeft() + QPoint(2,2), 1, 1);
                    painter.drawEllipse(obsRect.topRight() + QPoint(-2,2), 1, 1);
                    painter.drawEllipse(obsRect.bottomLeft() + QPoint(2,-2), 1, 1);
                    painter.drawEllipse(obsRect.bottomRight() + QPoint(-2,-2), 1, 1);
                }
            }
        }
    }
}
