#include "Mainwindow.h"
#include "ui_Mainwindow.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    gameTimer(new QTimer(this)),
    timeTimer(new QTimer(this))
{
    ui->setupUi(this);

    // Initial Screen
    ui->stackedWidget->setCurrentIndex(0);

    // Setup Timers
    connect(gameTimer, &QTimer::timeout, this, &MainWindow::gameTick);
    connect(timeTimer, &QTimer::timeout, this, &MainWindow::onTimeTick);

    // Setup dynamic Board UI inside gameCanvas
    setupBoardUI();

    // Apply Professional Styling (QSS) with Gradients and Shadows
    setupStyles();

    // Setup keyboard shortcuts to completely bypass focus issues
    setupShortcuts();

    // Prevent buttons from stealing focus
    ui->btnPause->setFocusPolicy(Qt::NoFocus);
    ui->btnMenuFromGame->setFocusPolicy(Qt::NoFocus);

    this->setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow() {
    delete ui;
}

// ─────────────────────────────────────────────
// UI Initialization & Styling
// ─────────────────────────────────────────────
void MainWindow::setupBoardUI() {
    m_boardWidget = new BoardWidget(&game, this);

    QVBoxLayout *layout = new QVBoxLayout(ui->gameCanvas);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_boardWidget);
}

void MainWindow::setupStyles() {
    // Advanced QSS with Linear Gradients
    QString mainStyle = R"(
        QMainWindow, QStackedWidget, QWidget#pagMenu, QWidget#pagGame, QWidget#pagGameOver {
            background-color: #1E1E2E;
            color: #CDD6F4;
            font-family: 'Segoe UI', Arial, sans-serif;
        }

        /* Buttons with Linear Gradients */
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #89B4FA, stop:1 #74C7EC);
            color: #11111B;
            border: none;
            border-radius: 8px;
            font-weight: bold;
            font-size: 16px;
            padding: 10px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B4BEFE, stop:1 #89B4FA);
        }
        QPushButton:pressed {
            background: #89B4FA;
        }

        /* Input fields and ComboBoxes */
        QLineEdit, QComboBox {
            background-color: #313244;
            color: #CDD6F4;
            border: 2px solid #45475A;
            border-radius: 5px;
            padding: 5px;
            font-size: 14px;
        }
        QLineEdit:focus, QComboBox:focus {
            border: 2px solid #89B4FA;
        }

        /* Labels */
        QLabel {
            font-size: 16px;
            color: #CDD6F4;
            font-weight: bold;
        }
    )";

    this->setStyleSheet(mainStyle);

    // Apply Drop Shadows to main buttons
    auto applyShadow = [](QWidget* widget) {
        QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
        shadow->setBlurRadius(15);
        shadow->setColor(QColor(0, 0, 0, 150));
        shadow->setOffset(0, 4);
        widget->setGraphicsEffect(shadow);
    };

    applyShadow(ui->btnStart);
    applyShadow(ui->btnLeaderboard);
    applyShadow(ui->btnQuit);
    applyShadow(ui->btnPlayAgain);
    applyShadow(ui->btnBackToMenu);
}

