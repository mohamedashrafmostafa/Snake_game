#include "Mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QApplication>
#include <QScrollArea>
#include <QFrame>
#include <QMessageBox>
#include <QFont>
#include <QPainterPath>
#include <QBuffer>
#include <QPixmap>

// ─── Snake logo widget (QPainter drawn & Animated) ───
class SnakeLogoWidget : public QWidget {
    QTimer* animTimer;
    double t;
public:
    SnakeLogoWidget(QWidget* parent = nullptr) : QWidget(parent), t(0.0) {
        setFixedSize(80, 80);
        animTimer = new QTimer(this);
        connect(animTimer, &QTimer::timeout, this, [this](){
            t += 0.12; // animation speed
            if(t > 2 * 3.14159) t -= 2 * 3.14159;
            update();
        });
        animTimer->start(50); // 50ms frame rate (~20 FPS)
    }
protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        int w = width(), h = height();
        int cx = w/2, cy = h/2;

        int numSegments = 14;
        double spacing = 0.35; // Distance between segments

        // Draw body segments (tail first so head renders on top)
        for (int i = numSegments - 1; i >= 0; i--) {
            double st = t - i * spacing;
            // Figure-8 infinity path
            double x = cx + 28 * cos(st);
            double y = cy + 16 * sin(2 * st);
            
            if (i == 0) { // Head
                double r = 10.5;
                p.setPen(QPen(QColor("#0A0E1A"), 2));
                p.setBrush(QColor("#24D17E")); // Bright green head
                p.drawEllipse(QPointF(x, y), r, r);
                
                // Calculate direction vector for eyes & tongue
                double dx = -28 * sin(st);
                double dy = 32 * cos(2 * st);
                double len = std::sqrt(dx*dx + dy*dy);
                dx /= len; dy /= len;
                
                // Normal vector for eye positioning
                double nx = -dy, ny = dx;
                
                // Eyes
                p.setBrush(QColor("#2a05fc"));
                p.setPen(Qt::NoPen);
                p.drawEllipse(QPointF(x + dx*2 + nx*3, y + dy*2 + ny*3), 1.5, 1.5);
                p.drawEllipse(QPointF(x + dx*2 - nx*3, y + dy*2 - ny*3), 1.5, 1.5);
                
                // Tongue
                p.setPen(QPen(QColor("#ff0000"), 1.5, Qt::SolidLine, Qt::RoundCap));
                p.drawLine(QPointF(x + dx*7, y + dy*7), QPointF(x + dx*11, y + dy*11));
            } else { // Body
                double r = 6.0 - (i * 0.15); // Tapere towards tail
                p.setPen(QPen(QColor("#0A0E1A"), 1.5));
                p.setBrush(QColor("#13BFA6")); // Teal body
                p.drawEllipse(QPointF(x, y), r, r);
                
                // Subtle yellow-green highlight
                p.setPen(Qt::NoPen);
                p.setBrush(QColor("#B9F65D"));
                p.drawEllipse(QPointF(x - r*0.3, y - r*0.3), r*0.3, r*0.3);
            }
        }
    }
};

// ─── Color constants ───
static const QString BG     = "#0A0E1A";
static const QString PANEL  = "#111827";
static const QString ACCENT = "#24D17E";
static const QString TEXT1  = "#F5F7FA";
static const QString TEXT2  = "#9CA3AF";
static const QString DANGER = "#FF4D6D";

