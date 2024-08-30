#include "src/GameEngine/PosRep/headers/RepetitionHistory.h"


RepetitionHistory::RepetitionHistory() = default;



void RepetitionHistory::AddPosition(ZobristHash hash)
{
    this->hashes.push_back(hash);
}

void RepetitionHistory::clear()
{
    this->hashes.clear();
}

uint8_t RepetitionHistory::getRepetitionNumber(ZobristHash hash) const
{
    uint8_t ctr = 0;
    for (auto hash1 : this->hashes) {
        if (hash == hash1) {
            ctr = ctr + 1;
        }
    }
    return ctr;
}
