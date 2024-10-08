#ifndef OPENINGBOOK_H
#define OPENINGBOOK_H

#include <fstream>
#include <sstream>
#include <tuple>
#include "src/GameEngine/MoveGen/headers/LegalMoveGen.h"

class OpeningBook{
public:
    OpeningBook();
    OpeningBook(const std::string& path);

    std::tuple<int32_t, bool, Move> TryToFindMove(const Position& position);

private:
    std::vector<std::vector<Move>> moves_;

};


#endif // OPENINGBOOK_H
