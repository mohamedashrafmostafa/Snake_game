#include "Game.h"
#include "InputHandler.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

// ─────────────────────────────────────────────
//  Constants
// ─────────────────────────────────────────────
static const int BOARD_WIDTH            = 20;
static const int BOARD_HEIGHT           = 20;
static const int BASE_TICK_MS           = 150;
static const int FAST_TICK_MS           = 80;
static const int SCORE_MULTIPLIER_VAL   = 2;
static const int POWERUP_DURATION_TICKS = 30;
static const int POWERUP_SPAWN_INTERVAL = 5;   // spawn a power-up every N foods eaten

// ─────────────────────────────────────────────
//  Constructor
// ─────────────────────────────────────────────
Game::Game()
    : snake(),
      board(BOARD_WIDTH, BOARD_HEIGHT),
      food({0, 0}, NORMAL),
      currentDirection({1, 0}),
      score(0),
      currentLevel(1),
      gameOver(false),
      paused(false),
      tickMs(BASE_TICK_MS),
      scoreMultiplier(1),
      invincible(false),
      powerUpTicksLeft(0),
      lastMoveAteFood(false),
      lastEarnedPoints(0)
{
    srand(static_cast<unsigned int>(time(nullptr)));
}

// ─────────────────────────────────────────────
//  start()
// ─────────────────────────────────────────────
void Game::start() {
    leaderboard.loadFromFile("leaderboard.txt");
    showMenu();
}

// ─────────────────────────────────────────────
//  showMenu()
// ─────────────────────────────────────────────
void Game::showMenu() {
    while (true) {
        system("cls");
        std::cout << "================================\n";
        std::cout << "         SNAKE GAME             \n";
        std::cout << "================================\n";
        std::cout << "  1. Start Game\n";
        std::cout << "  2. View Leaderboard\n";
        std::cout << "  3. Quit\n";
        std::cout << "================================\n";
        std::cout << "Choose: ";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            resetGame();
            update();
        } else if (choice == 2) {
            system("cls");
            leaderboard.displayTop10();
            std::cout << "\nPress Enter to go back...";
            std::cin.ignore();
            std::cin.get();
        } else if (choice == 3) {
            break;
        }
    }
}

// ─────────────────────────────────────────────
//  resetGame()
// ─────────────────────────────────────────────
void Game::resetGame() {
    snake            = Snake();
    board            = Board(BOARD_WIDTH, BOARD_HEIGHT);
    currentDirection = {1, 0};

    score            = 0;
    currentLevel     = 1;
    gameOver         = false;
    paused           = false;
    tickMs           = BASE_TICK_MS;
    scoreMultiplier  = 1;
    invincible       = false;
    powerUpTicksLeft = 0;
    lastMoveAteFood  = false;
    lastEarnedPoints = 0;

    activePowerUps.clear();
    while (!moveHistory.empty()) moveHistory.pop();

    spawnFood();

}

// ─────────────────────────────────────────────
//  update() — main game loop
// ─────────────────────────────────────────────
void Game::update() {
    int foodEatenCount = 0;  // tracks when to spawn a power-up

    while (!gameOver) {
        auto tickStart = std::chrono::steady_clock::now();

        handleInput();

        if (!paused) {
            // Save state BEFORE moving (for undo)
            Move currentMove;
            currentMove.previousTail      = snake.getBody().back();
            currentMove.previousDirection = currentDirection;
            currentMove.ateFood           = false;
            currentMove.earnedPoints      = 0;

            // Self-collision check BEFORE move() because Member 1's
            // checkSelfCollision() looks ahead at where the snake WILL move
            if (snake.checkSelfCollision()) {
                gameOver = true;
                showGameOver();
                return;
            }

            snake.move();

            checkCollisions();

            currentMove.ateFood      = lastMoveAteFood;
            currentMove.earnedPoints = lastEarnedPoints;
            lastEarnedPoints         = 0;  // reset for next tick
            moveHistory.push(currentMove);

            // Spawn a power-up every POWERUP_SPAWN_INTERVAL foods
            if (lastMoveAteFood) {
                foodEatenCount++;
                if (foodEatenCount % POWERUP_SPAWN_INTERVAL == 0)
                    spawnPowerUp();
            }

            tickPowerUps();

            board.render(snake, food);
            displayHUD();
        }

        // Sleep for the remainder of the tick
        auto tickEnd  = std::chrono::steady_clock::now();
        int  elapsed  = static_cast<int>(
            std::chrono::duration_cast<std::chrono::milliseconds>(tickEnd - tickStart).count()
        );
        int remaining = tickMs - elapsed;
        if (remaining > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(remaining));
    }

    showGameOver();
}

