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
      lastEarnedPoints(0),
      wallWrap(false),
      currentDifficulty(MEDIUM),
      foodEatenCount(0)
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
// ─────────────────────────────────────────────
//  clearConsole()
// ─────────────────────────────────────────────
void Game::clearConsole() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// ─────────────────────────────────────────────
//  showMenu()
// ─────────────────────────────────────────────
void Game::showMenu() {
    while (true) {
        clearConsole();
        std::cout << "========================================\n";
        std::cout << "              SNAKE++\n";
        std::cout << "========================================\n";
        std::cout << "  1. Play Game\n";
        std::cout << "  2. View Leaderboard\n";
        std::cout << "  3. Settings / Difficulty\n";
        std::cout << "  4. Instructions\n";
        std::cout << "  5. Exit\n";
        std::cout << "========================================\n";
        std::cout << "Choose: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::string discard;
            std::cin >> discard;
            continue;
        }

        if (choice == 1) {
            resetGame();
            update();
        } else if (choice == 2) {
            clearConsole();
            leaderboard.displayTop10();
            std::cout << "\nPress Enter to return to menu...";
            std::cin.ignore(10000, '\n');
            std::cin.get();
        } else if (choice == 3) {
            clearConsole();
            std::cout << "Select Difficulty:\n1. Easy\n2. Medium\n3. Hard\nChoice: ";
            int diff;
            if (std::cin >> diff && diff >= 1 && diff <= 3) {
                setDifficulty(diff);
            } else {
                std::cin.clear();
                std::string discard;
                std::cin >> discard;
            }
        } else if (choice == 4) {
            clearConsole();
            std::cout << "Instructions:\nWASD to move, P to pause, Q to quit.\nCollect food, avoid walls and obstacles.\n\nPress Enter to return to menu...";
            std::cin.ignore(10000, '\n');
            std::cin.get();
        } else if (choice == 5) {
            break;
        }
    }
}

// ─────────────────────────────────────────────
//  resetGame()
// ─────────────────────────────────────────────
void Game::resetGame() {
    snake            = Snake();
    currentDirection = {1, 0};

    score            = 0;
    currentLevel     = 1;
    gameOver         = false;
    paused           = false;
    // tickMs is managed by setDifficulty
    scoreMultiplier  = 1;
    invincible       = false;
    powerUpTicksLeft = 0;
    lastMoveAteFood  = false;
    lastEarnedPoints = 0;
    foodEatenCount   = 0;       // FIX: was static in tick(), now properly reset

    // NOTE: wallWrap and currentDifficulty are preserved — set before resetGame()

    activePowerUps.clear();
    while (!moveHistory.empty()) moveHistory.pop();

    // Ensure obstacles are placed based on currentDifficulty before spawning food
    setDifficulty(currentDifficulty);

    spawnFood();
}

