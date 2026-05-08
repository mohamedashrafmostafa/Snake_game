#pragma once
#include <map>
#include <vector>       // ADDED: needed for vector<string>
#include <string>
#include <functional>   // ADDED: needed for greater<int>

class Leaderboard {
private:
    // std::map is a BST — sorted descending by score (greater<int>)
    // vector<string> handles duplicate scores without overwriting
    std::map<int, std::vector<std::string>, std::greater<int>> scores; // FIXED: was map<int, string>

public:
    const std::map<int, std::vector<std::string>, std::greater<int>>& getScores() const { return scores; }
    void addScore(std::string name, int score);
    void displayTop10() const;
    void saveToFile(std::string filename) const;
    void loadFromFile(std::string filename);
};