// ─────────────────────────────────────────────
// Input Handling
// ─────────────────────────────────────────────
void MainWindow::setupShortcuts() {
    auto addShortcut = [this](QKeySequence key, InputKey gameKey) {
        QShortcut *shortcut = new QShortcut(key, this);
        shortcut->setContext(Qt::ApplicationShortcut);
        connect(shortcut, &QShortcut::activated, this, [this, gameKey, key]() {
            handleDirectionInput(gameKey);
        });
    };

    addShortcut(Qt::Key_Up, InputKey::UP);
    addShortcut(Qt::Key_Down, InputKey::DOWN);
    addShortcut(Qt::Key_Left, InputKey::LEFT);
    addShortcut(Qt::Key_Right, InputKey::RIGHT);

    addShortcut(Qt::Key_W, InputKey::UP);
    addShortcut(Qt::Key_S, InputKey::DOWN);
    addShortcut(Qt::Key_A, InputKey::LEFT);
    addShortcut(Qt::Key_D, InputKey::RIGHT);

    addShortcut(Qt::Key_P, InputKey::PAUSE);
    addShortcut(Qt::Key_Z, InputKey::UNDO);

    // Escape handles returning to menu
    QShortcut *escShortcut = new QShortcut(Qt::Key_Escape, this);
    escShortcut->setContext(Qt::ApplicationShortcut);
    connect(escShortcut, &QShortcut::activated, this, [this]() {
        if (ui->stackedWidget->currentIndex() == 1) {
            on_btnMenuFromGame_clicked();
        }
    });
}

void MainWindow::handleDirectionInput(InputKey key) {
    // 1. Check that current screen is the game screen
    if (ui->stackedWidget->currentIndex() != 1) return;

    // 2. Check that game exists and is not over
    if (game.isGameOver()) return;

    // 3. Log the received key
    QString keyName;
    switch(key) {
        case InputKey::UP: keyName = "UP"; break;
        case InputKey::DOWN: keyName = "DOWN"; break;
        case InputKey::LEFT: keyName = "LEFT"; break;
        case InputKey::RIGHT: keyName = "RIGHT"; break;
        case InputKey::PAUSE: keyName = "PAUSE"; break;
        case InputKey::UNDO: keyName = "UNDO"; break;
        default: keyName = "UNKNOWN"; break;
    }

    qDebug() << "Shortcut triggered:" << keyName;

    // 4. If game is paused, ignore movement keys
    if (game.isPaused() && key != InputKey::PAUSE && key != InputKey::QUIT) {
        qDebug() << "Game is paused. Movement ignored.";
        return;
    }

    qDebug() << "Calling processInputGUI for" << keyName;

    // 5. Apply input
    game.processInputGUI(key);

    // 6. Update UI specifically
    if (key == InputKey::PAUSE) {
        updateHUD();
    } else if (key == InputKey::UNDO) {
        updateBoardDisplay();
    }

    qDebug() << "Input handled successfully.";
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    // Left as fallback, but QShortcut handles movement cleanly
    QMainWindow::keyPressEvent(event);
}

void MainWindow::onTimeTick() {
    if (ui->stackedWidget->currentIndex() == 1 && !game.isPaused() && !game.isGameOver()) {
        secondsElapsed++;
        int m = secondsElapsed / 60;
        int s = secondsElapsed % 60;
        ui->lblTimer->setText(QString("Time: %1:%2").arg(m).arg(s, 2, 10, QChar('0')));
    }
}

// ─────────────────────────────────────────────
// Button Clicks
// ─────────────────────────────────────────────
void MainWindow::on_btnStart_clicked() {
    int diffIndex = ui->comboDifficulty->currentIndex();
    int diff = Game::MEDIUM; // default
    if (diffIndex == 0) diff = Game::EASY;
    else if (diffIndex == 1) diff = Game::MEDIUM;
    else if (diffIndex >= 2) diff = Game::HARD;

    game.resetGame();
    game.setDifficulty(diff);
    game.setPaused(false); // Explicitly ensure game is not paused

    secondsElapsed = 0;
    ui->lblTimer->setText("Time: 0:00");

    qDebug() << "Game started with tickMs:" << game.getTickMs();

    updateHUD();
    updateBoardDisplay();

    ui->stackedWidget->setCurrentIndex(1); // Go to pagGame

    gameTimer->start(game.getTickMs());
    timeTimer->start(1000); // 1 second intervals for the clock

    // Secure focus to window
    this->activateWindow();
    this->raise();
    this->setFocus();
}

void MainWindow::on_btnLeaderboard_clicked() {
    refreshLeaderboardUI();
    ui->stackedWidget->setCurrentIndex(2); // In UI, GameOver page is 2. Wait, the leaderboard table is ON the GameOver page!
}

