#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsDropShadowEffect>
#include <QShortcut>
#include <QStackedWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QTableWidget>
#include "../Core/Game.h"
#include "BoardWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void gameTick();
    void onTimeTick();
    void handleDirectionInput(InputKey key);

private:
    // ── Core ──────────────────────────────────────
    Game game;
    QTimer *gameTimer;
    QTimer *timeTimer;
    int secondsElapsed;

    // ── UI Pages (QStackedWidget) ─────────────────
    QStackedWidget *stackedWidget;

    // Page 0: Main Menu
    QWidget    *pagMenu;
    QLabel     *lblTitle;
    QLabel     *lblSubtitle;
    QLineEdit  *inputPlayerName;
    QComboBox  *comboDifficulty;
    QCheckBox  *chkWallWrap;
    QPushButton *btnStart;
    QPushButton *btnLeaderboard;
    QPushButton *btnInstructions;
    QPushButton *btnQuit;

    // Page 1: Game
    QWidget     *pagGame;
    BoardWidget *m_boardWidget;
    QLabel      *lblScoreValue;
    QLabel      *lblLevelValue;
    QLabel      *lblLengthValue;
    QLabel      *lblSpeedValue;
    QLabel      *lblPowerUpValue;
    QLabel      *lblTimer;
    QLabel      *lblControls;
    QPushButton *btnPause;
    QPushButton *btnMenuFromGame;

    // Page 2: Game Over
    QWidget    *pagGameOver;
    QLabel     *lblGameOver;
    QLabel     *lblFinalScore;
    QLabel     *lblFinalLength;
    QLabel     *lblFinalLevel;
    QLabel     *lblFinalTime;
    QLineEdit  *inputGameOverName;
    QPushButton *btnSaveScore;
    QPushButton *btnPlayAgain;
    QPushButton *btnViewLeaderboard;
    QPushButton *btnBackToMenuFromGO;

    // Page 3: Leaderboard
    QWidget      *pagLeaderboard;
    QTableWidget *tableLeaderboard;
    QPushButton  *btnLeaderboardBack;

    // Page 4: Instructions
    QWidget    *pagInstructions;
    QPushButton *btnInstructionsBack;

    // ── Setup methods ─────────────────────────────
    void buildMenuPage();
    void buildGamePage();
    void buildGameOverPage();
    void buildLeaderboardPage();
    void buildInstructionsPage();
    void setupShortcuts();
    QString globalStyleSheet();

    // ── Runtime methods ───────────────────────────
    void updateBoardDisplay();
    void updateHUD();
    void showGameOverScreen();
    void refreshLeaderboardUI();
    QString generateIconBase64(const QString& type);
};

#endif // MAINWINDOW_H