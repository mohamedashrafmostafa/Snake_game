#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsDropShadowEffect>
#include <QShortcut>
#include "../Core/Game.h"
#include "BoardWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_btnStart_clicked();
    void on_btnLeaderboard_clicked();
    void on_btnQuit_clicked();
    void on_btnPause_clicked();
    void on_btnMenuFromGame_clicked();
    void on_btnPlayAgain_clicked();
    void on_btnBackToMenu_clicked();
    
    void gameTick();
    void onTimeTick();
    void handleDirectionInput(InputKey key);

private:
    Ui::MainWindow *ui;
    Game game;
    QTimer *gameTimer;
    QTimer *timeTimer;
    int secondsElapsed;
    BoardWidget *m_boardWidget;

    void setupBoardUI();
    void setupStyles();
    void setupShortcuts();
    void updateBoardDisplay();
    void updateHUD();
    void showGameOverScreen();
    void refreshLeaderboardUI();
};

#endif // MAINWINDOW_H