static QPushButton* makeBtn(const QString& text, const QString& bg = ACCENT) {
    auto* b = new QPushButton(text);
    b->setMinimumHeight(48); b->setMinimumWidth(160); b->setCursor(Qt::PointingHandCursor);
    b->setFocusPolicy(Qt::NoFocus);
    b->setStyleSheet(QString(
        "QPushButton{background:%1;color:#0A0E1A;border:none;border-radius:10px;font:bold 15px 'Segoe UI';padding:10px 24px;}"
        "QPushButton:hover{background:%2;}"
        "QPushButton:pressed{background:%1;}").arg(bg, bg == ACCENT ? "#5DFFA0" : "#FF7A9A"));
    return b;
}
static QLabel* makeLabel(const QString& t, int sz=14, const QString& col=TEXT1, bool bold=false) {
    auto* l = new QLabel(t);
    l->setStyleSheet(QString("color:%1;font:%2 %3px 'Segoe UI';background:transparent;").arg(col, bold?"bold":"normal", QString::number(sz)));
    l->setAlignment(Qt::AlignCenter);
    return l;
}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent),
    gameTimer(new QTimer(this)), timeTimer(new QTimer(this)), secondsElapsed(0)
{
    setWindowTitle("Snake++");
    setMinimumSize(950, 700);
    resize(1050, 780);
    setStyleSheet(globalStyleSheet());

    auto* central = new QWidget(this);
    setCentralWidget(central);
    auto* mainLay = new QVBoxLayout(central);
    mainLay->setContentsMargins(0,0,0,0);

    stackedWidget = new QStackedWidget;
    mainLay->addWidget(stackedWidget);

    buildMenuPage();
    buildGamePage();
    buildGameOverPage();
    buildLeaderboardPage();
    buildInstructionsPage();

    stackedWidget->setCurrentIndex(0);

    connect(gameTimer, &QTimer::timeout, this, &MainWindow::gameTick);
    connect(timeTimer, &QTimer::timeout, this, &MainWindow::onTimeTick);
    setupShortcuts();
    setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow() {}

// ════════════════════════════════════════════
//  PAGE 0: MAIN MENU
// ════════════════════════════════════════════
void MainWindow::buildMenuPage() {
    pagMenu = new QWidget;
    auto* lay = new QVBoxLayout(pagMenu);
    lay->setAlignment(Qt::AlignCenter);
    lay->setSpacing(12);
    lay->setContentsMargins(60,30,60,30);

    // Snake logo + title row
    auto* logoWidget = new SnakeLogoWidget;
    auto* logoRow = new QHBoxLayout;
    logoRow->setAlignment(Qt::AlignCenter);
    logoRow->setSpacing(12);
    logoRow->addStretch();
    logoRow->addWidget(logoWidget);
    lblTitle = makeLabel("SNAKE++", 52, ACCENT, true);
    logoRow->addWidget(lblTitle);
    logoRow->addStretch();

    lblSubtitle = makeLabel("Advanced Data-Structure Snake Game", 15, TEXT2);
    lay->addStretch(2);
    lay->addLayout(logoRow);
    lay->addWidget(lblSubtitle);
    lay->addSpacing(30);

    // ── Centered vertical form ──
    QString inputStyle = QString("background:%1;color:%2;border:2px solid #1E293B;border-radius:8px;padding:10px;font:14px 'Segoe UI';").arg(PANEL,TEXT1);
    QString inputFocus = QString("border-color:%1;").arg(ACCENT);
    int formWidth = 360;

    auto* formWidget = new QWidget;
    formWidget->setFixedWidth(formWidth);
    auto* form = new QVBoxLayout(formWidget);
    form->setSpacing(4);
    form->setContentsMargins(0,0,0,0);

    auto* lblName = makeLabel("Player Name", 12, TEXT2, true);
    lblName->setAlignment(Qt::AlignLeft);
    form->addWidget(lblName);
    inputPlayerName = new QLineEdit;
    inputPlayerName->setPlaceholderText("Enter your name...");
    inputPlayerName->setMaxLength(20);
    inputPlayerName->setFixedWidth(formWidth);
    inputPlayerName->setStyleSheet(QString("QLineEdit{%1}QLineEdit:focus{%2}").arg(inputStyle, inputFocus));
    form->addWidget(inputPlayerName);
    form->addSpacing(12);

    auto* lblDiff = makeLabel("Difficulty", 12, TEXT2, true);
    lblDiff->setAlignment(Qt::AlignLeft);
    form->addWidget(lblDiff);
    comboDifficulty = new QComboBox;
    comboDifficulty->addItems({"Easy  -  Slow speed, few obstacles","Medium  -  Moderate speed","Hard  -  Fast speed, many obstacles"});
    comboDifficulty->setCurrentIndex(1);
    comboDifficulty->setFixedWidth(formWidth);
    comboDifficulty->setStyleSheet(QString("QComboBox{%1}QComboBox:focus{%2}QComboBox::drop-down{border:none;}QComboBox QAbstractItemView{background:%3;color:%4;selection-background-color:#1E293B;}").arg(inputStyle, inputFocus, PANEL, TEXT1));
    form->addWidget(comboDifficulty);
    form->addSpacing(12);

    chkWallWrap = new QCheckBox("  Wall Wrap Mode  (pass through walls)");
    chkWallWrap->setStyleSheet(QString("QCheckBox{color:%1;font:13px 'Segoe UI';spacing:8px;}QCheckBox::indicator{width:20px;height:20px;border-radius:4px;border:2px solid #1E293B;background:%2;}QCheckBox::indicator:checked{background:%3;border-color:%3;}").arg(TEXT1,PANEL,ACCENT));
    form->addWidget(chkWallWrap);

    auto* formRow = new QHBoxLayout;
    formRow->addStretch(); formRow->addWidget(formWidget); formRow->addStretch();
    lay->addLayout(formRow);

    lay->addSpacing(30);
    auto* btnRow = new QHBoxLayout;
    btnRow->setSpacing(14);
    btnRow->addStretch();
    btnStart = makeBtn("Start Game");
    btnLeaderboard = makeBtn("Leaderboard");
    btnInstructions = makeBtn("How to Play");
    btnQuit = makeBtn("Quit", DANGER);
    btnRow->addWidget(btnStart); btnRow->addWidget(btnLeaderboard);
    btnRow->addWidget(btnInstructions); btnRow->addWidget(btnQuit);
    btnRow->addStretch();
    lay->addLayout(btnRow);
    lay->addStretch(2);

    connect(btnStart, &QPushButton::clicked, this, [this](){
        int di = comboDifficulty->currentIndex();
        int diff = (di==0)?Game::EASY:(di==1)?Game::MEDIUM:Game::HARD;
        game.setDifficulty(diff); // Add obstacles first
        game.resetGame();         // Reset snake and spawn food (respecting obstacles)
        game.setWallWrap(chkWallWrap->isChecked());
        game.setPaused(false);
        secondsElapsed = 0;
        updateHUD();
        updateBoardDisplay();
        stackedWidget->setCurrentIndex(1);
        gameTimer->start(game.getTickMs());
        timeTimer->start(1000);
        activateWindow(); raise(); setFocus();
    });
    connect(btnLeaderboard, &QPushButton::clicked, this, [this](){
        refreshLeaderboardUI(); stackedWidget->setCurrentIndex(3);
    });
    connect(btnInstructions, &QPushButton::clicked, this, [this](){
        stackedWidget->setCurrentIndex(4);
    });
    connect(btnQuit, &QPushButton::clicked, this, [](){ QApplication::quit(); });

    stackedWidget->addWidget(pagMenu);
}

// ════════════════════════════════════════════
//  PAGE 1: GAME
// ════════════════════════════════════════════
void MainWindow::buildGamePage() {
    pagGame = new QWidget;
    auto* lay = new QVBoxLayout(pagGame);
    lay->setSpacing(0); lay->setContentsMargins(0,0,0,0);

    // HUD Bar
    auto* hud = new QFrame;
    hud->setStyleSheet(QString("QFrame{background:%1;border-bottom:2px solid %2;}").arg(PANEL,ACCENT));
    hud->setFixedHeight(60);
    auto* hudLay = new QHBoxLayout(hud);
    hudLay->setContentsMargins(16,4,16,4); hudLay->setSpacing(20);

    auto addStat = [&](const QString& cap, QLabel*& val) {
        auto* vl = new QVBoxLayout; vl->setSpacing(0);
        vl->addWidget(makeLabel(cap, 10, TEXT2, true));
        val = makeLabel("0", 18, ACCENT, true);
        vl->addWidget(val);
        hudLay->addLayout(vl);
    };
    addStat("SCORE", lblScoreValue);
    addStat("LEVEL", lblLevelValue);
    addStat("LENGTH", lblLengthValue);
    addStat("SPEED", lblSpeedValue);
    addStat("POWER-UP", lblPowerUpValue);

    hudLay->addStretch();
    btnPause = makeBtn("Pause"); btnPause->setFixedWidth(120); btnPause->setMinimumHeight(38);
    btnMenuFromGame = makeBtn("Menu"); btnMenuFromGame->setFixedWidth(100); btnMenuFromGame->setMinimumHeight(38);
    hudLay->addWidget(btnPause); hudLay->addWidget(btnMenuFromGame);
    lay->addWidget(hud);

    // Board
    m_boardWidget = new BoardWidget(&game, this);
    m_boardWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    lay->addWidget(m_boardWidget, 1);

    // Footer
    auto* footer = new QFrame;
    footer->setStyleSheet(QString("QFrame{background:%1;border-top:1px solid #1E293B;}").arg(PANEL));
    footer->setFixedHeight(32);
    auto* fLay = new QHBoxLayout(footer);
    fLay->setContentsMargins(16,2,16,2);
    lblControls = makeLabel("↑↓←→ / WASD move  |  P pause  |  ESC menu", 11, TEXT2);
    lblControls->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    lblTimer = makeLabel("Time: 0:00", 12, TEXT1, true);
    lblTimer->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    fLay->addWidget(lblControls); fLay->addWidget(lblTimer);
    lay->addWidget(footer);

    connect(btnPause, &QPushButton::clicked, this, [this](){
        game.processInputGUI(InputKey::PAUSE); updateHUD(); setFocus();
    });
    connect(btnMenuFromGame, &QPushButton::clicked, this, [this](){
        gameTimer->stop(); timeTimer->stop(); stackedWidget->setCurrentIndex(0);
    });

    stackedWidget->addWidget(pagGame);
}

// ════════════════════════════════════════════
//  PAGE 2: GAME OVER
// ════════════════════════════════════════════
void MainWindow::buildGameOverPage() {
    pagGameOver = new QWidget;
    auto* lay = new QVBoxLayout(pagGameOver);
    lay->setAlignment(Qt::AlignCenter); lay->setSpacing(16); lay->setContentsMargins(40,20,40,20);

    lay->addStretch();
    lblGameOver = makeLabel("GAME OVER", 42, DANGER, true);
    lay->addWidget(lblGameOver);
    lay->addSpacing(10);

    auto* statsFrame = new QFrame;
    statsFrame->setStyleSheet(QString("QFrame{background:%1;border-radius:12px;border:1px solid #1E293B;}").arg(PANEL));
    auto* sLay = new QHBoxLayout(statsFrame);
    sLay->setSpacing(32); sLay->setContentsMargins(24,16,24,16);

    auto addFinal = [&](const QString& cap, QLabel*& val) {
        auto* vl = new QVBoxLayout; vl->setSpacing(2);
        vl->addWidget(makeLabel(cap, 12, TEXT2, true));
        val = makeLabel("0", 28, ACCENT, true);
        vl->addWidget(val);
        sLay->addLayout(vl);
    };
    addFinal("SCORE", lblFinalScore);
    addFinal("LENGTH", lblFinalLength);
    addFinal("LEVEL", lblFinalLevel);
    addFinal("TIME", lblFinalTime);
    lay->addWidget(statsFrame);
    lay->addSpacing(10);

    auto* nameFrame = new QFrame;
    nameFrame->setStyleSheet(QString("QFrame{background:%1;border-radius:12px;border:1px solid #1E293B;}").arg(PANEL));
    auto* nLay = new QHBoxLayout(nameFrame);
    nLay->setContentsMargins(20,12,20,12); nLay->setSpacing(10);
    nLay->addWidget(makeLabel("Your Name:", 14, TEXT1, true));
    inputGameOverName = new QLineEdit;
    inputGameOverName->setPlaceholderText("Enter name to save score...");
    inputGameOverName->setMaxLength(20);
    inputGameOverName->setStyleSheet(QString("QLineEdit{background:#0D1520;color:%1;border:2px solid #1E293B;border-radius:8px;padding:8px;font:14px 'Segoe UI';}QLineEdit:focus{border-color:%2;}").arg(TEXT1,ACCENT));
    nLay->addWidget(inputGameOverName, 1);
    btnSaveScore = makeBtn("Save Score"); btnSaveScore->setMinimumWidth(140);
    nLay->addWidget(btnSaveScore);
    lay->addWidget(nameFrame);
    lay->addSpacing(10);

    auto* bRow = new QHBoxLayout; bRow->setSpacing(12); bRow->addStretch();
    btnPlayAgain = makeBtn("Play Again");
    btnViewLeaderboard = makeBtn("Leaderboard");
    btnBackToMenuFromGO = makeBtn("Main Menu");
    bRow->addWidget(btnPlayAgain); bRow->addWidget(btnViewLeaderboard); bRow->addWidget(btnBackToMenuFromGO);
    bRow->addStretch();
    lay->addLayout(bRow);
    lay->addStretch();

    connect(btnSaveScore, &QPushButton::clicked, this, [this](){
        QString name = inputGameOverName->text().trimmed();
        if (name.isEmpty()) { QMessageBox::warning(this,"Name Required","Please enter your name."); return; }
        game.saveCurrentScore(name.toStdString());
        btnSaveScore->setEnabled(false); btnSaveScore->setText("Saved");
    });
    connect(btnPlayAgain, &QPushButton::clicked, this, [this](){ btnSaveScore->setEnabled(true); btnSaveScore->setText("Save Score"); btnStart->click(); });
    connect(btnViewLeaderboard, &QPushButton::clicked, this, [this](){ refreshLeaderboardUI(); stackedWidget->setCurrentIndex(3); });
    connect(btnBackToMenuFromGO, &QPushButton::clicked, this, [this](){ btnSaveScore->setEnabled(true); btnSaveScore->setText("Save Score"); stackedWidget->setCurrentIndex(0); });

    stackedWidget->addWidget(pagGameOver);
}

// ════════════════════════════════════════════
//  PAGE 3: LEADERBOARD
// ════════════════════════════════════════════
void MainWindow::buildLeaderboardPage() {
    pagLeaderboard = new QWidget;
    auto* lay = new QVBoxLayout(pagLeaderboard);
    lay->setContentsMargins(40,20,40,20); lay->setSpacing(16);

    lay->addWidget(makeLabel("TOP 10 LEADERBOARD", 28, ACCENT, true));
    tableLeaderboard = new QTableWidget(0, 3);
    tableLeaderboard->setHorizontalHeaderLabels({"Rank","Player","Score"});
    tableLeaderboard->horizontalHeader()->setStretchLastSection(true);
    tableLeaderboard->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableLeaderboard->verticalHeader()->setVisible(false);
    tableLeaderboard->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableLeaderboard->setSelectionMode(QAbstractItemView::NoSelection);
    tableLeaderboard->setStyleSheet(QString(
        "QTableWidget{background:%1;color:%2;border:1px solid #1E293B;border-radius:8px;font:13px 'Segoe UI';gridline-color:#1E293B;}"
        "QHeaderView::section{background:#1E293B;color:%3;font:bold 13px 'Segoe UI';padding:8px;border:none;}"
        "QTableWidget::item{padding:6px;}").arg(PANEL,TEXT1,ACCENT));
    tableLeaderboard->setAlternatingRowColors(true);
    tableLeaderboard->setStyleSheet(tableLeaderboard->styleSheet() + QString("QTableWidget{alternate-background-color:#0D1520;}"));
    lay->addWidget(tableLeaderboard);

    auto* bRow = new QHBoxLayout; bRow->addStretch();
    btnLeaderboardBack = makeBtn("Back to Menu"); bRow->addWidget(btnLeaderboardBack); bRow->addStretch();
    lay->addLayout(bRow);

    connect(btnLeaderboardBack, &QPushButton::clicked, this, [this](){ stackedWidget->setCurrentIndex(0); });
    stackedWidget->addWidget(pagLeaderboard);
}

// ════════════════════════════════════════════
//  PAGE 4: INSTRUCTIONS
// ════════════════════════════════════════════
void MainWindow::buildInstructionsPage() {
    pagInstructions = new QWidget;
    auto* lay = new QVBoxLayout(pagInstructions);
    lay->setContentsMargins(40,20,40,20); lay->setSpacing(8);

    lay->addWidget(makeLabel("HOW TO PLAY", 28, ACCENT, true));

    auto* scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet(QString("QScrollArea{background:%1;border:none;}QScrollBar:vertical{background:%1;width:8px;border-radius:4px;}QScrollBar::handle:vertical{background:#1E293B;border-radius:4px;min-height:30px;}QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical{height:0;}").arg(BG));
    auto* content = new QWidget;
    auto* cLay = new QVBoxLayout(content);
    cLay->setSpacing(8);

    QString info = QString(R"(
<div style='color:#F5F7FA;font-family:Segoe UI;font-size:14px;line-height:1.7;'>

<h3 style='color:#24D17E; margin-top:12px;'>Objective</h3>
<p>Navigate your snake across the board to collect food. Each piece of food makes the snake grow longer. Avoid colliding with walls, obstacles, or your own tail.</p>

<h3 style='color:#24D17E; margin-top:16px;'>Controls</h3>
<table style='color:#9CA3AF;font-size:13px;' cellpadding='3'>
<tr><td style='color:#F5F7FA;' width='140'>Arrow Keys / WASD</td><td>Change direction</td></tr>
<tr><td style='color:#F5F7FA;'>P</td><td>Pause or resume the game</td></tr>
<tr><td style='color:#F5F7FA;'>ESC</td><td>Return to main menu</td></tr>
</table>

<h3 style='color:#24D17E; margin-top:16px;'>Food Types</h3>
<table style='color:#9CA3AF;font-size:13px;' cellpadding='3'>
<tr><td width='40'><img src='%1' width='24' height='24'></td><td width='120'><b>Apple</b></td><td>Standard food, awards 10 points</td></tr>
<tr><td><img src='%2' width='24' height='24'></td><td><b>Berry</b></td><td>Bonus food, awards 25 points</td></tr>
<tr><td><img src='%3' width='24' height='24'></td><td><b>Pepper</b></td><td>Speed food, awards 15 points and temporarily boosts speed</td></tr>
</table>

<h3 style='color:#24D17E; margin-top:16px;'>Power-Ups</h3>
<table style='color:#9CA3AF;font-size:13px;' cellpadding='3'>
<tr><td width='40'><img src='%4' width='24' height='24'></td><td width='120'><b>Speed Boost</b></td><td>Temporarily increases movement speed</td></tr>
<tr><td><img src='%5' width='24' height='24'></td><td><b>Invincibility</b></td><td>Makes the snake immune to walls and obstacles</td></tr>
<tr><td><img src='%6' width='24' height='24'></td><td><b>Multiplier</b></td><td>Doubles all points earned while active</td></tr>
</table>

<h3 style='color:#24D17E; margin-top:16px;'>Obstacles</h3>
<table style='color:#9CA3AF;font-size:13px;' cellpadding='3'>
<tr><td width='40'><img src='%7' width='24' height='24'></td><td>Green blocks placed on the board. Hitting one ends the game unless invincibility is active. Higher difficulty levels add more obstacles.</td></tr>
</table>

<h3 style='color:#24D17E; margin-top:16px;'>Wall Wrap Mode</h3>
<p>When enabled from the main menu, the snake passes through board edges and reappears on the opposite side instead of dying.</p>

<h3 style='color:#24D17E; margin-top:16px;'>Data Structures Used</h3>
<table style='color:#9CA3AF;font-size:13px;' cellpadding='4'>
<tr><td style='color:#24D17E;' width='140'>std::deque</td><td>Snake body segments, allowing O(1) insertion and removal at both ends</td></tr>
<tr><td style='color:#24D17E;'>unordered_set</td><td>Fast O(1) collision lookups for snake body and board obstacles</td></tr>
<tr><td style='color:#24D17E;'>std::vector</td><td>Active power-up storage, supporting iteration and indexed removal</td></tr>
<tr><td style='color:#24D17E;'>std::map (BST)</td><td>Leaderboard scores, automatically sorted in descending order</td></tr>
</table>

</div>)").arg(
        generateIconBase64("apple"), generateIconBase64("berry"), generateIconBase64("pepper"),
        generateIconBase64("speed"), generateIconBase64("invince"), generateIconBase64("multi"),
        generateIconBase64("obs")
    );

    auto* infoLabel = new QLabel(info);
    infoLabel->setWordWrap(true);
    infoLabel->setTextFormat(Qt::RichText);
    infoLabel->setStyleSheet(QString("background:%1;padding:16px;border-radius:12px;border:1px solid #1E293B;").arg(PANEL));
    cLay->addWidget(infoLabel);
    scroll->setWidget(content);
    lay->addWidget(scroll);

    auto* bRow = new QHBoxLayout; bRow->addStretch();
    btnInstructionsBack = makeBtn("Back to Menu"); bRow->addWidget(btnInstructionsBack); bRow->addStretch();
    lay->addLayout(bRow);

    connect(btnInstructionsBack, &QPushButton::clicked, this, [this](){ stackedWidget->setCurrentIndex(0); });
    stackedWidget->addWidget(pagInstructions);
}

