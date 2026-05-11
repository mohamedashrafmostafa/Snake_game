#include "BoardWidget.h"
#include <QPainter>
#include <QPainterPath>
#include <cmath>

// ─── Color Palette ────────────────────────────────────────
// Inspired by the reference image: teal/green snake on dark navy
static const QColor COL_BG_DARK    ("#0A0E1A");   // Main background
static const QColor COL_BG_CELL_A  ("#0D1520");   // Checkerboard A
static const QColor COL_BG_CELL_B  ("#111D2B");   // Checkerboard B
static const QColor COL_BORDER     ("#13BFA6");   // Board border (teal glow)

static const QColor COL_SNAKE_HEAD ("#24D17E");   // Snake head — bright green
static const QColor COL_SNAKE_BODY ("#13BFA6");   // Snake body outer — teal
static const QColor COL_SNAKE_INNER("#B9F65D");   // Snake body inner highlight — yellow-green

static const QColor COL_FOOD_NORMAL("#F7E85C");   // Normal food — yellow
static const QColor COL_FOOD_BONUS ("#FF9BD2");   // Bonus food — pink
static const QColor COL_FOOD_SPEED ("#FFB347");   // Speed food — orange

static const QColor COL_PU_SPEED   ("#00E5FF");   // Power-up: Speed — cyan
static const QColor COL_PU_INVINCE ("#B48EF0");   // Power-up: Invincibility — purple
static const QColor COL_PU_MULTI   ("#FFD700");   // Power-up: Score Multiplier — gold

static const QColor COL_OBSTACLE   ("#2E7D32");   // Obstacle — dark forest green
static const QColor COL_OBS_INNER  ("#1B5E20");   // Obstacle inner — darker


BoardWidget::BoardWidget(Game* game, QWidget *parent)
    : QWidget(parent), m_game(game) {
    setFocusPolicy(Qt::NoFocus);
    setMinimumSize(400, 400);
}

void BoardWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int cols = m_game->getBoard().getWidth();
    int rows = m_game->getBoard().getHeight();

    // Calculate cell size to fit widget, keeping square cells
    int cellW = width() / cols;
    int cellH = height() / rows;
    int cellSize = qMin(cellW, cellH);

    // Center the board
    int totalW = cols * cellSize;
    int totalH = rows * cellSize;
    int xOffset = (width()  - totalW) / 2;
    int yOffset = (height() - totalH) / 2;

    painter.translate(xOffset, yOffset);

    drawGrid(painter, cols, rows, cellSize, cellSize);
    drawObstacles(painter, cellSize, cellSize);
    drawPowerUps(painter, cellSize, cellSize);
    drawFood(painter, cellSize, cellSize);
    drawSnake(painter, cellSize, cellSize);
}

// ─── Grid: Checkerboard dark background ──────────────────
void BoardWidget::drawGrid(QPainter& painter, int cols, int rows, int cellW, int cellH) {
    // Fill entire board area
    painter.fillRect(0, 0, cols * cellW, rows * cellH, COL_BG_DARK);

    // Checkerboard pattern
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            QRect cell(x * cellW, y * cellH, cellW, cellH);
            painter.fillRect(cell, ((x + y) % 2 == 0) ? COL_BG_CELL_A : COL_BG_CELL_B);
        }
    }

    // Border glow
    QPen borderPen(COL_BORDER, 2);
    painter.setPen(borderPen);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(0, 0, cols * cellW, rows * cellH);
}

