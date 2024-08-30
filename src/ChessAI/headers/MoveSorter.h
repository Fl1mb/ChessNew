#ifndef MOVESORTER_H
#define MOVESORTER_H

#include "StaticEvaluator.h"
#include "src/GameEngine/MoveGen/headers/MoveList.h"

class MoveSorter{
public:
    static MoveList sort(Pieces pieces, MoveList moves);
private:
    static int32_t evaluateMove(Pieces pieces, Move move);
};


#endif // MOVESORTER_H