// ════════════════════════════════════════════
//  SHORTCUTS
// ════════════════════════════════════════════
void MainWindow::setupShortcuts() {
    auto add = [this](QKeySequence k, InputKey gk) {
        auto* s = new QShortcut(k, this);
        s->setContext(Qt::ApplicationShortcut);
        connect(s, &QShortcut::activated, this, [this,gk](){ handleDirectionInput(gk); });
    };
    add(Qt::Key_Up,InputKey::UP); add(Qt::Key_Down,InputKey::DOWN);
    add(Qt::Key_Left,InputKey::LEFT); add(Qt::Key_Right,InputKey::RIGHT);
    add(Qt::Key_W,InputKey::UP); add(Qt::Key_S,InputKey::DOWN);
    add(Qt::Key_A,InputKey::LEFT); add(Qt::Key_D,InputKey::RIGHT);
    add(Qt::Key_P,InputKey::PAUSE);

    auto* esc = new QShortcut(Qt::Key_Escape, this);
    esc->setContext(Qt::ApplicationShortcut);
    connect(esc, &QShortcut::activated, this, [this](){
        if (stackedWidget->currentIndex()==1) { gameTimer->stop(); timeTimer->stop(); stackedWidget->setCurrentIndex(0); }
    });
}

// ════════════════════════════════════════════
//  INPUT
// ════════════════════════════════════════════
void MainWindow::handleDirectionInput(InputKey key) {
    if (stackedWidget->currentIndex() != 1) return;
    if (game.isGameOver()) return;
    if (game.isPaused() && key != InputKey::PAUSE && key != InputKey::QUIT) return;
    game.processInputGUI(key);
    if (key == InputKey::PAUSE) updateHUD();
}

