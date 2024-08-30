#ifndef PSLEGALMOVEGEN_H
#define PSLEGALMOVEGEN_H

#include "KnightMask.h"
#include "KingMasks.h"
#include "SliderMasks.h"
#include "src/GameEngine/PosRep/headers/Position.h"


class PsLegalMoveGen{
public:
    static BitBoard generatePawnDefaultMask(Pieces pieces, uint8_t side);
    static BitBoard generatePawnLongMask(Pieces piece, uint8_t side);
    static BitBoard generatePawnLeftCaptureMask(Pieces pieces, uint8_t side, bool includeAll = false);
    static BitBoard generatePawnRightCaptureMask(Pieces pieces, uint8_t side, bool includeAll = false);


    static BitBoard generateKnightMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCapture = false);
    static BitBoard generateKingMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCapture = false);
    static BitBoard generateBishopMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCapture = false);
    static BitBoard generateRookMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCapture = false);
    static BitBoard generateQueenMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCapture = false);

    static bool inDanger(Pieces pieces, uint8_t p, uint8_t side);

private:
    static BitBoard calcRay(Pieces pieces, uint8_t p, uint8_t side, bool OnlyCapture, uint8_t dir, bool bsr);
};

#endif // PSLEGALMOVEGEN_H
