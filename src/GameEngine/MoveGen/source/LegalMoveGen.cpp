#include "src/GameEngine/MoveGen/headers/LegalMoveGen.h"

#define Bitboard BitBoard
#define PsLegalMoveMaskGen PsLegalMoveGen
#define BOp BitBoardOperations


MoveList LegalMoveGen::generate(const Position &position, uint8_t side, bool onlyCaptures) {
    MoveList moves;

    Bitboard pawnsLeftCaptures = PsLegalMoveMaskGen::generatePawnLeftCaptureMask(position.getPieces(), side, false);
    Bitboard pawnsRightCaptures = PsLegalMoveMaskGen::generatePawnRightCaptureMask(position.getPieces(), side, false);

    int8_t pawnsLeftCaptureIndex;
    int8_t pawnsRightCaptureIndex;

    if (side == SIDE::WHITE) {
        pawnsLeftCaptureIndex = -7;
        pawnsRightCaptureIndex = -9;
    }
    else {
        pawnsLeftCaptureIndex = 9;
        pawnsRightCaptureIndex = 7;
    }
    pawnMaskToMoves(position.getPieces(), pawnsLeftCaptures, side, pawnsLeftCaptureIndex, true,Move::FLAGS::DEFAULT, moves);
    pawnMaskToMoves(position.getPieces(), pawnsRightCaptures, side, pawnsRightCaptureIndex, true,Move::FLAGS::DEFAULT, moves);

    if (!onlyCaptures) {
        Bitboard pawnsDefault = PsLegalMoveMaskGen::generatePawnDefaultMask(position.getPieces(), side);
        Bitboard pawnsLong = PsLegalMoveMaskGen::generatePawnLongMask(position.getPieces(), side);

        int8_t pawnDefaultIndex;
        int8_t pawnLongIndex;
        if (side == SIDE::WHITE) {
            pawnDefaultIndex = -8;
            pawnLongIndex = -16;
        }
        else {
            pawnDefaultIndex = 8;
            pawnLongIndex = 16;
        }
        pawnMaskToMoves(position.getPieces(), pawnsDefault, side, pawnDefaultIndex, false,Move::FLAGS::DEFAULT, moves);
        pawnMaskToMoves(position.getPieces(), pawnsLong, side, pawnLongIndex, false,Move::FLAGS::PAWN_LONG_MOVE, moves);
    }

    Bitboard allKnights = position.getPieces().getPieceBitBoard(side, PIECE::KNIGHT);
    Bitboard allBishops = position.getPieces().getPieceBitBoard(side, PIECE::BISHOP);
    Bitboard allRooks = position.getPieces().getPieceBitBoard(side, PIECE::ROOK);
    Bitboard allQueens = position.getPieces().getPieceBitBoard(side, PIECE::QUEEN);
    uint8_t attackerP;
    Bitboard mask;
    while (allKnights) {
        attackerP = BOp::bsf(allKnights);
        allKnights = BOp::set0(allKnights, attackerP);
        mask = PsLegalMoveMaskGen::generateKnightMask(position.getPieces(), attackerP, side, onlyCaptures);
        pieceMaskToMoves(position.getPieces(), mask, attackerP, PIECE::KNIGHT, side, moves);
    }
    while (allBishops) {
        attackerP = BOp::bsf(allBishops);
        allBishops = BOp::set0(allBishops, attackerP);
        mask = PsLegalMoveMaskGen::generateBishopMask(position.getPieces(), attackerP, side, onlyCaptures);
        pieceMaskToMoves(position.getPieces(), mask, attackerP, PIECE::BISHOP, side, moves);
    }
    while (allRooks) {
        attackerP = BOp::bsf(allRooks);
        allRooks = BOp::set0(allRooks, attackerP);
        mask = PsLegalMoveMaskGen::generateRookMask(position.getPieces(), attackerP, side, onlyCaptures);
        pieceMaskToMoves(position.getPieces(), mask, attackerP, PIECE::ROOK, side, moves);
    }
    while (allQueens) {
        attackerP = BOp::bsf(allQueens);
        allQueens = BOp::set0(allQueens, attackerP);
        mask = PsLegalMoveMaskGen::generateQueenMask(position.getPieces(), attackerP, side, onlyCaptures);
        pieceMaskToMoves(position.getPieces(), mask, attackerP, PIECE::QUEEN, side, moves);
    }
    attackerP = BOp::bsf(position.getPieces().getPieceBitBoard(side, PIECE::KING));
    mask = PsLegalMoveMaskGen::generateKingMask(position.getPieces(), attackerP, side, onlyCaptures);
    pieceMaskToMoves(position.getPieces(), mask, attackerP, PIECE::KING, side, moves);

    addEnPassantCaptures(position.getPieces(), side, position.getEnPassant(), moves);
    if (!onlyCaptures) {
        if (side == SIDE::WHITE) {
            addCastlingMoves(position.getPieces(), SIDE::WHITE, position.getWLCastling(), position.getWSCastling(),moves);
        }
        else {
            addCastlingMoves(position.getPieces(), SIDE::BLACK, position.getBLCastling(), position.getBSCastling(),moves);
        }
    }

    return moves;
}