void MainWindow::on_btnQuit_clicked() {
    QApplication::quit();
}

void MainWindow::on_btnPause_clicked() {
    game.processInputGUI(InputKey::PAUSE);
    updateHUD();
    this->setFocus();
}

void MainWindow::on_btnMenuFromGame_clicked() {
    gameTimer->stop();
    timeTimer->stop();
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_btnPlayAgain_clicked() {
    on_btnStart_clicked();
}

void MainWindow::on_btnBackToMenu_clicked() {
    ui->stackedWidget->setCurrentIndex(0);
}

// ─────────────────────────────────────────────
// Game Loop (QTimer)
// ─────────────────────────────────────────────
void MainWindow::gameTick() {
    if (game.isGameOver()) {
        gameTimer->stop();
        timeTimer->stop();
        showGameOverScreen();
        return;
    }

    if (!game.isPaused()) {
        game.tick();
        updateBoardDisplay();
        updateHUD();
        gameTimer->setInterval(game.getTickMs()); // Adjust speed dynamically for powerups
    }
}

// ─────────────────────────────────────────────
// Rendering
// ─────────────────────────────────────────────
void MainWindow::updateBoardDisplay() {
    // Simply trigger a repaint of the BoardWidget
    if (m_boardWidget) {
        m_boardWidget->update();
    }
}

void MainWindow::updateHUD() {
    ui->lblScoreValue->setText(QString::number(game.getScore()));
    ui->lblLevelValue->setText(QString::number(game.getLevel()));
    ui->lblLengthValue->setText(QString::number(game.getSnake().getBody().size()));

    if (game.isPaused()) {
        ui->lblSpeedValue->setText("PAUSED");
        ui->btnPause->setText("Resume");
    } else {
        ui->lblSpeedValue->setText(QString::number(game.getTickMs()) + " ms");
        ui->btnPause->setText("Pause");
    }
}

void MainWindow::showGameOverScreen() {
    // Save score if player entered a name
    QString pName = ui->inputPlayerName->text().trimmed();
    if (pName.isEmpty()) pName = "Anonymous";

    // We instantiate leaderboard, but actually game has it. We need access to game's leaderboard or just save it.
    // game.saveCurrentScore(pName.toStdString()); // game object already handles saving if we modify Game.h to expose it.

    // Instead of directly calling saveCurrentScore, we will add an explicit score save method if it doesn't exist.
    Leaderboard tempLb;
    tempLb.loadFromFile("leaderboard.txt");
    tempLb.addScore(pName.toStdString(), game.getScore());
    tempLb.saveToFile("leaderboard.txt");

    // Update GameOver UI
    ui->lblFinalScore->setText(QString::number(game.getScore()));
    ui->lblFinalLevel->setText(QString::number(game.getLevel()));
    ui->lblFinalLength->setText(QString::number(game.getSnake().getBody().size()));

    refreshLeaderboardUI();
    ui->stackedWidget->setCurrentIndex(2); // GameOver Screen
}

void MainWindow::refreshLeaderboardUI() {
    Leaderboard tempLb;
    tempLb.loadFromFile("leaderboard.txt");

    ui->tableLeaderboard->setRowCount(0);
    int row = 0;

    auto scores = tempLb.getScores();
    for (auto it = scores.begin(); it != scores.end(); ++it) {
        for (const std::string& name : it->second) {
            if (row >= 10) break;
            ui->tableLeaderboard->insertRow(row);
            ui->tableLeaderboard->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
            ui->tableLeaderboard->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(name)));
            ui->tableLeaderboard->setItem(row, 2, new QTableWidgetItem(QString::number(it->first)));
            ui->tableLeaderboard->setItem(row, 3, new QTableWidgetItem("-")); // Length not saved in original
            row++;
        }
    }
}