void MainWindow::keyPressEvent(QKeyEvent *event) { QMainWindow::keyPressEvent(event); }

void MainWindow::onTimeTick() {
    if (stackedWidget->currentIndex()==1 && !game.isPaused() && !game.isGameOver()) {
        secondsElapsed++;
        lblTimer->setText(QString("Time: %1:%2").arg(secondsElapsed/60).arg(secondsElapsed%60,2,10,QChar('0')));
    }
}

// ════════════════════════════════════════════
//  GAME LOOP
// ════════════════════════════════════════════
void MainWindow::gameTick() {
    if (game.isGameOver()) { gameTimer->stop(); timeTimer->stop(); showGameOverScreen(); return; }
    if (!game.isPaused()) {
        game.tick();
        updateBoardDisplay(); updateHUD();
        if (game.isGameOver()) { gameTimer->stop(); timeTimer->stop(); showGameOverScreen(); return; }
        gameTimer->setInterval(game.getTickMs());
    }
}

void MainWindow::updateBoardDisplay() { if (m_boardWidget) m_boardWidget->update(); }

void MainWindow::updateHUD() {
    lblScoreValue->setText(QString::number(game.getScore()));
    lblLevelValue->setText(QString::number(game.getLevel()));
    lblLengthValue->setText(QString::number(game.getSnake().getBody().size()));
    if (game.isPaused()) {
        lblSpeedValue->setText("PAUSED"); btnPause->setText("Resume");
    } else {
        lblSpeedValue->setText(QString::number(game.getTickMs()) + "ms"); btnPause->setText("Pause");
    }
    // Power-up status
    if (game.isInvincible()) lblPowerUpValue->setText("Shield " + QString::number(game.getPowerUpTicksLeft()));
    else if (game.getScoreMultiplier()>1) lblPowerUpValue->setText("x" + QString::number(game.getScoreMultiplier()) + " " + QString::number(game.getPowerUpTicksLeft()));
    else if (game.getTickMs()<150 && game.getPowerUpTicksLeft()>0) lblPowerUpValue->setText("Speed " + QString::number(game.getPowerUpTicksLeft()));
    else lblPowerUpValue->setText("None");
}

