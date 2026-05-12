#pragma once
#include <vector>
#include <stack>
#include <string>
#include <unordered_set>
#include "Snake.h"
#include "Board.h"
#include "Food.h"
#include "Leaderboard.h"
#include "PowerUp.h"
#include "Position.h"
#include "InputHandler.h"

class Game {
public:
    enum Difficulty { EASY = 1, MEDIUM = 2, HARD = 3 };

private:
    Snake       snake;
    Board       board;
    Leaderboard leaderboard;
    Food        food;

    std::vector<PowerUp> activePowerUps;

    Position currentDirection;

    int  score;
    int  currentLevel;
    bool gameOver;
    bool paused;

    int  tickMs;
    int  scoreMultiplier;
    bool invincible;
    int  powerUpTicksLeft;

    bool lastMoveAteFood;
    int  lastEarnedPoints;

    bool wallWrap;
    int  currentDifficulty;
    int  foodEatenCount;

    void tickPowerUps();
    void spawnPowerUp();

public:
    Game();

    void spawnFood();
    void checkCollisions();
    void applyPowerUp(PowerUpType type);
    void saveCurrentScore(std::string playerName);

    void resetGame();
    void tick();
    void processInputGUI(InputKey key);
    void setDifficulty(int diff);
    void setPaused(bool p) { paused = p; }

    void setWallWrap(bool enabled) { wallWrap = enabled; }
    bool isWallWrap() const { return wallWrap; }

    const Snake& getSnake() const { return snake; }
    const Board& getBoard() const { return board; }
    const Food& getFood() const { return food; }
    const Leaderboard& getLeaderboard() const { return leaderboard; }
    int getScore() const { return score; }
    int getLevel() const { return currentLevel; }
    int getTickMs() const { return tickMs; }
    bool isGameOver() const { return gameOver; }
    bool isPaused() const { return paused; }

    const std::vector<PowerUp>& getActivePowerUps() const { return activePowerUps; }
    int  getDifficulty() const { return currentDifficulty; }
    bool isInvincible() const { return invincible; }
    int  getScoreMultiplier() const { return scoreMultiplier; }
    int  getPowerUpTicksLeft() const { return powerUpTicksLeft; }
};