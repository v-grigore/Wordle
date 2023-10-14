#include "letters.h"

#include <fstream>

Letters::Letters() {
    std::ifstream fin("letters");

    for (char k = 'a'; k <= 'z'; k++)
        for (int i = 1; i < 8; i++)
            for (int j = 5; j < 15; j++)
                fin >> key[k][i][j];

    fin.close();
}