// ─── Snake: Teal body + yellow-green inner (matching reference) ──
void BoardWidget::drawSnake(QPainter& painter, int cellW, int cellH) {
    if (m_game->getSnake().getBody().empty()) return;

    auto body = m_game->getSnake().getBody();
    int pad = 1;        // outer padding
    int innerPad = 4;   // inner highlight padding

    painter.setPen(Qt::NoPen);

    // Draw body segments (tail to head-1)
    for (size_t i = 1; i < body.size(); ++i) {
        Position p = body[i];
        QRect outerRect(p.x * cellW + pad, p.y * cellH + pad, cellW - 2*pad, cellH - 2*pad);
        QRect innerRect(p.x * cellW + innerPad, p.y * cellH + innerPad,
                        cellW - 2*innerPad, cellH - 2*innerPad);

        // Outer teal block
        painter.setBrush(COL_SNAKE_BODY);
        painter.drawRoundedRect(outerRect, 3, 3);

        // Inner yellow-green highlight (like the reference image)
        painter.setBrush(COL_SNAKE_INNER);
        painter.drawRoundedRect(innerRect, 2, 2);
    }

    // Draw head — brighter, with eyes
    Position head = body.front();
    QRect headRect(head.x * cellW + pad, head.y * cellH + pad, cellW - 2*pad, cellH - 2*pad);

    // Head gradient: bright green
    QLinearGradient headGrad(headRect.topLeft(), headRect.bottomRight());
    headGrad.setColorAt(0, COL_SNAKE_HEAD);
    headGrad.setColorAt(1, COL_SNAKE_HEAD.lighter(120));
    painter.setBrush(headGrad);
    painter.drawRoundedRect(headRect, 5, 5);

    // Inner highlight on head too
    QRect headInner(head.x * cellW + innerPad, head.y * cellH + innerPad,
                    cellW - 2*innerPad, cellH - 2*innerPad);
    painter.setBrush(COL_SNAKE_INNER);
    painter.drawRoundedRect(headInner, 3, 3);

    // Eyes (two dark dots)
    painter.setBrush(QColor("#0A0E1A"));
    int eyeSize = qMax(cellW / 6, 2);
    Position dir = m_game->getSnake().getDirection();

    int eyeX1, eyeY1, eyeX2, eyeY2;
    int cx = headRect.center().x();
    int cy = headRect.center().y();

    if (dir.x == 1) { // Right
        eyeX1 = cx + cellW/6; eyeY1 = cy - cellH/5;
        eyeX2 = cx + cellW/6; eyeY2 = cy + cellH/5;
    } else if (dir.x == -1) { // Left
        eyeX1 = cx - cellW/6; eyeY1 = cy - cellH/5;
        eyeX2 = cx - cellW/6; eyeY2 = cy + cellH/5;
    } else if (dir.y == -1) { // Up
        eyeX1 = cx - cellW/5; eyeY1 = cy - cellH/6;
        eyeX2 = cx + cellW/5; eyeY2 = cy - cellH/6;
    } else { // Down
        eyeX1 = cx - cellW/5; eyeY1 = cy + cellH/6;
        eyeX2 = cx + cellW/5; eyeY2 = cy + cellH/6;
    }
    painter.drawEllipse(QPoint(eyeX1, eyeY1), eyeSize, eyeSize);
    painter.drawEllipse(QPoint(eyeX2, eyeY2), eyeSize, eyeSize);
}

// ─── Food: QPainter fruit shapes per type ────────────────
void BoardWidget::drawFood(QPainter& painter, int cellW, int cellH) {
    Position fPos = m_game->getFood().getPosition();
    FoodType fType = m_game->getFood().getType();

    int pad = 2;
    QRect cell(fPos.x * cellW + pad, fPos.y * cellH + pad, cellW - 2*pad, cellH - 2*pad);
    QPoint c = cell.center();
    int w = cell.width(), h = cell.height();

    painter.setPen(Qt::NoPen);

    switch (fType) {
    case NORMAL: {
        // Apple shape — red/yellow rounded body with stem and leaf
        int bodyH = h * 6 / 8;
        int bodyW = w * 7 / 8;
        QRect bodyRect(c.x() - bodyW/2, cell.bottom() - bodyH, bodyW, bodyH);

        // Body gradient
        QRadialGradient grad(bodyRect.center(), bodyW/2);
        grad.setColorAt(0, QColor("#FF6B6B"));
        grad.setColorAt(0.6, QColor("#E84545"));
        grad.setColorAt(1, QColor("#C62828"));
        painter.setBrush(grad);
        painter.drawEllipse(bodyRect);

        // Highlight
        painter.setBrush(QColor(255,255,255,60));
        painter.drawEllipse(bodyRect.adjusted(bodyW/4, bodyH/6, -bodyW/4, -bodyH/2));

        // Stem
        painter.setPen(QPen(QColor("#5D4037"), qMax(1, w/10)));
        painter.drawLine(c.x(), bodyRect.top(), c.x(), bodyRect.top() - h/6);
        painter.setPen(Qt::NoPen);

        // Leaf
        QPainterPath leaf;
        int lx = c.x() + 1, ly = bodyRect.top() - h/8;
        leaf.moveTo(lx, ly);
        leaf.cubicTo(lx + w/4, ly - h/5, lx + w/3, ly, lx + w/6, ly + h/6);
        painter.setBrush(QColor("#66BB6A"));
        painter.drawPath(leaf);
        break;
    }
    case BONUS: {
        // Berry cluster — 3 overlapping pink circles with stem
        int r = qMax(w/5, 3);
        QColor berryCol("#FF69B4");

        // Three berries in triangle
        painter.setBrush(QColor(berryCol.red(), berryCol.green(), berryCol.blue(), 50));
        painter.drawEllipse(c, w/2 + 2, h/2 + 2); // subtle glow

        QRadialGradient g1(c.x()-r/2, c.y()+r/2, r*2);
        g1.setColorAt(0, QColor("#FF9BD2")); g1.setColorAt(1, berryCol);
        painter.setBrush(g1);
        painter.drawEllipse(QPoint(c.x()-r, c.y()+r/2), r, r);

        QRadialGradient g2(c.x()+r/2, c.y()+r/2, r*2);
        g2.setColorAt(0, QColor("#FFB6E0")); g2.setColorAt(1, berryCol);
        painter.setBrush(g2);
        painter.drawEllipse(QPoint(c.x()+r, c.y()+r/2), r, r);

        QRadialGradient g3(c.x(), c.y()-r/3, r*2);
        g3.setColorAt(0, QColor("#FFC8E8")); g3.setColorAt(1, berryCol);
        painter.setBrush(g3);
        painter.drawEllipse(QPoint(c.x(), c.y()-r/2), r, r);

        // Stem
        painter.setPen(QPen(QColor("#5D4037"), qMax(1, w/12)));
        painter.drawLine(c.x(), c.y()-r-r/2, c.x(), c.y()-r-r);
        painter.setPen(Qt::NoPen);

        // Leaf
        QPainterPath leaf;
        int lx = c.x(), ly = c.y()-r-r/2;
        leaf.moveTo(lx, ly);
        leaf.cubicTo(lx+w/5, ly-h/6, lx+w/4, ly, lx+w/8, ly+h/8);
        painter.setBrush(QColor("#66BB6A"));
        painter.drawPath(leaf);
        break;
    }
    case SPEED_UP: {
        // Chili pepper — curved red/orange shape
        QPainterPath pepper;
        int px = cell.left() + w/4;
        int py = cell.top() + h/6;

        pepper.moveTo(px + w/2, py);
        pepper.cubicTo(px + w*3/4, py + h/4,
                       px + w*2/3, py + h*2/3,
                       px + w/3, py + h*3/4);
        pepper.cubicTo(px, py + h*5/6,
                       px - w/8, py + h/2,
                       px + w/6, py + h/6);
        pepper.cubicTo(px + w/4, py, px + w/3, py, px + w/2, py);
        pepper.closeSubpath();

        QLinearGradient pepGrad(cell.topLeft(), cell.bottomRight());
        pepGrad.setColorAt(0, QColor("#FF6D00"));
        pepGrad.setColorAt(0.5, QColor("#FF3D00"));
        pepGrad.setColorAt(1, QColor("#DD2C00"));
        painter.setBrush(pepGrad);
        painter.drawPath(pepper);

        // Stem
        painter.setPen(QPen(QColor("#33691E"), qMax(1, w/8)));
        painter.drawLine(px + w/2, py, px + w/2 + w/8, py - h/6);
        painter.setPen(Qt::NoPen);
        break;
    }
    }
}

