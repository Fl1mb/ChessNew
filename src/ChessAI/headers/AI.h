#ifndef AI_H
#define AI_H

#include <chrono>
#include <future>
#include <thread>
#include <unistd.h>
#include <iomanip>
#include "src/GameEngine/MoveGen/headers/LegalMoveGen.h"
#include "MoveSorter.h"
#include "TranspositionTable.h"
#include "OpeningBook.h"
#include "SearchInterrupter.h"

class AI{
public:

    AI();
    AI(const std::string& opening_book_path);

    static Move getBestMove(const Position& position, uint8_t side, int32_t ms);

private:
    OpeningBook opening_book;

    static std::tuple<int32_t, bool, Move> AlphaBeta(const Position& position,  uint8_t side, int32_t depthLeft);

    static std::tuple<int32_t, bool, Move> alphaBetaMin(const Position& position, int32_t alpha, int32_t beta, int32_t depthLeft, int32_t depthCurrent = 0);
    static std::tuple<int32_t, bool, Move> alphaBetaMax(const Position& position, int32_t alpha, int32_t beta, int32_t depthLeft, int32_t depthCurrent = 0);

    static int32_t alphaBetaMinOnlyCaptures(const Position& position, int32_t alpha, int32_t beta);
    static int32_t alphaBetaMaxOnlyCaptures(const Position& position, int32_t alpha, int32_t beta);

    struct INF{
        static constexpr int32_t positive = 1e+9;
        static constexpr int32_t negative = -1e+9;
    };

};

#endif // AI_H