// ─────────────────────────────────────────────
//  update() — main game loop
// ─────────────────────────────────────────────
void Game::update() {
    while (!gameOver) {
        auto tickStart = std::chrono::steady_clock::now();

        handleInput();

        tick(); // Non-blocking game step

        if (!paused && !gameOver) {
            clearConsole();
            drawConsoleScreen();
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
//  drawConsoleScreen()
// ─────────────────────────────────────────────
void Game::drawConsoleScreen() const {
    std::cout << "========================================\n";
    std::cout << "              SNAKE++\n";
    std::cout << "========================================\n";
    std::cout << "Score: " << score << "    Level: " << currentLevel << "    Length: " << snake.getBody().size() << "\n";
    
    std::string puStr = "None";
    if (invincible) puStr = "Shield (" + std::to_string(powerUpTicksLeft) + ")";
    else if (scoreMultiplier > 1) puStr = "x2 Score (" + std::to_string(powerUpTicksLeft) + ")";
    else if (tickMs == FAST_TICK_MS) puStr = "Speed (" + std::to_string(powerUpTicksLeft) + ")";
    std::cout << "Speed: " << tickMs << "ms    Power-Up: " << puStr << "\n";
    std::cout << "----------------------------------------\n";

    // Build the grid
    std::vector<std::string> grid(BOARD_HEIGHT, std::string(BOARD_WIDTH, ' '));
    
    // Draw obstacles
    for (const Position& obs : board.getObstacles()) {
        if (obs.x >= 0 && obs.x < BOARD_WIDTH && obs.y >= 0 && obs.y < BOARD_HEIGHT)
            grid[obs.y][obs.x] = '#';
    }

    // Draw active powerups
    for (const PowerUp& pu : activePowerUps) {
        if (pu.pos.x >= 0 && pu.pos.x < BOARD_WIDTH && pu.pos.y >= 0 && pu.pos.y < BOARD_HEIGHT) {
            if (pu.type == P_SPEED_UP) grid[pu.pos.y][pu.pos.x] = 'S';
            else if (pu.type == INVINCIBILITY) grid[pu.pos.y][pu.pos.x] = 'I';
            else if (pu.type == SCORE_MULTIPLIER) grid[pu.pos.y][pu.pos.x] = 'M';
        }
    }

    // Draw food
    Position fPos = food.getPosition();
    if (fPos.x >= 0 && fPos.x < BOARD_WIDTH && fPos.y >= 0 && fPos.y < BOARD_HEIGHT) {
        if (food.getType() == NORMAL) grid[fPos.y][fPos.x] = '@';
        else if (food.getType() == BONUS) grid[fPos.y][fPos.x] = '*';
        else grid[fPos.y][fPos.x] = '&';
    }

    // Draw snake body
    const auto& body = snake.getBody();
    for (size_t i = 0; i < body.size(); ++i) {
        Position p = body[i];
        if (p.x >= 0 && p.x < BOARD_WIDTH && p.y >= 0 && p.y < BOARD_HEIGHT) {
            grid[p.y][p.x] = (i == 0) ? 'O' : 'o';
        }
    }

    // Print the grid with borders
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        std::cout << "|";
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            std::cout << grid[y][x];
        }
        std::cout << "|\n";
    }

    std::cout << "----------------------------------------\n";
    std::cout << "Controls: WASD move | P pause | Q quit\n";
    std::cout << "========================================\n";
}

// ─────────────────────────────────────────────
//  tick() — runs one logic frame (used by QTimer)
// ─────────────────────────────────────────────
void Game::tick() {
    if (paused || gameOver) return;

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
        return;
    }

    snake.move();

    // ── Wall Wrap / Invincibility: teleport head to opposite side ────
    if (wallWrap || invincible) {
        Position head = snake.getHead();
        bool wrapped = false;

        if (head.x < 0)             { head.x = BOARD_WIDTH  - 1; wrapped = true; }
        if (head.x >= BOARD_WIDTH)  { head.x = 0;               wrapped = true; }
        if (head.y < 0)             { head.y = BOARD_HEIGHT - 1; wrapped = true; }
        if (head.y >= BOARD_HEIGHT) { head.y = 0;               wrapped = true; }

        if (wrapped) {
            snake.setHead(head);  // Update deque front + bodySet
        }
    }

    checkCollisions();

    currentMove.ateFood      = lastMoveAteFood;
    currentMove.earnedPoints = lastEarnedPoints;
    lastEarnedPoints         = 0;  // reset for next tick
    moveHistory.push(currentMove);

    // Spawn a power-up every POWERUP_SPAWN_INTERVAL foods
    if (lastMoveAteFood) {
        foodEatenCount++;         // FIX: now a member variable, properly reset
        if (foodEatenCount % POWERUP_SPAWN_INTERVAL == 0)
            spawnPowerUp();
    }

    tickPowerUps();
}

