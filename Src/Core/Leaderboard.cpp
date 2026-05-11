#include "Leaderboard.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>


// push_back appends the name.  Ties are stored together, never dropped.

void Leaderboard::addScore(std::string name, int score) {
    scores[score].push_back(name);
}


void Leaderboard::displayTop10() const {
    if (scores.empty()) {
        std::cout << "Leaderboard is empty.\n";
        return;
    }

    std::cout << "\n" << std::string(36, '=') << "\n";
    std::cout << "        TOP 10 LEADERBOARD\n";
    std::cout << std::string(36, '=') << "\n";
    std::cout << std::left
              << std::setw(5)  << "Rank"
              << std::setw(20) << "Name"
              << "Score\n";
    std::cout << std::string(36, '-') << "\n";

    int rank = 1, displayed = 0;

    for (std::map<int, std::vector<std::string>>::const_iterator it = scores.begin(); it != scores.end(); ++it) {
        if (displayed >= 10) break;

        for (std::vector<std::string>::const_iterator nameIt = it->second.begin(); nameIt != it->second.end(); ++nameIt) {
            // Double-check inside the inner loop in case a tie pushes us over 10
            if (displayed >= 10) break;

            std::cout << std::left
                      << std::setw(5)  << rank
                      << std::setw(20) << *nameIt
                      << it->first << "\n";

            ++displayed;
        }
        rank += static_cast<int>(it->second.size());
    }



    std::cout << std::string(36, '=') << "\n\n";
}



void Leaderboard::saveToFile(std::string filename) const {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: could not open \"" << filename << "\" for writing.\n";
        return;
    }

    for (std::map<int, std::vector<std::string>>::const_iterator it = scores.begin(); it != scores.end(); ++it) {

        for (std::vector<std::string>::const_iterator nameIt = it->second.begin(); nameIt != it->second.end(); ++nameIt) {

            file << it->first << " " << *nameIt << "\n";
        }
    }

    file.close();
    std::cout << "Leaderboard saved to \"" << filename << "\".\n";
}
// ── loadFromFile ──────────────────────────────────────────────────────────────
// Reads the format written by saveToFile.
// Uses addScore so duplicates and merging work identically to normal inserts.
// Malformed lines are skipped with a warning instead of crashing.
// Time complexity: O(n log n) — one BST insert per line
// ─────────────────────────────────────────────────────────────────────────────
void Leaderboard::loadFromFile(std::string filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Warning: could not open \"" << filename
                             << "\". Starting with an empty leaderboard.\n";
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
        else {
            std::cerr << "Warning: skipping malformed line: \"" << line << "\"\n";
        }
    }

    file.close();
    std::cout << "Loaded " << loaded << " entr"
              << (loaded == 1 ? "y" : "ies")
              << " from \"" << filename << "\".\n";
}