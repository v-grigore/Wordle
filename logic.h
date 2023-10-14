#ifndef LOGIC_H
#define LOGIC_H

#include <unordered_map>
#include <string>
#include <array>
#include <list>

#include "constants.h"

namespace Logic {
    class Letters {
    public:
        Letters();

        std::unordered_map<char, int[9][19]> key;
    };//TODO Declare Letters as struct instead of class

    class Stats {
    public:
        Stats();

        int playedA{} , wonA{} , curStkA{} , maxStkA{} ,
            playedB{} , wonB{} , curStkB{} , maxStkB{};
        std::array<int , 6> guessA{} , guessB{};

        void save() const;
    };

    class Settings {
    public:
        Settings();

        bool solverON{} , updatedVer{} , randomAnswer{};
        int nrOfGames{};

        void save() const;
    };

    void readAns(std::array<std::string , Constants::ansLen>& answers);

    void readGuess(std::array<std::string , Constants::guessLen>& guesses , std::list<std::string>& candidates);
}
#endif
