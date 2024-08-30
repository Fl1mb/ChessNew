#ifndef TRANSPOSITIONTABLE_H
#define TRANSPOSITIONTABLE_H

#include <set>
#include "src/ChessAI/headers/Entry.h"

class TranspositionTable{
public:
    TranspositionTable();

    void addEntry(const Entry& entry);
    [[nodiscard]] tryToFindBestMoveIndex(ZobristHash hash_);


    static constexpr uint8_t NONE = 255;
private:

    std::set<Entry> _set;


};

#endif // TRANSPOSITIONTABLE_H
