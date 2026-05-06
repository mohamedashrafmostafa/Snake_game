/********************************************************************************
** Form generated from reading UI file 'Mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *centralLayout;
    QStackedWidget *stackedWidget;
    QWidget *pagMenu;
    QVBoxLayout *menuLayout;
    QLabel *lblTitle;
    QLabel *lblSubtitle;
    QSpacerItem *spacerItem;
    QHBoxLayout *hboxLayout;
    QLabel *lblName;
    QLineEdit *inputPlayerName;
    QHBoxLayout *hboxLayout1;
    QLabel *lblDifficulty;
    QComboBox *comboDifficulty;
    QHBoxLayout *hboxLayout2;
    QLabel *lblMode;
    QComboBox *comboMode;
    QHBoxLayout *hboxLayout3;
    QLabel *lblWrap;
    QCheckBox *chkWallWrap;
    QSpacerItem *spacerItem1;
    QSpacerItem *spacerItem2;
    QLabel *lblBoardSize;
    QHBoxLayout *hboxLayout4;
    QSpacerItem *spacerItem3;
    QPushButton *btnStart;
    QPushButton *btnLeaderboard;
    QPushButton *btnQuit;
    QSpacerItem *spacerItem4;
    QWidget *pagGame;
    QVBoxLayout *gameLayout;
    QFrame *frmHud;
    QHBoxLayout *hudLayout;
    QVBoxLayout *vboxLayout;
    QLabel *lblScoreCaption;
    QLabel *lblScoreValue;
    QVBoxLayout *vboxLayout1;
    QLabel *lblSpeedCaption;
    QLabel *lblSpeedValue;
    QVBoxLayout *vboxLayout2;
    QLabel *lblLengthCaption;
    QLabel *lblLengthValue;
    QVBoxLayout *vboxLayout3;
    QLabel *lblLevelCaption;
    QLabel *lblLevelValue;
    QSpacerItem *spacerItem5;
    QPushButton *btnPause;
    QPushButton *btnMenuFromGame;
    QWidget *gameCanvas;
    QFrame *frmControls;
    QHBoxLayout *hboxLayout5;
    QLabel *lblControls;
    QSpacerItem *spacerItem6;
    QLabel *lblTimer;
    QWidget *pagGameOver;
    QVBoxLayout *gameOverLayout;
    QLabel *lblGameOver;
    QFrame *frmStats;
    QHBoxLayout *hboxLayout6;
    QVBoxLayout *vboxLayout4;
    QLabel *lblFinalScoreCaption;
    QLabel *lblFinalScore;
    QVBoxLayout *vboxLayout5;
    QLabel *lblFinalLengthCaption;
    QLabel *lblFinalLength;
    QVBoxLayout *vboxLayout6;
    QLabel *lblFinalLevelCaption;
    QLabel *lblFinalLevel;
    QVBoxLayout *vboxLayout7;
    QLabel *lblNewRecordCaption;
    QLabel *lblNewRecord;
    QTableWidget *tableLeaderboard;
    QLabel *lblLeaderboardTitle;
    QHBoxLayout *hboxLayout7;
    QSpacerItem *spacerItem7;
    QPushButton *btnPlayAgain;
    QPushButton *btnBackToMenu;
    QSpacerItem *spacerItem8;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(766, 600);
        MainWindow->setMinimumSize(QSize(700, 600));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralLayout = new QVBoxLayout(centralwidget);
        centralLayout->setSpacing(0);
        centralLayout->setContentsMargins(0, 0, 0, 0);
        centralLayout->setObjectName("centralLayout");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        pagMenu = new QWidget();
        pagMenu->setObjectName("pagMenu");
        menuLayout = new QVBoxLayout(pagMenu);
        menuLayout->setSpacing(18);
        menuLayout->setObjectName("menuLayout");
        lblTitle = new QLabel(pagMenu);
        lblTitle->setObjectName("lblTitle");
        lblTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);

        menuLayout->addWidget(lblTitle);

        lblSubtitle = new QLabel(pagMenu);
        lblSubtitle->setObjectName("lblSubtitle");
        lblSubtitle->setAlignment(Qt::AlignmentFlag::AlignCenter);

        menuLayout->addWidget(lblSubtitle);

        spacerItem = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        menuLayout->addItem(spacerItem);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName("hboxLayout");
        lblName = new QLabel(pagMenu);
        lblName->setObjectName("lblName");

        hboxLayout->addWidget(lblName);

        inputPlayerName = new QLineEdit(pagMenu);
        inputPlayerName->setObjectName("inputPlayerName");
        inputPlayerName->setMaxLength(20);

        hboxLayout->addWidget(inputPlayerName);


        menuLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setObjectName("hboxLayout1");
        lblDifficulty = new QLabel(pagMenu);
        lblDifficulty->setObjectName("lblDifficulty");

        hboxLayout1->addWidget(lblDifficulty);

        comboDifficulty = new QComboBox(pagMenu);
        comboDifficulty->addItem(QString());
        comboDifficulty->addItem(QString());
        comboDifficulty->addItem(QString());
        comboDifficulty->addItem(QString());
        comboDifficulty->setObjectName("comboDifficulty");

        hboxLayout1->addWidget(comboDifficulty);


        menuLayout->addLayout(hboxLayout1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setObjectName("hboxLayout2");
        lblMode = new QLabel(pagMenu);
        lblMode->setObjectName("lblMode");

        hboxLayout2->addWidget(lblMode);

        comboMode = new QComboBox(pagMenu);
        comboMode->addItem(QString());
        comboMode->addItem(QString());
        comboMode->addItem(QString());
        comboMode->setObjectName("comboMode");

        hboxLayout2->addWidget(comboMode);


        menuLayout->addLayout(hboxLayout2);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setObjectName("hboxLayout3");
        lblWrap = new QLabel(pagMenu);
        lblWrap->setObjectName("lblWrap");

        hboxLayout3->addWidget(lblWrap);

        chkWallWrap = new QCheckBox(pagMenu);
        chkWallWrap->setObjectName("chkWallWrap");

        hboxLayout3->addWidget(chkWallWrap);

        spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout3->addItem(spacerItem1);


        menuLayout->addLayout(hboxLayout3);

        spacerItem2 = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        menuLayout->addItem(spacerItem2);

        lblBoardSize = new QLabel(pagMenu);
        lblBoardSize->setObjectName("lblBoardSize");

        menuLayout->addWidget(lblBoardSize);

        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setObjectName("hboxLayout4");
        spacerItem3 = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout4->addItem(spacerItem3);

        btnStart = new QPushButton(pagMenu);
        btnStart->setObjectName("btnStart");
        btnStart->setMinimumSize(QSize(160, 48));

        hboxLayout4->addWidget(btnStart);

        btnLeaderboard = new QPushButton(pagMenu);
        btnLeaderboard->setObjectName("btnLeaderboard");
        btnLeaderboard->setMinimumSize(QSize(160, 48));

        hboxLayout4->addWidget(btnLeaderboard);

        btnQuit = new QPushButton(pagMenu);
        btnQuit->setObjectName("btnQuit");
        btnQuit->setMinimumSize(QSize(100, 48));

        hboxLayout4->addWidget(btnQuit);

        spacerItem4 = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout4->addItem(spacerItem4);


        menuLayout->addLayout(hboxLayout4);

        stackedWidget->addWidget(pagMenu);
        pagGame = new QWidget();
        pagGame->setObjectName("pagGame");
        gameLayout = new QVBoxLayout(pagGame);
        gameLayout->setSpacing(0);
        gameLayout->setObjectName("gameLayout");
        frmHud = new QFrame(pagGame);
        frmHud->setObjectName("frmHud");
        frmHud->setFrameShape(QFrame::Shape::StyledPanel);
        hudLayout = new QHBoxLayout(frmHud);
        hudLayout->setSpacing(24);
        hudLayout->setObjectName("hudLayout");
        vboxLayout = new QVBoxLayout();
        vboxLayout->setObjectName("vboxLayout");
        lblScoreCaption = new QLabel(frmHud);
        lblScoreCaption->setObjectName("lblScoreCaption");

        vboxLayout->addWidget(lblScoreCaption);

        lblScoreValue = new QLabel(frmHud);
        lblScoreValue->setObjectName("lblScoreValue");

        vboxLayout->addWidget(lblScoreValue);


        hudLayout->addLayout(vboxLayout);

        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setObjectName("vboxLayout1");
        lblSpeedCaption = new QLabel(frmHud);
        lblSpeedCaption->setObjectName("lblSpeedCaption");

        vboxLayout1->addWidget(lblSpeedCaption);

        lblSpeedValue = new QLabel(frmHud);
        lblSpeedValue->setObjectName("lblSpeedValue");

        vboxLayout1->addWidget(lblSpeedValue);


        hudLayout->addLayout(vboxLayout1);

        vboxLayout2 = new QVBoxLayout();
        vboxLayout2->setSpacing(10);
        vboxLayout2->setObjectName("vboxLayout2");
        lblLengthCaption = new QLabel(frmHud);
        lblLengthCaption->setObjectName("lblLengthCaption");

        vboxLayout2->addWidget(lblLengthCaption);

        lblLengthValue = new QLabel(frmHud);
        lblLengthValue->setObjectName("lblLengthValue");

        vboxLayout2->addWidget(lblLengthValue);


        hudLayout->addLayout(vboxLayout2);

        vboxLayout3 = new QVBoxLayout();
        vboxLayout3->setObjectName("vboxLayout3");
        lblLevelCaption = new QLabel(frmHud);
        lblLevelCaption->setObjectName("lblLevelCaption");

        vboxLayout3->addWidget(lblLevelCaption);

        lblLevelValue = new QLabel(frmHud);
        lblLevelValue->setObjectName("lblLevelValue");

        vboxLayout3->addWidget(lblLevelValue);


        hudLayout->addLayout(vboxLayout3);

        spacerItem5 = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hudLayout->addItem(spacerItem5);

        btnPause = new QPushButton(frmHud);
        btnPause->setObjectName("btnPause");
        btnPause->setMinimumSize(QSize(90, 34));

        hudLayout->addWidget(btnPause);

        btnMenuFromGame = new QPushButton(frmHud);
        btnMenuFromGame->setObjectName("btnMenuFromGame");
        btnMenuFromGame->setMinimumSize(QSize(90, 34));

        hudLayout->addWidget(btnMenuFromGame);


        gameLayout->addWidget(frmHud);

        gameCanvas = new QWidget(pagGame);
        gameCanvas->setObjectName("gameCanvas");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(gameCanvas->sizePolicy().hasHeightForWidth());
        gameCanvas->setSizePolicy(sizePolicy);

        gameLayout->addWidget(gameCanvas);

        frmControls = new QFrame(pagGame);
        frmControls->setObjectName("frmControls");
        frmControls->setFrameShape(QFrame::Shape::StyledPanel);
        hboxLayout5 = new QHBoxLayout(frmControls);
        hboxLayout5->setObjectName("hboxLayout5");
        lblControls = new QLabel(frmControls);
        lblControls->setObjectName("lblControls");

        hboxLayout5->addWidget(lblControls);

        spacerItem6 = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout5->addItem(spacerItem6);

        lblTimer = new QLabel(frmControls);
        lblTimer->setObjectName("lblTimer");

        hboxLayout5->addWidget(lblTimer);


        gameLayout->addWidget(frmControls);

        stackedWidget->addWidget(pagGame);
        pagGameOver = new QWidget();
        pagGameOver->setObjectName("pagGameOver");
        gameOverLayout = new QVBoxLayout(pagGameOver);
        gameOverLayout->setSpacing(16);
        gameOverLayout->setObjectName("gameOverLayout");
        lblGameOver = new QLabel(pagGameOver);
        lblGameOver->setObjectName("lblGameOver");
        lblGameOver->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gameOverLayout->addWidget(lblGameOver);

        frmStats = new QFrame(pagGameOver);
        frmStats->setObjectName("frmStats");
        frmStats->setFrameShape(QFrame::Shape::StyledPanel);
        hboxLayout6 = new QHBoxLayout(frmStats);
        hboxLayout6->setSpacing(32);
        hboxLayout6->setObjectName("hboxLayout6");
        vboxLayout4 = new QVBoxLayout();
        vboxLayout4->setObjectName("vboxLayout4");
        lblFinalScoreCaption = new QLabel(frmStats);
        lblFinalScoreCaption->setObjectName("lblFinalScoreCaption");
        lblFinalScoreCaption->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vboxLayout4->addWidget(lblFinalScoreCaption);

        lblFinalScore = new QLabel(frmStats);
        lblFinalScore->setObjectName("lblFinalScore");
        lblFinalScore->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vboxLayout4->addWidget(lblFinalScore);


        hboxLayout6->addLayout(vboxLayout4);

        vboxLayout5 = new QVBoxLayout();
        vboxLayout5->setObjectName("vboxLayout5");
        lblFinalLengthCaption = new QLabel(frmStats);
        lblFinalLengthCaption->setObjectName("lblFinalLengthCaption");
        lblFinalLengthCaption->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vboxLayout5->addWidget(lblFinalLengthCaption);

        lblFinalLength = new QLabel(frmStats);
        lblFinalLength->setObjectName("lblFinalLength");
        lblFinalLength->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vboxLayout5->addWidget(lblFinalLength);


        hboxLayout6->addLayout(vboxLayout5);

        vboxLayout6 = new QVBoxLayout();
        vboxLayout6->setObjectName("vboxLayout6");
        lblFinalLevelCaption = new QLabel(frmStats);
        lblFinalLevelCaption->setObjectName("lblFinalLevelCaption");
        lblFinalLevelCaption->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vboxLayout6->addWidget(lblFinalLevelCaption);

        lblFinalLevel = new QLabel(frmStats);
        lblFinalLevel->setObjectName("lblFinalLevel");
        lblFinalLevel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vboxLayout6->addWidget(lblFinalLevel);


        hboxLayout6->addLayout(vboxLayout6);

        vboxLayout7 = new QVBoxLayout();
        vboxLayout7->setObjectName("vboxLayout7");
        lblNewRecordCaption = new QLabel(frmStats);
        lblNewRecordCaption->setObjectName("lblNewRecordCaption");
        lblNewRecordCaption->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vboxLayout7->addWidget(lblNewRecordCaption);

        lblNewRecord = new QLabel(frmStats);
        lblNewRecord->setObjectName("lblNewRecord");
        lblNewRecord->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vboxLayout7->addWidget(lblNewRecord);


        hboxLayout6->addLayout(vboxLayout7);


        gameOverLayout->addWidget(frmStats);

        tableLeaderboard = new QTableWidget(pagGameOver);
        if (tableLeaderboard->columnCount() < 4)
            tableLeaderboard->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableLeaderboard->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableLeaderboard->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableLeaderboard->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableLeaderboard->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        tableLeaderboard->setObjectName("tableLeaderboard");
        tableLeaderboard->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableLeaderboard->setAlternatingRowColors(true);
        tableLeaderboard->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);

        gameOverLayout->addWidget(tableLeaderboard);

        lblLeaderboardTitle = new QLabel(pagGameOver);
        lblLeaderboardTitle->setObjectName("lblLeaderboardTitle");
        lblLeaderboardTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gameOverLayout->addWidget(lblLeaderboardTitle);

        hboxLayout7 = new QHBoxLayout();
        hboxLayout7->setObjectName("hboxLayout7");
        spacerItem7 = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout7->addItem(spacerItem7);

        btnPlayAgain = new QPushButton(pagGameOver);
        btnPlayAgain->setObjectName("btnPlayAgain");
        btnPlayAgain->setMinimumSize(QSize(150, 48));

        hboxLayout7->addWidget(btnPlayAgain);

        btnBackToMenu = new QPushButton(pagGameOver);
        btnBackToMenu->setObjectName("btnBackToMenu");
        btnBackToMenu->setMinimumSize(QSize(150, 48));

        hboxLayout7->addWidget(btnBackToMenu);

        spacerItem8 = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout7->addItem(spacerItem8);


        gameOverLayout->addLayout(hboxLayout7);

        stackedWidget->addWidget(pagGameOver);

        centralLayout->addWidget(stackedWidget);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Snake++", nullptr));
        lblTitle->setText(QCoreApplication::translate("MainWindow", "\360\237\220\215 Snake Game", nullptr));
        lblSubtitle->setText(QString());
        lblName->setText(QCoreApplication::translate("MainWindow", "Player Name", nullptr));
        inputPlayerName->setPlaceholderText(QCoreApplication::translate("MainWindow", "Enter your name...", nullptr));
        lblDifficulty->setText(QCoreApplication::translate("MainWindow", "Difficulty", nullptr));
        comboDifficulty->setItemText(0, QCoreApplication::translate("MainWindow", "Easy \342\200\224 Slow speed, no obstacles", nullptr));
        comboDifficulty->setItemText(1, QCoreApplication::translate("MainWindow", "Medium \342\200\224 Moderate speed, some obstacles", nullptr));
        comboDifficulty->setItemText(2, QCoreApplication::translate("MainWindow", "Hard \342\200\224 Fast speed, many obstacles", nullptr));
        comboDifficulty->setItemText(3, QCoreApplication::translate("MainWindow", "Insane \342\200\224 Maximum speed, maze obstacles", nullptr));

        lblMode->setText(QCoreApplication::translate("MainWindow", "Game Mode", nullptr));
        comboMode->setItemText(0, QCoreApplication::translate("MainWindow", "Classic \342\200\224 Eat food, grow, survive", nullptr));
        comboMode->setItemText(1, QCoreApplication::translate("MainWindow", "Time Attack \342\200\224 Score as much as possible in 60s", nullptr));
        comboMode->setItemText(2, QCoreApplication::translate("MainWindow", "Level Rush \342\200\224 Clear levels as fast as possible", nullptr));

        lblWrap->setText(QCoreApplication::translate("MainWindow", "Wall Wrap", nullptr));
        chkWallWrap->setText(QCoreApplication::translate("MainWindow", "Enable (pass through walls)", nullptr));
        lblBoardSize->setText(QString());
        btnStart->setText(QCoreApplication::translate("MainWindow", " Start Game", nullptr));
        btnLeaderboard->setText(QCoreApplication::translate("MainWindow", "Leaderboard", nullptr));
        btnQuit->setText(QCoreApplication::translate("MainWindow", " Quit", nullptr));
        lblScoreCaption->setText(QCoreApplication::translate("MainWindow", "SCORE", nullptr));
        lblScoreValue->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblSpeedCaption->setText(QCoreApplication::translate("MainWindow", "SPEED", nullptr));
        lblSpeedValue->setText(QCoreApplication::translate("MainWindow", "Slow", nullptr));
        lblLengthCaption->setText(QCoreApplication::translate("MainWindow", "LENGTH", nullptr));
        lblLengthValue->setText(QCoreApplication::translate("MainWindow", "4", nullptr));
        lblLevelCaption->setText(QCoreApplication::translate("MainWindow", "LEVEL", nullptr));
        lblLevelValue->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        btnPause->setText(QCoreApplication::translate("MainWindow", " Pause", nullptr));
        btnMenuFromGame->setText(QCoreApplication::translate("MainWindow", " Menu", nullptr));
        lblControls->setText(QCoreApplication::translate("MainWindow", "Controls:  \342\206\221 \342\206\223 \342\206\220 \342\206\222  move  |  P  pause  |  ESC  menu", nullptr));
        lblTimer->setText(QCoreApplication::translate("MainWindow", "Time: 0:00", nullptr));
        lblGameOver->setText(QCoreApplication::translate("MainWindow", "GAME OVER", nullptr));
        lblFinalScoreCaption->setText(QCoreApplication::translate("MainWindow", "Final Score", nullptr));
        lblFinalScore->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblFinalLengthCaption->setText(QCoreApplication::translate("MainWindow", "Length", nullptr));
        lblFinalLength->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblFinalLevelCaption->setText(QCoreApplication::translate("MainWindow", "Level Reached", nullptr));
        lblFinalLevel->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblNewRecordCaption->setText(QCoreApplication::translate("MainWindow", "New Record", nullptr));
        lblNewRecord->setText(QCoreApplication::translate("MainWindow", "\342\200\224", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableLeaderboard->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Rank", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableLeaderboard->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableLeaderboard->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Score", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableLeaderboard->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Length", nullptr));
        lblLeaderboardTitle->setText(QCoreApplication::translate("MainWindow", "  Top 10 Leaderboard", nullptr));
        btnPlayAgain->setText(QCoreApplication::translate("MainWindow", "\342\226\266  Play Again", nullptr));
        btnBackToMenu->setText(QCoreApplication::translate("MainWindow", "\342\254\205  Main Menu", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