void LegalMoveGen::pieceMaskToMoves(Pieces pieces, Bitboard mask, uint8_t attackerP, uint8_t attackerType, uint8_t attackerSide, MoveList &moves) {
    while (mask) {
        uint8_t defenderP = BOp::bsf(mask);
        mask = BOp::set0(mask, defenderP);

        uint8_t defenderType = Move::NONE;
        for (uint8_t i = 0; i < 6; i = i + 1) {
            if (BOp::getBit(pieces.getPieceBitBoard(Pieces::inverse(attackerSide), i), defenderP)) {
                defenderType = i;
                break;
            }
        }

        Move move = {attackerP, defenderP, attackerType, attackerSide, defenderType, Pieces::inverse(attackerSide)};

        if (isLegal(pieces, move)) {
            moves.push_back(move);
        }
    }
}
void LegalMoveGen::pawnMaskToMoves(Pieces pieces, BitBoard mask, uint8_t attackerSide, uint8_t attacker_index, bool look_for_defender, uint8_t flag, MoveList& moves) {
    uint8_t defenderType = Move::NONE;

    while (mask) {
        uint8_t defenderP = BOp::bsf(mask);
        mask = BOp::set0(mask, defenderP);

        if (look_for_defender) {
            defenderType = Move::NONE;
            for (uint8_t i = 0; i < 6; i = i + 1) {
                if (BOp::getBit(pieces.getPieceBitBoard(Pieces::inverse(attackerSide), i), defenderP)) {
                    defenderType = i;
                    break;
                }
            }
        }

        Move move = {(uint8_t)(defenderP + attacker_index), defenderP, PIECE::PAWN, attackerSide, defenderType, Pieces::inverse(attackerSide), flag};

        if (isLegal(pieces, move)) {
            if (defenderP < 8 or defenderP > 55) {
                moves.push_back({(uint8_t)(defenderP + attacker_index), defenderP, 0, attackerSide, defenderType,Pieces::inverse(attackerSide), Move::FLAGS::PROMOTE_TO_KNIGHT});
                moves.push_back({(uint8_t)(defenderP + attacker_index), defenderP, 0, attackerSide, defenderType,Pieces::inverse(attackerSide), Move::FLAGS::PROMOTE_TO_BISHOP});
                moves.push_back({(uint8_t)(defenderP + attacker_index), defenderP, 0, attackerSide, defenderType,Pieces::inverse(attackerSide), Move::FLAGS::PROMOTE_TO_ROOK});
                moves.push_back({(uint8_t)(defenderP + attacker_index), defenderP, 0, attackerSide, defenderType,Pieces::inverse(attackerSide), Move::FLAGS::PROMOTE_TO_QUEEN});
            }
            else {
                moves.push_back(move);
            }
        }
    }
}
bool LegalMoveGen::isLegal(Pieces pieces, Move move) {
    pieces.setPieceBitBoard(move.getAttackerSide(), move.getAttackerType(), BOp::set0(pieces.getPieceBitBoard(move.getAttackerSide(), move.getAttackerType()), move.getFrom()));
    pieces.setPieceBitBoard(move.getAttackerSide(), move.getAttackerType(), BOp::set1(pieces.getPieceBitBoard(move.getAttackerSide(), move.getAttackerType()), move.getTo()));
    if (move.getDefenderType() != Move::NONE) {
        pieces.setPieceBitBoard(move.getDefenderSide(), move.getDefenderType(), BOp::set0(pieces.getPieceBitBoard(move.getDefenderSide(), move.getDefenderType()), move.getTo()));
    }
    if (move.getFlag() == Move::FLAGS::EN_PASSANT_CAPTURE) {
        if (move.getAttackerSide() == SIDE::WHITE) {
            pieces.setPieceBitBoard(SIDE::BLACK, PIECE::PAWN, BOp::set0(pieces.getPieceBitBoard(SIDE::BLACK, PIECE::PAWN), move.getTo() - 8));
        }
        else {
            pieces.setPieceBitBoard(SIDE::WHITE, PIECE::PAWN, BOp::set0(pieces.getPieceBitBoard(SIDE::WHITE, PIECE::PAWN), move.getTo() + 8));
        }
    }

    pieces.updateBitBoards();

    return !PsLegalMoveMaskGen::inDanger(pieces, BOp::bsf(pieces.getPieceBitBoard(move.getAttackerSide(), PIECE::KING)), move.getAttackerSide());
}
void LegalMoveGen::addEnPassantCaptures(Pieces pieces, uint8_t side, uint8_t enPassant, MoveList &moves) {
    if (enPassant == Position::NONE) {
        return;
    }

    if (side == SIDE::WHITE) {
        if (enPassant % 8 != 7 and BOp::getBit(pieces.getPieceBitBoard(SIDE::WHITE, PIECE::PAWN), enPassant - 7)) {
            auto move = Move((uint8_t)(enPassant - 7), enPassant, PIECE::PAWN, SIDE::WHITE, Move::NONE, Move::NONE, Move::FLAGS::EN_PASSANT_CAPTURE);
            if (isLegal(pieces, move)) {
                moves.push_back(move);
            }
        }
        if (enPassant % 8 != 0 and BOp::getBit(pieces.getPieceBitBoard(SIDE::WHITE, PIECE::PAWN), enPassant - 9)) {
            auto move = Move((uint8_t)(enPassant - 9), enPassant, PIECE::PAWN, SIDE::WHITE, Move::NONE, Move::NONE, Move::FLAGS::EN_PASSANT_CAPTURE);
            if (isLegal(pieces, move)) {
                moves.push_back(move);
            }
        }
    }
    else {
        if (enPassant % 8 != 0 and BOp::getBit(pieces.getPieceBitBoard(SIDE::BLACK, PIECE::PAWN), enPassant + 7)) {
            auto move = Move((uint8_t)(enPassant + 7), enPassant, PIECE::PAWN, SIDE::BLACK, Move::NONE, Move::NONE, Move::FLAGS::EN_PASSANT_CAPTURE);
            if (isLegal(pieces, move)) {
                moves.push_back(move);
            }
        }
        if (enPassant % 8 != 7 and BOp::getBit(pieces.getPieceBitBoard(SIDE::BLACK, PIECE::PAWN), enPassant + 9)) {
            auto move = Move((uint8_t)(enPassant + 9), enPassant, PIECE::PAWN, SIDE::BLACK, Move::NONE, Move::NONE, Move::FLAGS::EN_PASSANT_CAPTURE);
            if (isLegal(pieces, move)) {
                moves.push_back(move);
            }
        }
    }
}
void LegalMoveGen::addCastlingMoves(Pieces pieces, uint8_t side, bool lCastling, bool sCastling, MoveList &moves) {
    uint8_t index;
    uint8_t longCastlingFlag;
    uint8_t shortCastlingFlag;
    if (side == SIDE::WHITE) {
        index = 0;
        longCastlingFlag = Move::FLAGS::WL_CASTLING;
        shortCastlingFlag = Move::FLAGS::WS_CASTLING;
    }
    else {
        index = 56;
        longCastlingFlag = Move::FLAGS::BL_CASTLING;
        shortCastlingFlag = Move::FLAGS::BS_CASTLING;
    }

    if (lCastling and
        BOp::getBit(pieces.getPieceBitBoard(side, PIECE::ROOK), 0 + index) and
        BOp::getBit(pieces.getEmptyBitBoard(), 1 + index) and
        BOp::getBit(pieces.getEmptyBitBoard(), 2 + index) and
        BOp::getBit(pieces.getEmptyBitBoard(), 3 + index) and
        !PsLegalMoveMaskGen::inDanger(pieces, BOp::bsf(pieces.getPieceBitBoard(side, PIECE::KING)), side) and
        !PsLegalMoveMaskGen::inDanger(pieces, 2 + index, side) and
        !PsLegalMoveMaskGen::inDanger(pieces, 3 + index, side)) {

        moves.push_back({(uint8_t)(4 + index), (uint8_t)(2 + index), PIECE::KING, side, Move::NONE, Move::NONE, longCastlingFlag});
    }
    if (sCastling and
        BOp::getBit(pieces.getPieceBitBoard(side, PIECE::ROOK), 7 + index) and
        BOp::getBit(pieces.getEmptyBitBoard(), 5 + index) and
        BOp::getBit(pieces.getEmptyBitBoard(), 6 + index) and
        !PsLegalMoveMaskGen::inDanger(pieces, BOp::bsf(pieces.getPieceBitBoard(side, PIECE::KING)), side) and
        !PsLegalMoveMaskGen::inDanger(pieces, 5 + index, side) and
        !PsLegalMoveMaskGen::inDanger(pieces, 6 + index, side)) {

        moves.push_back({(uint8_t)(4 + index), (uint8_t)(6 + index), PIECE::KING, side, Move::NONE, Move::NONE, shortCastlingFlag});
    }
}


#undef Bitboard BitBoard
#undef PsLegalMoveMaskGen PsLegalMoveGen
#undef BOp BitBoardOperations
