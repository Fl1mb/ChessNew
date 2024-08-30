#ifndef REPETITIONHISTORY_H
#define REPETITIONHISTORY_H

#include <vector>
#include "ZobristHash.h"

class RepetitionHistory{
public:

    RepetitionHistory();

    void AddPosition(ZobristHash hash);
    void clear();
    [[nodiscard]] uint8_t getRepetitionNumber(ZobristHash hash) const;

private:
    std::vector<ZobristHash> hashes;

};


#endif // REPETITIONHISTORY_H
