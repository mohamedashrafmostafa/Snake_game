#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <vector>

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

// All terminal-related rendering removed for GUI-only build

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
    int rand_num_obstacles = 0;
    int rand_x = rand() % BOARD_WIDTH;
    int rand_y = rand() % BOARD_HEIGHT;
    board.clear(); // Clear existing obstacles
    if (diff == Game::EASY) {
        tickMs = 200;
    } else if (diff == Game::MEDIUM) {
        tickMs = BASE_TICK_MS;
        // Medium obstacles
        rand_num_obstacles = 2 + rand() % 4; // 2 to 5 obstacles
        for (int i = 0; i < rand_num_obstacles; ++i) {
            rand_x = rand() % BOARD_WIDTH;
            rand_y = rand() % BOARD_HEIGHT;
            board.addObstacle({rand_x, rand_y});
        }
    } else if (diff == Game::HARD) {
        tickMs = 100;
        // Many obstacles
        rand_num_obstacles = 10 + rand() % 11; // 10 to 20 obstacles
        for (int i = 0; i < rand_num_obstacles; ++i) {
            rand_x = rand() % BOARD_WIDTH;
            rand_y = rand() % BOARD_HEIGHT;
            board.addObstacle({rand_x, rand_y});
        }
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
//  saveCurrentScore()
// ─────────────────────────────────────────────
void Game::saveCurrentScore(std::string playerName) {
    leaderboard.addScore(playerName, score);
    leaderboard.saveToFile("leaderboard.txt");
}

