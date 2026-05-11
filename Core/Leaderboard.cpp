#include "Leaderboard.h"
#include <fstream>
#include <sstream>


// push_back appends the name.  Ties are stored together, never dropped.

void Leaderboard::addScore(std::string name, int score) {
    scores[score].push_back(name);
}


void Leaderboard::saveToFile(std::string filename) const {
    std::ofstream file(filename);

    if (!file.is_open()) {
        return;
    }

    for (std::map<int, std::vector<std::string>>::const_iterator it = scores.begin(); it != scores.end(); ++it) {

        for (std::vector<std::string>::const_iterator nameIt = it->second.begin(); nameIt != it->second.end(); ++nameIt) {

            file << it->first << " " << *nameIt << "\n";
        }
    }

    file.close();
}
void Leaderboard::loadFromFile(std::string filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        return;
    }

    std::string line;
    int loaded = 0;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        int score; std::string name;

        if (iss >> score) {
            std::getline(iss >> std::ws, name);
            addScore(name, score);
            ++loaded;
        }
    }

    file.close();
}