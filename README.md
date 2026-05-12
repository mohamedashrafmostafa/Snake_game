# 🐍 Snake++

**Course:** CSE333 - Data Structures and Algorithms
**Semester:** Spring 2026
**Department:** Computer and Systems Engineering Department
**Institution:** Ain Shams University - Faculty of Engineering, Specialized Programs

---

## 👥 Team Members

| # | Name | Student ID |
|---|------|------------|
| 1 |Mohamed Ashraf|2300475|
| 2 |omar Ahmed |2300131 |
| 3 |yahia Mohamed|2300644|
| 4 |Amr Emad|2300379|
| 5 |Ahmed Mohamed|2300099|

---

## 📖 Project Description

Snake++ is a robust C++ implementation of the classic playable Snake Game. The system handles core game logic such as continuous grid-based movement, food collection, dynamic growth of the snake, and wall/self-collision detection. Beyond the classic mechanics, this project integrates advanced data structures to manage an active power-up system and a persistent high-score leaderboard, successfully applying problem-solving concepts to a meaningful application problem.

---

## ✨ Implemented Features

1. **Classic Game Loop & Mechanics:** Smooth, non-blocking directional movement, randomized food generation, incremental snake growth, and accurate collision detection.
2. **Persistent BST Leaderboard:** A high-score tracking system that saves, loads, and dynamically ranks player scores using a custom Binary Search Tree implementation, persisting data to `leaderboard.txt`.
3. **Priority-Based Power-Ups:** A system that spawns special items on the board, managing their active durations and effects using a priority queue.
4. **Wall & Self-Collision Detection:** The game correctly ends when the snake collides with the boundary walls or its own body.
5. **Score Tracking:** The player's current score is updated in real time as food is collected.

---

## 🏗️ Data Structures Used

Our design focuses on efficient data organization and correct implementation using course-related data structures:

- **Double-Ended Queue (`std::deque`):** Used in `Snake.h` to represent the snake's body.
  - *Why:* A deque allows for optimal O(1) operations at both ends. Moving the snake simply requires pushing a new head coordinate to the front of the deque and popping the tail coordinate from the back, avoiding the O(N) shifting overhead of standard arrays.

- **Binary Search Tree (BST):** Used in `Leaderboard.h` to store and manage player scores.
  - *Why:* A BST provides efficient O(log N) average time complexity for inserting new scores and allows us to easily traverse the tree in descending order to display the top players.

- **Priority Queue:** Used in `PowerUp.h` to manage active timed power-ups.
  - *Why:* Ensures that the game easily tracks which power-up expires next or takes precedence without needing to continuously sort an array every frame.

- **2D Array:** Used in `Board.h` for grid rendering and spatial tracking.
  - *Why:* Provides constant O(1) lookups to instantly check if a coordinate contains a wall, food, or empty space.

---

## 📥 How to Get the Project

Clone the repository using Git:

```bash
git clone https://github.com/mohamedashrafmostafa/Snake_game.git
cd Snake_game
```

Or download it directly from GitHub:
[https://github.com/mohamedashrafmostafa/Snake_game](https://github.com/mohamedashrafmostafa/Snake_game)

---

## 📋 Prerequisites

Before building, make sure you have:

- [Qt6](https://www.qt.io/download-qt-installer) installed (includes MinGW, CMake, and Ninja)
- CMake 3.20+ *(bundled with Qt — no separate install needed)*

---

## 🔧 Build Instructions

### Step 1 — Find your Qt installation path

After installing Qt, locate your install directory. It typically looks like:

```
C:\Qt\
D:\Qt\
```

You will need to know:
- Your **Qt version** (e.g. `6.8.3`)
- Your **Qt install root** (e.g. `C:\Qt`)

> **Tip:** Open Qt Maintenance Tool — the path shown at the top is your install root.

---

### Step 2 — Configure the project

Open **Command Prompt** in the project root folder and run the command below.  
Replace `<QT_PATH>` with your Qt install root and `<QT_VERSION>` with your version:

```cmd
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE="<QT_PATH>\<QT_VERSION>\mingw_64\lib\cmake\Qt6\qt.toolchain.cmake" -DCMAKE_C_COMPILER="<QT_PATH>\Tools\mingw1310_64\bin\gcc.exe" -DCMAKE_CXX_COMPILER="<QT_PATH>\Tools\mingw1310_64\bin\g++.exe" -S . -B build -G "MinGW Makefiles"
```

**Example** — if Qt is installed at `C:\Qt\6.8.3`:

```cmd
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE="C:\Qt\6.8.3\mingw_64\lib\cmake\Qt6\qt.toolchain.cmake" -DCMAKE_C_COMPILER="C:\Qt\Tools\mingw1310_64\bin\gcc.exe" -DCMAKE_CXX_COMPILER="C:\Qt\Tools\mingw1310_64\bin\g++.exe" -S . -B build -G "MinGW Makefiles"
```

---

### Step 3 — Build

```cmd
cmake --build build
```

---

### Step 4 — Deploy Qt DLLs

The `.exe` needs Qt DLLs to run. Deploy them with `windeployqt`:

```cmd
<QT_PATH>\<QT_VERSION>\mingw_64\bin\windeployqt.exe build\SnakePlusPlus.exe
```

**Example:**

```cmd
C:\Qt\6.8.3\mingw_64\bin\windeployqt.exe build\SnakePlusPlus.exe
```

> This copies all required Qt DLLs next to your executable automatically.

---

### Step 5 — Run

```cmd
build\SnakePlusPlus.exe
```

---

## 🛠️ Alternative — Qt Creator (No terminal needed)

1. Open **Qt Creator** → `File > Open File or Project`
2. Select `CMakeLists.txt`
3. Choose a **Qt6 MinGW kit** when prompted
4. Press **Build** (`Ctrl+B`) then **Run** (`Ctrl+R`)

Qt Creator handles all paths and DLL deployment automatically.

---

## ❓ Troubleshooting

| Error | Fix |
|---|---|
| `Could not find Qt6` | Double-check `<QT_PATH>` and `<QT_VERSION>` in the cmake command |
| `gcc.exe not found` | Verify MinGW is inside `<QT_PATH>\Tools\mingw1310_64\bin\` |
| `.exe crashes on launch` | Run `windeployqt` (Step 4) — missing DLLs |
| `CMakeLists.txt not found` | Make sure you're running the command from the **project root**, not inside `build\` |

## 🤖 AI Usage Declaration

In accordance with the course guidelines, we transparently declare our use of AI tools throughout this project.

### Tools Used
- **Claude** — used for brainstorming data structure selection, improving documentation, and helping structure the README.
- **Gemini** — used for debugging assistance and learning concepts related to BST traversal and priority queue implementation.
- **Copilot** — Used as an agent to autocomplete any redundant lines as for loops instantiation and missing semi colons
  
### What AI Was Used For
- Brainstorming which data structures best fit each game subsystem.
- Learning and clarifying concepts (e.g., deque vs. vector trade-offs, BST in-order traversal).
- Debugging logic errors during development.
- Improving the structure and wording of this README.
- Used it to help us implement our ideas for the gui by using the suitable libraries inside QT  

### What Was Modified or Rejected
- AI-suggested BST implementations were reviewed and rewritten by the team to match the exact requirements of the course and our specific score-comparison logic.
- README sections generated by AI were edited to reflect our actual implementation details and file structure accurately.
- When adding the final touches in the code it deleted some comments and part of the code which caused some bugs as after careful reviewing of the code we were able to fix it :)


