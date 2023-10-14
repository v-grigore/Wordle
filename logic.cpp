#include "logic.h"

#include <fstream>

namespace Logic {
    Letters::Letters() {
        std::ifstream fin("letters");

        for (char k = 'a'; k <= 'z'; k++)
            for (int i = 1; i < 8; i++)
                for (int j = 5; j < 15; j++)
                    fin >> key[k][i][j];

        fin.close();
    }

    Stats::Stats() {
        std::ifstream fin("stats");

        fin >> playedA >> wonA >> curStkA >> maxStkA;
        for (int i = 0; i < 6; i++)
            fin >> guessA[i];
        fin >> playedB >> wonB >> curStkB >> maxStkB;
        for (int i = 0; i < 6; i++)
            fin >> guessB[i];

        fin.close();
    }

    void Stats::save() const {
        std::ofstream fout("stats");

        fout << playedA << '\n' << wonA << '\n' << curStkA << '\n' << maxStkA << '\n';
        for (int i = 0; i < 6; i++)
            fout << guessA[i] << '\n';
        fout << playedB << '\n' << wonB << '\n' << curStkB << '\n' << maxStkB << '\n';
        for (int i = 0; i < 6; i++)
            fout << guessB[i] << '\n';

        fout.close();
    }

    Settings::Settings() {
        std::ifstream fin("settings");

        fin >> solverON >> updatedVer >> randomAnswer >> nrOfGames;

        fin.close();
    }

    void Settings::save() const {
        std::ofstream fout("settings");

        fout << solverON << '\n' << updatedVer << '\n' << randomAnswer << '\n' << nrOfGames;

        fout.close();
    }

    void readAns(std::array<std::string , Constants::ansLen>& answers) {
        std::ifstream fin("answers.txt");
        for (int i = 0; i < Constants::ansLen; i++)
            fin >> answers[i];
        fin.close();
    }

    void readGuess(std::array<std::string , Constants::guessLen>& guesses , std::list<std::string>& candidates) {
        std::ifstream fin("guesses.txt");
        for (int i = 0; i < Constants::guessLen; i++) {
            fin >> guesses[i];
            candidates.push_front(guesses[i]);
        }
        fin.close();
    }
}