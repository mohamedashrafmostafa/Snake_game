#pragma once
#include <map>
#include <string>

class Leaderboard {
private:
    // Binary Search Tree (BST) sorting scores in descending order
    std::map<int, std::string, std::greater<int>> scores;

public:
    void addScore(std::string name, int score);
    void displayTop10() const; // Display top 10 players
    void saveToFile(std::string filename) const;
    void loadFromFile(std::string filename);
};