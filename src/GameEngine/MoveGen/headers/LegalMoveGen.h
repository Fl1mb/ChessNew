#ifndef LEGALMOVEGEN_H
#define LEGALMOVEGEN_H

#include "MoveList.h"
#include "PsLegalMoveGen.h"

class LegalMoveGen{
public:
    static MoveList generate(const Position& position, uint8_t side, bool olnyCapture = false);

private:
    static bool isLegal(Pieces pieces, Move move);

    static void pawnMaskToMoves(Pieces pieces, BitBoard mask, uint8_t attackerSide, uint8_t attacker_index, bool look_for_defender, uint8_t flag, MoveList& moves);
    static void pieceMaskToMoves(Pieces pieces, BitBoard mask, uint8_t attackerP, uint8_t attackerType, uint8_t attackerSide, MoveList& moves);

    static void addEnPassantCaptures(Pieces pieces, uint8_t side, uint8_t enPassant, MoveList& moves);
    static void addCastlingMoves(Pieces pieces, uint8_t side, bool long_castling, bool short_castling, MoveList& moves);
};

#endif // LEGALMOVEGEN_H