// ─────────────────────────────────────────────
//  handleInput()
//  NOTE: Snake::setDirection() is commented out in Snake.h by Member 1.
//  We track direction in currentDirection and call snake.setDirection()
//  once Member 1 uncomments it. Lines are ready — just uncomment them.
// ─────────────────────────────────────────────
void Game::handleInput() {
    InputKey key = InputHandler::getKeyPress();

    switch (key) {
        case InputKey::UP:
            if (currentDirection.y != 1) {
                currentDirection = {0, -1};
                // snake.setDirection({0, -1});  // uncomment when Member 1 adds it
            }
            break;

        case InputKey::DOWN:
            if (currentDirection.y != -1) {
                currentDirection = {0, 1};
                // snake.setDirection({0, 1});
            }
            break;

        case InputKey::LEFT:
            if (currentDirection.x != 1) {
                currentDirection = {-1, 0};
                // snake.setDirection({-1, 0});
            }
            break;

        case InputKey::RIGHT:
            if (currentDirection.x != -1) {
                currentDirection = {1, 0};
                // snake.setDirection({1, 0});
            }
            break;

        case InputKey::PAUSE:
            paused = !paused;
            if (paused) {
                system("cls");
                std::cout << "\n  *** PAUSED - press P to resume ***\n";
            }
            break;

        case InputKey::UNDO:
            undoLastMove();
            break;

        case InputKey::QUIT:
            gameOver = true;
            break;

        default:
            break;
    }
}

// ─────────────────────────────────────────────
//  spawnFood()
//  Key data structure: unordered_set for O(1) occupied-cell lookup
// ─────────────────────────────────────────────
void Game::spawnFood() {
    // Collect all occupied positions (snake body + active power-up positions)
    std::unordered_set<Position, PositionHash> occupied;
    for (const Position& p : snake.getBody())
        occupied.insert(p);
    for (const PowerUp& pu : activePowerUps)
        occupied.insert(pu.pos);

    // Weighted random food type
    int roll = rand() % 10;
    FoodType type;
    if      (roll < 7) type = NORMAL;
    else if (roll < 9) type = BONUS;
    else               type = FoodType::SPEED_UP;

    // Find a random empty cell
    Position candidate = {0, 0};
    do {
        candidate.x = rand() % BOARD_WIDTH;
        candidate.y = rand() % BOARD_HEIGHT;
    } while (occupied.count(candidate) > 0);

    food = Food(candidate, type);
}

// ─────────────────────────────────────────────
//  spawnPowerUp()
//  ADDED: was completely missing — this is what puts power-ups on the board.
//  Called every POWERUP_SPAWN_INTERVAL foods eaten.
// ─────────────────────────────────────────────
void Game::spawnPowerUp() {
    // Collect all occupied positions so we don't spawn on top of anything
    std::unordered_set<Position, PositionHash> occupied;
    for (const Position& p : snake.getBody())
        occupied.insert(p);
    occupied.insert(food.getPosition());
    for (const PowerUp& pu : activePowerUps)
        occupied.insert(pu.pos);

    // Pick a random PowerUpType
    PowerUpType types[3] = { PowerUpType::P_SPEED_UP, PowerUpType::INVINCIBILITY, PowerUpType::SCORE_MULTIPLIER };
    PowerUpType chosenType = types[rand() % 3];

    // Find a random empty cell
    Position candidate = {0, 0};
    do {
        candidate.x = rand() % BOARD_WIDTH;
        candidate.y = rand() % BOARD_HEIGHT;
    } while (occupied.count(candidate) > 0);

    PowerUp pu;
    pu.pos            = candidate;
    pu.type           = chosenType;
    pu.expirationTime = 0;  // not used for spawned power-ups (we erase on pickup)

    activePowerUps.push_back(pu);
}

