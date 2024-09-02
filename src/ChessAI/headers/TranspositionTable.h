#ifndef TRANSPOSITIONTABLE_H
#define TRANSPOSITIONTABLE_H

#include <unordered_map>
#include "src/ChessAI/headers/Entry.h"

class TranspositionTable{
public:

    TranspositionTable(const TranspositionTable& other) = delete;

    static TranspositionTable *getPtr();

    void addEntry(ZobristHash hash, int32_t depth, uint8_t bestMoveIndex);
    [[nodiscard]] tryToFindBestMoveIndex(ZobristHash hash_);


    static constexpr uint8_t NONE = 255;
private:
    TranspositionTable();
    static TranspositionTable* table;

     std::unordered_map<uint64_t, std::pair<int32_t, uint8_t>> map;


};

#endif // TRANSPOSITIONTABLE_H