void MainWindow::showGameOverScreen() {
    lblFinalScore->setText(QString::number(game.getScore()));
    lblFinalLength->setText(QString::number(game.getSnake().getBody().size()));
    lblFinalLevel->setText(QString::number(game.getLevel()));
    lblFinalTime->setText(QString("%1:%2").arg(secondsElapsed/60).arg(secondsElapsed%60,2,10,QChar('0')));
    QString pName = inputPlayerName->text().trimmed();
    inputGameOverName->setText(pName);
    btnSaveScore->setEnabled(true); btnSaveScore->setText("💾 Save Score");
    stackedWidget->setCurrentIndex(2);
}

void MainWindow::refreshLeaderboardUI() {
    Leaderboard lb; lb.loadFromFile("leaderboard.txt");
    tableLeaderboard->setRowCount(0);
    int row=0;
    for (auto it=lb.getScores().begin(); it!=lb.getScores().end() && row<10; ++it) {
        for (const auto& name : it->second) {
            if (row>=10) break;
            tableLeaderboard->insertRow(row);
            auto makeItem = [](const QString& t) { auto* i=new QTableWidgetItem(t); i->setTextAlignment(Qt::AlignCenter); return i; };
            tableLeaderboard->setItem(row,0,makeItem(QString::number(row+1)));
            tableLeaderboard->setItem(row,1,makeItem(QString::fromStdString(name)));
            tableLeaderboard->setItem(row,2,makeItem(QString::number(it->first)));
            row++;
        }
    }
}

