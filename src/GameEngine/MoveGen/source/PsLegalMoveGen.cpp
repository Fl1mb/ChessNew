#include "src/GameEngine/MoveGen/headers/PsLegalMoveGen.h"



BitBoard PsLegalMoveGen::generatePawnDefaultMask(Pieces pieces, uint8_t side)
{
    if (side == SIDE::WHITE) {
        return (pieces.getPieceBitBoard(SIDE::WHITE, PIECE::PAWN) << 8) & pieces.getEmptyBitBoard();
    }
    return (pieces.getPieceBitBoard(SIDE::BLACK, PIECE::PAWN) >> 8) & pieces.getEmptyBitBoard();
}

BitBoard PsLegalMoveGen::generatePawnLongMask(Pieces piece, uint8_t side)
{
    BitBoard defaultMask = generatePawnDefaultMask(piece, side);
    if (side == SIDE::WHITE) {
        return ((defaultMask & BRows::ROWS[2]) << 8) & piece.getEmptyBitBoard();
    }
    return ((defaultMask & BRows::ROWS[5]) >> 8) & piece.getEmptyBitBoard();
}

BitBoard PsLegalMoveGen::generatePawnLeftCaptureMask(Pieces pieces, uint8_t side, bool includeAll )
{
    if (side == SIDE::WHITE) {
        BitBoard mask = (pieces.getPieceBitBoard(SIDE::WHITE, PIECE::PAWN) << 7) & BColumns::INV_COLUMNS[7];
        if (!includeAll) {
            mask = mask & pieces.getSideBitBoard(SIDE::BLACK);
        }
        return mask;
    }

    BitBoard mask = (pieces.getPieceBitBoard(SIDE::BLACK, PIECE::PAWN) >> 9) & BColumns::INV_COLUMNS[7];
    if (!includeAll) {
        mask = mask & pieces.getSideBitBoard(SIDE::WHITE);
    }
    return mask;
}

BitBoard PsLegalMoveGen::generatePawnRightCaptureMask(Pieces pieces, uint8_t side, bool includeAll)
{
    if (side == SIDE::WHITE) {
        BitBoard mask = (pieces.getPieceBitBoard(SIDE::WHITE, PIECE::PAWN) << 9) & BColumns::INV_COLUMNS[0];
        if (!includeAll) {
            mask = mask & pieces.getSideBitBoard(SIDE::BLACK);
        }
        return mask;
    }

    BitBoard mask = (pieces.getPieceBitBoard(SIDE::BLACK, PIECE::PAWN) >> 7) & BColumns::INV_COLUMNS[0];
    if (!includeAll) {
        mask = mask & pieces.getSideBitBoard(SIDE::WHITE);
    }
    return mask;

}

BitBoard PsLegalMoveGen::generateKnightMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCapture)
{
    if (onlyCapture) {
        return KnightMasks::MASKS[p] & pieces.getSideBitBoard(Pieces::inverse(side));
    }
    return KnightMasks::MASKS[p] & pieces.getInvSideBitBoard(side);
}

BitBoard PsLegalMoveGen::generateKingMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCapture)
{
    if (onlyCapture) {
        return KingMasks::MASKS[p] & pieces.getSideBitBoard(Pieces::inverse(side));
    }
    return KingMasks::MASKS[p] & pieces.getInvSideBitBoard(side);

}

BitBoard PsLegalMoveGen::generateBishopMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCapture)
{
    BitBoard nw = calcRay(pieces, p, side, onlyCapture, SlidersMasks::DIRECTION::NORTH_WEST, false);
    BitBoard ne = calcRay(pieces, p, side, onlyCapture, SlidersMasks::DIRECTION::NORTH_EAST, false);
    BitBoard sw = calcRay(pieces, p, side, onlyCapture, SlidersMasks::DIRECTION::SOUTH_WEST, true);
    BitBoard se = calcRay(pieces, p, side, onlyCapture, SlidersMasks::DIRECTION::SOUTH_EAST, true);
    return nw | ne | sw | se;
}

BitBoard PsLegalMoveGen::generateRookMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCapture)
{
    BitBoard north = calcRay(pieces, p, side, onlyCapture, SlidersMasks::DIRECTION::NORTH, false);
    BitBoard south = calcRay(pieces, p, side, onlyCapture, SlidersMasks::DIRECTION::SOUTH, true);
    BitBoard west = calcRay(pieces, p, side, onlyCapture, SlidersMasks::DIRECTION::WEST, true);
    BitBoard east = calcRay(pieces, p, side, onlyCapture, SlidersMasks::DIRECTION::EAST, false);
    return north | south | west | east;
}

BitBoard PsLegalMoveGen::generateQueenMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCapture)
{
    return generateBishopMask(pieces, p, side, onlyCapture) | generateRookMask(pieces, p, side, onlyCapture);
}

bool PsLegalMoveGen::inDanger(Pieces pieces, uint8_t p, uint8_t side)
{
    BitBoard oppositePawnsLeftCaptures = generatePawnLeftCaptureMask(pieces, Pieces::inverse(side), true);
    BitBoard oppositePawnsRightCaptures = generatePawnRightCaptureMask(pieces, Pieces::inverse(side), true);
    BitBoard oppositePawnsCaptures = oppositePawnsLeftCaptures | oppositePawnsRightCaptures;

    if (BitBoardOperations::getBit(oppositePawnsCaptures, p)) {
        return true;
    }

    if (generateKnightMask(pieces, p, side, true) & pieces.getPieceBitBoard(Pieces::inverse(side), PIECE::KNIGHT)) {
        return true;
    }
    if (generateBishopMask(pieces, p, side, true) & pieces.getPieceBitBoard(Pieces::inverse(side), PIECE::BISHOP)) {
        return true;
    }
    if (generateRookMask(pieces, p, side, true) & pieces.getPieceBitBoard(Pieces::inverse(side), PIECE::ROOK)) {
        return true;
    }
    if (generateQueenMask(pieces, p, side, true) & pieces.getPieceBitBoard(Pieces::inverse(side), PIECE::QUEEN)) {
        return true;
    }
    if (generateKingMask(pieces, p, side, true) & pieces.getPieceBitBoard(Pieces::inverse(side), PIECE::KING)) {
        return true;
    }

    return false;
}

BitBoard PsLegalMoveGen::calcRay(Pieces pieces, uint8_t p, uint8_t side, bool OnlyCapture, uint8_t dir, bool bsr)
{
    BitBoard blockers = SlidersMasks::Masks[p][dir] & pieces.getAllBitBoard();
    if (blockers == 0) {
        if (OnlyCapture) {
            return 0;
        }
        return SlidersMasks::Masks[p][dir];
    }

    uint8_t blockingSquare;
    if (bsr) {
        blockingSquare = BitBoardOperations::bsr(blockers);
    }
    else {
        blockingSquare = BitBoardOperations::bsf(blockers);
    }

    BitBoard moves;
    if (OnlyCapture) {
        moves = 0;
    }
    else {
        moves = SlidersMasks::Masks[p][dir] ^ SlidersMasks::Masks[blockingSquare][dir];
    }

    if (BitBoardOperations::getBit(pieces.getSideBitBoard(side), blockingSquare)) {
        moves = BitBoardOperations::set0(moves, blockingSquare);
    }
    else {
        moves = BitBoardOperations::set1(moves, blockingSquare);
    }

    return moves;
}
