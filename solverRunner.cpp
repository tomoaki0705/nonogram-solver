#include <iostream>
#include "solver.hpp"

char inline replace(const int s) { return s == -1 ? '.' : s == 0 ? '_' : 'F'; };
char inline replace(const enum state s) {
    switch (s)
    {
    case STATE_FILL:
        return 'F';
    case STATE_EMPTY:
        return '_';
    case STATE_UNCERTAIN:
    default:
        return '.';
    }
}

int main(int argc, char**argv)
{
    clues_t cluesH, cluesV;
    cluesH.push_back(3);
    cluesH.push_back(4);
    cluesH.push_back(2);
    cluesH.push_back(5);
    cluesV.push_back(3);
    cluesV.push_back(2);
    cluesV.push_back(5);
    cluesV.push_back(3);
    nonogramSolver solverH(20, cluesH);
    nonogramSolver solverV(20, cluesV);
    solverH.solve();
    solverV.solve();
    for (size_t i = 0; i < 20; i++)
    {
        std::cout << i % 10;
    }
    std::cout << std::endl;
    for (size_t i = 0; i < 20; i++)
    {
        std::cout << replace(solverH[i]);
    }
    std::cout << std::endl;
    for (size_t i = 0; i < 20; i++)
    {
        std::cout << replace(solverV[i]);
    }
    std::cout << std::endl;
    patterns_t patternH(20, STATE_UNCERTAIN);
    patterns_t patternV(20, STATE_UNCERTAIN);
    patternH[7] = STATE_FILL;
    patternH[8] = STATE_FILL;
    patternH[12] = STATE_FILL;
    patternV[6] = STATE_FILL;
    patternV[7] = STATE_FILL;
    patternV[8] = STATE_FILL;
    patternV[11] = STATE_FILL;
    solverH.overwritePattern(patternH);
    solverV.overwritePattern(patternV);
    for (size_t i = 0; i < 20; i++)
    {
        std::cout << replace(solverH[i]);
    }
    std::cout << std::endl;
    solverH.solve();
    for (size_t i = 0; i < 20; i++)
    {
        std::cout << replace(solverV[i]);
    }
    std::cout << std::endl;
    solverV.solve();
    for (size_t i = 0; i < 20; i++)
    {
        std::cout << replace(solverV[i]);
    }
    std::cout << std::endl;
    for (size_t i = 0; i < 20; i++)
    {
        std::cout << replace(solverH[i]);
    }
    std::cout << std::endl;
    for (size_t i = 0; i < 20; i++)
    {
        std::cout << replace(solverV[i]);
    }
    std::cout << std::endl;
    return 0;
}