// ─────────────────────────────────────────────
//  processInputGUI() — Handle input from Qt
// ─────────────────────────────────────────────
void Game::processInputGUI(InputKey key) {
    switch (key) {
        case InputKey::UP:
            if (currentDirection.y != 1) {
                currentDirection = {0, -1};
                snake.setDirection({0, -1});
            }
            break;

        case InputKey::DOWN:
            if (currentDirection.y != -1) {
                currentDirection = {0, 1};
                snake.setDirection({0, 1});
            }
            break;

        case InputKey::LEFT:
            if (currentDirection.x != 1) {
                currentDirection = {-1, 0};
                snake.setDirection({-1, 0});
            }
            break;

        case InputKey::RIGHT:
            if (currentDirection.x != -1) {
                currentDirection = {1, 0};
                snake.setDirection({1, 0});
            }
            break;

        case InputKey::PAUSE:
            paused = !paused;
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
//  setDifficulty() — Set tick speed & obstacles
// ─────────────────────────────────────────────
void Game::setDifficulty(int diff) {
    currentDifficulty = diff;
    board.clear(); // Clear existing obstacles
    if (diff == Game::EASY) {
        tickMs = 200;
        // Few obstacles
        board.addObstacle({5, 5});
        board.addObstacle({15, 15});
    } else if (diff == Game::MEDIUM) {
        tickMs = BASE_TICK_MS;
        // Medium obstacles
        board.addObstacle({5, 5});
        board.addObstacle({15, 15});
        board.addObstacle({5, 15});
        board.addObstacle({15, 5});
    } else if (diff == Game::HARD) {
        tickMs = 100;
        // Many obstacles
        for(int i=4; i<=16; i+=4) {
            board.addObstacle({i, 5});
            board.addObstacle({i, 15});
        }
    }
}



// ─────────────────────────────────────────────
//  handleInput()
// ─────────────────────────────────────────────
void Game::handleInput() {
    InputKey key = InputHandler::getKeyPress();

    switch (key) {
        case InputKey::UP:
            if (currentDirection.y != 1) {
                currentDirection = {0, -1};
                snake.setDirection({0, -1});
            }
            break;

        case InputKey::DOWN:
            if (currentDirection.y != -1) {
                currentDirection = {0, 1};
                snake.setDirection({0, 1});
            }
            break;

        case InputKey::LEFT:
            if (currentDirection.x != 1) {
                currentDirection = {-1, 0};
                snake.setDirection({-1, 0});
            }
            break;

        case InputKey::RIGHT:
            if (currentDirection.x != -1) {
                currentDirection = {1, 0};
                snake.setDirection({1, 0});
            }
            break;

        case InputKey::PAUSE:
            paused = !paused;
            if (paused) {
                std::cout << "\n  *** PAUSED - press P to resume ***\n";
            }
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
    // Collect all occupied positions (snake body + active power-up positions + obstacles)
    std::unordered_set<Position, PositionHash> occupied;
    for (const Position& p : snake.getBody())
        occupied.insert(p);
    for (const PowerUp& pu : activePowerUps)
        occupied.insert(pu.pos);

    // FIX: also exclude obstacle positions so food never spawns on obstacles
    for (const Position& obs : board.getObstacles())
        occupied.insert(obs);

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
    for (const Position& obs : board.getObstacles())
        occupied.insert(obs);

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

    // 1. Wall collision — SKIP if wall wrap is enabled (already handled in tick())
    if (!wallWrap && !invincible && !board.isInsideBoard(head)) {
        gameOver = true;
        return;
    }

    // 2. Obstacle collision (obstacles still kill even with wall wrap)
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
    clearConsole();
    std::cout << "========================================\n";
    std::cout << "          GAME OVER             \n";
    std::cout << "========================================\n";
    std::cout << "  Final Score : " << score        << "\n";
    std::cout << "  Level       : " << currentLevel << "\n";
    std::cout << "  Length      : " << snake.getBody().size() << "\n";
    std::cout << "========================================\n";
    std::cout << "  Enter your name: ";

    std::string name;
    if (std::cin >> name) {
        saveCurrentScore(name);
        std::cout << "\n  Score saved! Top 10:\n\n";
        leaderboard.displayTop10();
    } else {
        std::cin.clear();
        std::string discard;
        std::cin >> discard;
    }

    std::cout << "\n  Press Enter to return to menu...";
    std::cin.ignore(10000, '\n');
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