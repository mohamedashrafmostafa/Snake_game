#pragma once
#include <map>
#include <vector>
#include <string>
#include <functional>

class Leaderboard {
private:
    std::map<int, std::vector<std::string>, std::greater<int>> scores;

public:
    const std::map<int, std::vector<std::string>, std::greater<int>>& getScores() const { return scores; }
    void addScore(std::string name, int score);
    void saveToFile(std::string filename) const;
    void loadFromFile(std::string filename);
};
