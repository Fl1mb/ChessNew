#ifndef ENTRY_H
#define ENTRY_H

#include "src/GameEngine/PosRep/headers/ZobristHash.h"

class Entry{
public:
    Entry();
    Entry(ZobristHash hash, int32_t depth, uint8_t best_move_index);

    friend bool operator<(Entry left, Entry right);

    ZobristHash hash_;
    int32_t depth_;
    uint8_t best_move_index_;
};

#endif // ENTRY_H
