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
#include "InputHandler.h"

class Game {
public:
    // Expose enum for difficulty if needed
    enum Difficulty { EASY = 1, MEDIUM = 2, HARD = 3 };

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

    // ── Added for GUI Integration ──────────────────
    void resetGame();
    void tick();
    void processInputGUI(InputKey key);
    void setDifficulty(int diff);
    void setPaused(bool p) { paused = p; }

    const Snake& getSnake() const { return snake; }
    const Board& getBoard() const { return board; }
    const Food& getFood() const { return food; }
    int getScore() const { return score; }
    int getLevel() const { return currentLevel; }
    int getTickMs() const { return tickMs; }
    bool isGameOver() const { return gameOver; }
    bool isPaused() const { return paused; }
};