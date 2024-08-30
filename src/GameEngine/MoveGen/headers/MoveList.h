#ifndef MOVELIST_H
#define MOVELIST_H

#include "src/GameEngine/PosRep/headers/Move.h"
#include "src/GameEngine/PosRep/headers/Pieces.h"

class MoveList{
   public:
    MoveList();

    Move &operator[](uint8_t index);
    Move operator[](uint8_t index) const;

    void push_back(Move move);
    [[nodiscard]] uint8_t size()const;

private:
    std::array<Move, 218> moves;
    uint8_t size_;
};


#endif // MOVELIST_H