// ─────────────────────────────────────────────
//  checkCollisions()
// ─────────────────────────────────────────────
void Game::checkCollisions() {
    lastMoveAteFood = false;
    Position head   = snake.getHead();

    // 1. Wall collision
    if (!invincible && !board.isInsideBoard(head)) {
        gameOver = true;
        return;
    }

    // 2. Obstacle collision
    if (!invincible && board.isObstacle(head)) {
        gameOver = true;
        return;
    }

    // 3. Food collision
    if (head == food.getPosition()) {
        int earned      = food.getPoints() * scoreMultiplier;
        score          += earned;
        lastMoveAteFood = true;
        lastEarnedPoints = earned;  // save exact value for undo

        if (food.getType() == FoodType::SPEED_UP) {
            tickMs           = FAST_TICK_MS;
            powerUpTicksLeft = POWERUP_DURATION_TICKS;
        }

        snake.grow();
        spawnFood();

        currentLevel = (score / 50) + 1;
        return;
    }

    // 4. Power-up collision — iterate vector, erase on hit
    for (auto it = activePowerUps.begin(); it != activePowerUps.end(); ) {
        if (head == it->pos) {
            applyPowerUp(it->type);
            it = activePowerUps.erase(it);  // erase returns next valid iterator
            break;                          // only one power-up eaten per tick
        } else {
            ++it;
        }
    }
}

// ─────────────────────────────────────────────
//  applyPowerUp()
// ─────────────────────────────────────────────
void Game::applyPowerUp(PowerUpType type) {
    powerUpTicksLeft = POWERUP_DURATION_TICKS;

    switch (type) {
        case INVINCIBILITY:
            invincible = true;
            break;
        case SCORE_MULTIPLIER:
            scoreMultiplier = SCORE_MULTIPLIER_VAL;
            break;
        default:  // SPEED_UP
            tickMs = FAST_TICK_MS;
            break;
    }
}

// ─────────────────────────────────────────────
//  tickPowerUps()
// ─────────────────────────────────────────────
void Game::tickPowerUps() {
    if (powerUpTicksLeft <= 0) return;
    powerUpTicksLeft--;
    if (powerUpTicksLeft == 0) {
        tickMs          = BASE_TICK_MS;
        invincible      = false;
        scoreMultiplier = 1;
    }
}

// ─────────────────────────────────────────────
//  undoLastMove()
//  Direction + score rollback done here.
//  Body rollback needs Snake::undoMove(Position) from Member 1.
// ─────────────────────────────────────────────
void Game::undoLastMove() {
    if (moveHistory.empty()) return;

    Move last = moveHistory.top();
    moveHistory.pop();

    currentDirection = last.previousDirection;
    // snake.setDirection(last.previousDirection);  // uncomment when Member 1 adds it
    // snake.undoMove(last.previousTail);           // uncomment when Member 1 adds it

    if (last.ateFood) {
        score -= last.earnedPoints;  // subtract exact points earned that tick
        if (score < 0) score = 0;
    }
}

// ─────────────────────────────────────────────
//  saveCurrentScore()
// ─────────────────────────────────────────────
void Game::saveCurrentScore(std::string playerName) {
    leaderboard.addScore(playerName, score);
    leaderboard.saveToFile("leaderboard.txt");
}

// ─────────────────────────────────────────────
//  showGameOver()
// ─────────────────────────────────────────────
void Game::showGameOver() {
    system("cls");
    std::cout << "================================\n";
    std::cout << "          GAME OVER             \n";
    std::cout << "================================\n";
    std::cout << "  Final Score : " << score        << "\n";
    std::cout << "  Level       : " << currentLevel << "\n";
    std::cout << "================================\n";
    std::cout << "  Enter your name: ";

    std::string name;
    std::cin >> name;
    saveCurrentScore(name);

    std::cout << "\n  Score saved! Top 10:\n\n";
    leaderboard.displayTop10();

    std::cout << "\n  Press Enter to return to menu...";
    std::cin.ignore();
    std::cin.get();

    showMenu();
}

// ─────────────────────────────────────────────
//  displayHUD()
// ─────────────────────────────────────────────
void Game::displayHUD() const {
    std::cout << "  Score: " << score
              << "   Level: " << currentLevel
              << "   [P]ause  [Z]undo  [Q]uit\n";

    if (invincible)
        std::cout << "  *** INVINCIBLE (" << powerUpTicksLeft << " ticks left) ***\n";
    else if (scoreMultiplier > 1)
        std::cout << "  *** x" << scoreMultiplier << " SCORE (" << powerUpTicksLeft << " ticks left) ***\n";
    else if (tickMs == FAST_TICK_MS)
        std::cout << "  *** SPEED UP (" << powerUpTicksLeft << " ticks left) ***\n";
}