QString MainWindow::globalStyleSheet() {
    return QString("QMainWindow,QWidget{background:%1;color:%2;font-family:'Segoe UI';}").arg(BG,TEXT1);
}

QString MainWindow::generateIconBase64(const QString& type) {
    QPixmap pix(24, 24);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    p.setRenderHint(QPainter::Antialiasing);
    
    int w = 24, h = 24;
    int cx = w/2, cy = h/2;
    QRect cell(0, 0, w, h);
    p.setPen(Qt::NoPen);

    if (type == "apple") {
        int bodyH = h * 6 / 8;
        int bodyW = w * 7 / 8;
        QRect bodyRect(cx - bodyW/2, cell.bottom() - bodyH, bodyW, bodyH);
        QRadialGradient grad(bodyRect.center(), bodyW/2);
        grad.setColorAt(0, QColor("#FF6B6B")); grad.setColorAt(0.6, QColor("#E84545")); grad.setColorAt(1, QColor("#C62828"));
        p.setBrush(grad); p.drawEllipse(bodyRect);
        p.setBrush(QColor(255,255,255,60)); p.drawEllipse(bodyRect.adjusted(bodyW/4, bodyH/6, -bodyW/4, -bodyH/2));
        p.setPen(QPen(QColor("#5D4037"), 2)); p.drawLine(cx, bodyRect.top(), cx, bodyRect.top() - h/6); p.setPen(Qt::NoPen);
        QPainterPath leaf; leaf.moveTo(cx + 1, bodyRect.top() - h/8); leaf.cubicTo(cx + w/4, bodyRect.top() - h/8 - h/5, cx + w/3, bodyRect.top() - h/8, cx + w/6, bodyRect.top() - h/8 + h/6);
        p.setBrush(QColor("#66BB6A")); p.drawPath(leaf);
    } else if (type == "berry") {
        int r = 5; QColor berryCol("#FF69B4"); QPoint c(cx, cy);
        p.setBrush(QColor(berryCol.red(), berryCol.green(), berryCol.blue(), 50)); p.drawEllipse(c, w/2, h/2);
        auto drawB = [&](int x, int y, QColor c1) { QRadialGradient g(x-r/2, y+r/2, r*2); g.setColorAt(0, c1); g.setColorAt(1, berryCol); p.setBrush(g); p.drawEllipse(QPoint(x, y), r, r); };
        drawB(c.x()-r, c.y()+r/2, QColor("#FF9BD2")); drawB(c.x()+r, c.y()+r/2, QColor("#FFB6E0")); drawB(c.x(), c.y()-r/2, QColor("#FFC8E8"));
        p.setPen(QPen(QColor("#5D4037"), 2)); p.drawLine(cx, c.y()-r-r/2, cx, c.y()-r-r); p.setPen(Qt::NoPen);
        QPainterPath leaf; int lx = cx, ly = c.y()-r-r/2; leaf.moveTo(lx, ly); leaf.cubicTo(lx+w/5, ly-h/6, lx+w/4, ly, lx+w/8, ly+h/8);
        p.setBrush(QColor("#66BB6A")); p.drawPath(leaf);
    } else if (type == "pepper") {
        QPainterPath pepper; int px = cell.left() + w/4, py = cell.top() + h/6;
        pepper.moveTo(px + w/2, py); pepper.cubicTo(px + w*3/4, py + h/4, px + w*2/3, py + h*2/3, px + w/3, py + h*3/4);
        pepper.cubicTo(px, py + h*5/6, px - w/8, py + h/2, px + w/6, py + h/6); pepper.cubicTo(px + w/4, py, px + w/3, py, px + w/2, py); pepper.closeSubpath();
        QLinearGradient pepGrad(cell.topLeft(), cell.bottomRight()); pepGrad.setColorAt(0, QColor("#FF6D00")); pepGrad.setColorAt(0.5, QColor("#FF3D00")); pepGrad.setColorAt(1, QColor("#DD2C00"));
        p.setBrush(pepGrad); p.drawPath(pepper);
        p.setPen(QPen(QColor("#33691E"), 2)); p.drawLine(px + w/2, py, px + w/2 + w/8, py - h/6); p.setPen(Qt::NoPen);
    } else if (type == "speed") {
        p.setBrush(QColor("#00E5FF")); p.drawEllipse(1, 1, 22, 22);
        p.setPen(QColor("#0A0E1A")); QFont f = p.font(); f.setPixelSize(12); f.setBold(true); p.setFont(f); p.drawText(cell, Qt::AlignCenter, "S");
    } else if (type == "invince") {
        p.setBrush(QColor("#B48EF0")); p.drawEllipse(1, 1, 22, 22);
        p.setPen(QColor("#0A0E1A")); QFont f = p.font(); f.setPixelSize(12); f.setBold(true); p.setFont(f); p.drawText(cell, Qt::AlignCenter, "I");
    } else if (type == "multi") {
        p.setBrush(QColor("#FFD700")); p.drawEllipse(1, 1, 22, 22);
        p.setPen(QColor("#0A0E1A")); QFont f = p.font(); f.setPixelSize(12); f.setBold(true); p.setFont(f); p.drawText(cell, Qt::AlignCenter, "×2");
    } else if (type == "obs") {
        QRect out(2, 2, 20, 20); QRect in(4, 4, 16, 16);
        p.setBrush(QColor("#2E7D32")); p.drawRoundedRect(out, 2, 2);
        p.setBrush(QColor("#1B5E20")); p.drawRoundedRect(in, 1, 1);
        p.setPen(QPen(QColor("#2E7D32").darker(130), 1)); p.drawLine(out.topLeft()+QPoint(2,2), out.bottomRight()-QPoint(2,2)); p.drawLine(out.topRight()+QPoint(-2,2), out.bottomLeft()+QPoint(2,-2));
    }
    
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    pix.save(&buffer, "PNG");
    return QString("data:image/png;base64,") + bytes.toBase64();
}