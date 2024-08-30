#include "src/ChessAI/headers/Entry.h"


Entry::Entry() = default;




Entry::Entry(ZobristHash hash, int32_t depth, uint8_t best_move_index) : hash_(hash), depth_(depth), best_move_index_(best_move_index)
{}

bool operator<(Entry left, Entry right){
    return left.hash_ < right.hash_;
}