// ─── Obstacles: Dark forest green blocks ─────────────────
void BoardWidget::drawObstacles(QPainter& painter, int cellW, int cellH) {
    const auto& obstacles = m_game->getBoard().getObstacles();

    painter.setPen(Qt::NoPen);

    for (const Position& obs : obstacles) {
        QRect outerRect(obs.x * cellW + 1, obs.y * cellH + 1, cellW - 2, cellH - 2);
        QRect innerRect(obs.x * cellW + 3, obs.y * cellH + 3, cellW - 6, cellH - 6);

        // Outer block
        painter.setBrush(COL_OBSTACLE);
        painter.drawRoundedRect(outerRect, 2, 2);

        // Inner darker block for texture
        painter.setBrush(COL_OBS_INNER);
        painter.drawRoundedRect(innerRect, 1, 1);

        // Cross pattern for visual texture
        painter.setPen(QPen(COL_OBSTACLE.darker(130), 1));
        painter.drawLine(outerRect.topLeft() + QPoint(2,2), outerRect.bottomRight() - QPoint(2,2));
        painter.drawLine(outerRect.topRight() + QPoint(-2,2), outerRect.bottomLeft() + QPoint(2,-2));
        painter.setPen(Qt::NoPen);
    }
}

// ─── Power-Ups: Colored circles with letter icons ───────
void BoardWidget::drawPowerUps(QPainter& painter, int cellW, int cellH) {
    const auto& powerUps = m_game->getActivePowerUps();

    int pad = 3;
    painter.setPen(Qt::NoPen);

    for (const PowerUp& pu : powerUps) {
        QRect puRect(pu.pos.x * cellW + pad, pu.pos.y * cellH + pad, cellW - 2*pad, cellH - 2*pad);
        QPoint center = puRect.center();

        QColor puColor;
        QString puText;

        switch (pu.type) {
        case P_SPEED_UP:
            puColor = COL_PU_SPEED;
            puText = "S";
            break;
        case INVINCIBILITY:
            puColor = COL_PU_INVINCE;
            puText = "I";
            break;
        case SCORE_MULTIPLIER:
            puColor = COL_PU_MULTI;
            puText = "×2";
            break;
        }

        // Glow effect
        QRadialGradient glow(center, cellW / 2);
        glow.setColorAt(0, puColor);
        glow.setColorAt(1, QColor(puColor.red(), puColor.green(), puColor.blue(), 0));
        painter.setBrush(glow);
        painter.drawEllipse(puRect.adjusted(-2, -2, 2, 2));

        // Main circle
        painter.setBrush(puColor);
        painter.drawEllipse(puRect);

        // Label
        painter.setPen(QColor("#0A0E1A"));
        QFont f = painter.font();
        f.setPixelSize(qMax(cellW / 3, 8));
        f.setBold(true);
        painter.setFont(f);
        painter.drawText(puRect, Qt::AlignCenter, puText);
        painter.setPen(Qt::NoPen);
    }
}
