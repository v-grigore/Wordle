#ifndef SOLVER_H
#define SOLVER_H

#include <string>
#include <array>
#include <list>
#include <unordered_map>
#include <map>

#include "constants.h"

namespace Solver1 {

    void pruneCandidates(std::list<std::string> &candidates , const std::string &myoutcome , std::string guess);
}

#endif
