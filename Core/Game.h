#pragma once
#include <vector>   // explicit — needed for std::vector<PowerUp>
#include <stack>
#include <string>
#include <unordered_set>
#include "Snake.h"
#include "Board.h"
#include "Food.h"
#include "Leaderboard.h"
#include "PowerUp.h"
#include "Move.h"
#include "Position.h"

class Game {
private:
    // ── Core components ──────────────────────────
    Snake       snake;
    Board       board;
    Leaderboard leaderboard;
    Food        food;

    // ── Data structures ──────────────────────────
    std::vector<PowerUp> activePowerUps;   // vector so we can iterate + erase easily
    std::stack<Move>     moveHistory;      // Stack for undo feature

    // ── Direction (Snake::setDirection is commented out by Member 1) ──
    // We track direction here until Member 1 uncomments setDirection/getDirection
    Position currentDirection;

    // ── Game state ───────────────────────────────
    int  score;
    int  currentLevel;
    bool gameOver;
    bool paused;

    // ── Power-up state ───────────────────────────
    int  tickMs;
    int  scoreMultiplier;
    bool invincible;
    int  powerUpTicksLeft;

    // ── Internal flags ────────────────────────────
    bool lastMoveAteFood;
    int  lastEarnedPoints;   // exact points earned this tick (used by undo)

    // ── Private helpers ──────────────────────────
    void showMenu();
    void resetGame();
    void showGameOver();
    void  displayHUD() const;
    void tickPowerUps();
    void spawnPowerUp();     // ADDED: was missing — spawns a power-up on the board

public:
    Game();
    void start();
    void update();
    void handleInput();

    void spawnFood();
    void checkCollisions();
    void applyPowerUp(PowerUpType type);
    void undoLastMove();
    void saveCurrentScore(std::string playerName);
};