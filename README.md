# 🐍 Snake++ 

**Course:** CSE333 - Data Structures and Algorithms
**Semester:** Spring 2026[cite: 1]  
**Department:** Computer and Systems Engineering Department
**Institution:** Ain Shams University - Faculty of Engineering, Specialized Programs


## 📖 Project Description
Snake++ is a robust C++ implementation of the classic playable Snake Game. The system handles core game logic such as continuous grid-based movement, food collection, dynamic growth of the snake, and wall/self-collision detection. Beyond the classic mechanics, this project integrates advanced data structures to manage an active power-up system and a persistent high-score leaderboard, successfully applying problem-solving concepts to a meaningful application problem.

## ✨ Core Features
1.  **Classic Game Loop & Mechanics:** Smooth, non-blocking directional movement, randomized food generation, incremental snake growth, and accurate collision detection.
2.  **Persistent BST Leaderboard:** A high-score tracking system that saves, loads, and dynamically ranks player scores using a custom Binary Search Tree implementation, persisting data to `leaderboard.txt`.
3.  **Priority-Based Power-Ups:** A system that spawns special items on the board, managing their active durations and effects using a priority queue.

## 🏗️ Data Structures Used
Our design focuses on efficient data organization and correct implementation using course-related data structures:

*   **Double-Ended Queue (`std::deque`):** Used in `Snake.h` to represent the snake's body. 
    *   *Why:* A deque allows for optimal O(1) operations at both ends. Moving the snake simply requires pushing a new head coordinate to the front of the deque and popping the tail coordinate from the back, avoiding the O(N) shifting overhead of standard arrays.
*   **Binary Search Tree (BST):** Used in `Leaderboard.h` to store and manage player scores.
    *   *Why:* A BST provides efficient O(log N) average time complexity for inserting new scores and allows us to easily traverse the tree in descending order to display the top players.
*   **Priority Queue:** Used in `PowerUp.h` to manage active timed power-ups.
    *   *Why:* Ensures that the game easily tracks which power-up expires next or takes precedence without needing to continuously sort an array every frame.
*   **2D Array:** Used in `Board.h` for grid rendering and spatial tracking.
    *   *Why:* Provides constant O(1) lookups to instantly check if a coordinate contains a wall, food, or empty space.

## 🚀 How to Compile and Run
To compile and execute the game from the terminal, ensure you have a standard C++ compiler (like `g++`) installed. 

1. **Navigate to the project directory:**
   ```bash
   cd path/to/SnakePlusPlus
   ```
2. **Compile the source files:**
   ```bash
   g++ main.cpp Game.cpp Snake.cpp Board.cpp Food.cpp Leaderboard.cpp PowerUp.cpp InputHandler.cpp Utils.cpp -o snake_game
   ```
3. **Run the executable:**
   ```bash
   # On Windows:
   snake_game.exe
   
   # On Linux/macOS:
   ./snake_game
   ```
