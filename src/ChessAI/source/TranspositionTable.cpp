#include "src/ChessAI/headers/TranspositionTable.h"



TranspositionTable::TranspositionTable() = default;
void TranspositionTable::addEntry(const Entry &entry)
{
    auto hash_copy = this->_set.find(entry);
    if(hash_copy == this->_set.end() or hash_copy->depth_ < entry.depth_)
        this->_set.insert(entry);
}

TranspositionTable::tryToFindBestMoveIndex(ZobristHash hash_)
{
    auto entry = this->_set.find({hash_, 0, 0});

    if(entry == this->_set.end()) return NONE;
    return entry->best_move_index_;
}


