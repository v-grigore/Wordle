#include "solver.h"

#include <unordered_map>
#include <cmath>
#include <iostream>

namespace Solver1 {

    void pruneCandidates(std::list<std::string> &candidates , const std::string &myoutcome , std::string guess) {
        for (auto it = candidates.begin(); it != candidates.end();) {
            bool isCandidate = true;
            std::string tempGuess = guess;
            for (int i = 0; i < 5; i++)
                if (guess[i] != (*it)[i] && myoutcome[i] == 'g') {
                    isCandidate = false;
                    tempGuess[i] = '0';
                }
            for (int i = 0; i < 5; i++) {
                if (myoutcome[i] == 'b') {
                    for (int j = 0; j < 5; j++) {
                        if (tempGuess[i] == (*it)[j])
                            isCandidate = false;
                    }
                }
                if (myoutcome[i] == 'y') {
                    bool test = false;
                    for (int j = 0; j < 5; j++) {
                        if (tempGuess[i] == (*it)[j]) {
                            test = true;
                            tempGuess[i] = '0';
                            break;
                        }
                    }
                    if (!test || guess[i] == (*it)[i])
                        isCandidate = false;
                }
            }
            if (!isCandidate)
                it = candidates.erase(it);
            else
                it++;
        }
    }
}