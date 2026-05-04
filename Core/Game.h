#pragma once
#include <queue>
#include <stack>
#include <string>
#include "Snake.h"
#include "Board.h"
#include "Leaderboard.h"
#include "PowerUp.h"
#include "Move.h"

class Game {
private:
    Snake snake;
    Board board;
    Leaderboard leaderboard;
    
    Position food; // Food system
    std::priority_queue<PowerUp> activePowerUps; // Power-ups management
    std::stack<Move> moveHistory; // Move history for undo feature

    int score;
    int currentLevel;
    bool gameOver;

public:
    Game();
    void start();                    // Start the game and show menu
    void update();                   // Main game loop
    void handleInput();              // Handle player input
    
    // Functions you will implement in the .cpp file
    void spawnFood();                // Spawn food in an empty spot using Hash Set
    void checkCollisions();          // Check all types of collisions
    void applyPowerUp(PowerUpType type);
    void undoLastMove();             // Undo feature using Stack
    void saveCurrentScore(std::string playerName);
};