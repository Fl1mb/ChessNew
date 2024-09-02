#include "src/ChessAI/headers/StaticEvaluator.h"


int32_t StaticEvaluator::evaluate(Pieces pieces)
{
    int32_t evaluation = 0;

    int32_t materialEvaluation = material(pieces);
    int32_t mobilityEvaluation = mobility(pieces);
    int32_t doublePawnEvaluation = doublePawn(pieces);
    int32_t connectedPawnEvaluation = connectedPawn(pieces);
    int32_t pawnPromotionEvaluation = pawnPromotion(pieces);
    int32_t kingSafetyEvaluation = kingSafety(pieces);
    int32_t endGameEvaluation = endgame(pieces, materialEvaluation >= 0);

    evaluation += materialEvaluation;
    evaluation += mobilityEvaluation;
    evaluation += doublePawnEvaluation;
    evaluation += connectedPawnEvaluation;
    evaluation += pawnPromotionEvaluation;
    evaluation += kingSafetyEvaluation;
    evaluation += endGameEvaluation;

    return evaluation;
}

int32_t StaticEvaluator::material(Pieces pieces)
{
    int32_t material_ = 0;
    material_ = material_ + MATERIAL::PAWN * (BitBoardOperations::count_1(pieces.getPieceBitBoard(WHITE,PAWN)) - BitBoardOperations::count_1(pieces.getPieceBitBoard(BLACK, PAWN)));
    material_ = material_ + MATERIAL::KNIGHT * (BitBoardOperations::count_1(pieces.getPieceBitBoard(WHITE,KNIGHT)) - BitBoardOperations::count_1(pieces.getPieceBitBoard(BLACK, KNIGHT)));
    material_ = material_ + MATERIAL::BISHOP * (BitBoardOperations::count_1(pieces.getPieceBitBoard(WHITE,BISHOP)) - BitBoardOperations::count_1(pieces.getPieceBitBoard(BLACK, BISHOP)));
    material_ = material_ + MATERIAL::ROOK * (BitBoardOperations::count_1(pieces.getPieceBitBoard(WHITE,ROOK)) - BitBoardOperations::count_1(pieces.getPieceBitBoard(BLACK, ROOK)));
    material_ = material_ + MATERIAL::QUEEN * (BitBoardOperations::count_1(pieces.getPieceBitBoard(WHITE,QUEEN)) - BitBoardOperations::count_1(pieces.getPieceBitBoard(BLACK, QUEEN)));

    return material_;

}

int32_t StaticEvaluator::mobility(Pieces pieces)
{
    int32_t mobility_ = 0;

    std::array<std::array<BitBoard,6>,2> masks = pieces.getPieceBitBoards();
    int32_t knightMoves = 0;
    int32_t bishopMoves = 0;
    int32_t rookMoves = 0;
    int32_t queenMoves = 0;

    BitBoard whitePawnsLeftAttacks = PsLegalMoveGen::generatePawnLeftCaptureMask(pieces, WHITE, true);
    BitBoard whitePawnsRightAttacks = PsLegalMoveGen::generatePawnRightCaptureMask(pieces, SIDE::WHITE, true);
    BitBoard whitePawnsAttacks = whitePawnsLeftAttacks | whitePawnsRightAttacks;
    BitBoard safeForBlack = ~whitePawnsAttacks;

    BitBoard blackPawnsLeftAttacks = PsLegalMoveGen::generatePawnLeftCaptureMask(pieces, BLACK, true);
    BitBoard blackPawnsRightAttacks = PsLegalMoveGen::generatePawnRightCaptureMask(pieces, SIDE::BLACK, true);
    BitBoard blackPawnsAttacks = blackPawnsLeftAttacks | blackPawnsRightAttacks;
    BitBoard safeForWhite = ~blackPawnsAttacks;

    while(masks[WHITE][KNIGHT]){
        uint8_t index = BitBoardOperations::bsf(masks[WHITE][KNIGHT]);
        masks[WHITE][KNIGHT] = BitBoardOperations::set0(masks[WHITE][KNIGHT], index);
        knightMoves = knightMoves + BitBoardOperations::count_1(PsLegalMoveGen::generateKnightMask(pieces, index, WHITE, false) & safeForWhite);
    }

    while(masks[WHITE][BISHOP]){
        uint8_t index = BitBoardOperations::bsf(masks[WHITE][BISHOP]);
        masks[WHITE][BISHOP] = BitBoardOperations::set0(masks[WHITE][BISHOP], index);
        bishopMoves = bishopMoves + BitBoardOperations::count_1(PsLegalMoveGen::generateBishopMask(pieces, index, WHITE, false) & safeForWhite);
    }

    while(masks[WHITE][ROOK]){
        uint8_t index = BitBoardOperations::bsf(masks[WHITE][ROOK]);
        masks[WHITE][ROOK] = BitBoardOperations::set0(masks[WHITE][ROOK], index);
        rookMoves +=  BitBoardOperations::count_1(PsLegalMoveGen::generateRookMask(pieces, index, WHITE, false) & safeForWhite);
    }
    while(masks[WHITE][QUEEN]){
        uint8_t index = BitBoardOperations::bsf(masks[WHITE][QUEEN]);
        masks[WHITE][QUEEN] = BitBoardOperations::set0(masks[WHITE][QUEEN], index);
        queenMoves += BitBoardOperations::count_1(PsLegalMoveGen::generateQueenMask(pieces, index, WHITE, false) & safeForWhite);
    }

    //
    while(masks[BLACK][KNIGHT]){
        uint8_t index = BitBoardOperations::bsf(masks[BLACK][KNIGHT]);
        masks[BLACK][KNIGHT] = BitBoardOperations::set0(masks[BLACK][KNIGHT], index);
        knightMoves = knightMoves + BitBoardOperations::count_1(PsLegalMoveGen::generateKnightMask(pieces, index, BLACK, false) & safeForBlack);
    }

    while(masks[BLACK][BISHOP]){
        uint8_t index = BitBoardOperations::bsf(masks[BLACK][BISHOP]);
        masks[BLACK][BISHOP] = BitBoardOperations::set0(masks[BLACK][BISHOP], index);
        bishopMoves = bishopMoves + BitBoardOperations::count_1(PsLegalMoveGen::generateBishopMask(pieces, index, BLACK, false) & safeForBlack);
    }

    while(masks[BLACK][ROOK]){
        uint8_t index = BitBoardOperations::bsf(masks[BLACK][ROOK]);
        masks[BLACK][ROOK] = BitBoardOperations::set0(masks[BLACK][ROOK], index);
        rookMoves +=  BitBoardOperations::count_1(PsLegalMoveGen::generateRookMask(pieces, index, BLACK, false) & safeForBlack);
    }
    while(masks[BLACK][QUEEN]){
        uint8_t index = BitBoardOperations::bsf(masks[BLACK][QUEEN]);
        masks[BLACK][QUEEN] = BitBoardOperations::set0(masks[BLACK][QUEEN], index);
        queenMoves += BitBoardOperations::count_1(PsLegalMoveGen::generateQueenMask(pieces, index, BLACK, false) & safeForBlack);
    }

    mobility_ = mobility_ + MOBILITY::KNIGHT * knightMoves;
    mobility_ = mobility_ + MOBILITY::BISHOP * bishopMoves;
    mobility_ = mobility_ + MOBILITY::ROOK * rookMoves;
    mobility_ = mobility_ + MOBILITY::QUEEN * queenMoves;

    return mobility_;

}

int32_t StaticEvaluator::doublePawn(Pieces pieces)
{
    int32_t doublePawnNum = 0;

    for(uint8_t x = 0; x < 8; x++){
        uint8_t whitePawn = BitBoardOperations::count_1(pieces.getPieceBitBoard(WHITE, PAWN) & BColumns::COLUMNS[x]);
        uint8_t blackPawn = BitBoardOperations::count_1(pieces.getPieceBitBoard(BLACK, PAWN) & BColumns::COLUMNS[x]);

        doublePawnNum = doublePawnNum + std::max(0, whitePawn - 1);
        doublePawnNum = doublePawnNum - std::max(0, blackPawn - 1);
    }

    return PAWN_STRUCTURE::DOUBLE_PAWN * doublePawnNum;
}

int32_t StaticEvaluator::connectedPawn(Pieces pieces)
{
    int32_t connected_pawn_ctr = 0;

    BitBoard white_captures = PsLegalMoveGen::generatePawnLeftCaptureMask(pieces, WHITE, true) | PsLegalMoveGen::generatePawnRightCaptureMask(pieces, WHITE, true);
    BitBoard black_captures = PsLegalMoveGen::generatePawnLeftCaptureMask(pieces, BLACK, true) | PsLegalMoveGen::generatePawnRightCaptureMask(pieces, BLACK, true);

    connected_pawn_ctr += BitBoardOperations::count_1(white_captures & pieces.getPieceBitBoard(WHITE, PAWN));
    connected_pawn_ctr -= BitBoardOperations::count_1(black_captures & pieces.getPieceBitBoard(BLACK, PAWN));

    return PAWN_STRUCTURE::CONNECTED_PAWN * connected_pawn_ctr;
}

int32_t StaticEvaluator::pawnPromotion(Pieces pieces)
{
    int32_t pawnPromotion = 0;

    BitBoard whitePawns = pieces.getPieceBitBoard(SIDE::WHITE, PIECE::PAWN);
    BitBoard blackPawns = pieces.getPieceBitBoard(SIDE::BLACK, PIECE::PAWN);

    while (whitePawns) {
        uint8_t index = BitBoardOperations::bsf(whitePawns);
        whitePawns = BitBoardOperations::set0(whitePawns, index);
        if (PassedPawnMasks::WHITE_PASSED_PAWN_MASKS[index] & pieces.getPieceBitBoard(SIDE::BLACK, PIECE::PAWN)) {
            pawnPromotion = pawnPromotion + PAWN_STRUCTURE::DEFAULT_PAWN_PROMOTION[index / 8];
        }
        else {
            pawnPromotion = pawnPromotion + PAWN_STRUCTURE::PASSED_PAWN_PROMOTION[index / 8];
        }
    }
    while (blackPawns) {
        uint8_t index = BitBoardOperations::bsf(blackPawns);
        blackPawns = BitBoardOperations::set0(blackPawns, index);
        if (PassedPawnMasks::BLACK_PASSED_PAWN_MASKS[index] & pieces.getPieceBitBoard(SIDE::WHITE, PIECE::PAWN)) {
            pawnPromotion = pawnPromotion - PAWN_STRUCTURE::DEFAULT_PAWN_PROMOTION[7 - index / 8];
        }
        else {
            pawnPromotion = pawnPromotion - PAWN_STRUCTURE::PASSED_PAWN_PROMOTION[7 - index / 8];
        }
    }

    return pawnPromotion;
}

int32_t StaticEvaluator::kingSafety(Pieces pieces)
{
    int32_t kingSafety = 0;

    if (BitBoardOperations::count_1(pieces.getAllBitBoard()) <= ENDGAME::MAXIMUM_PIECES_FOR_ENDGAME) {
        return kingSafety;
    }

    uint8_t whiteKingP = BitBoardOperations::bsf(pieces.getPieceBitBoard(SIDE::WHITE, PIECE::KING));
    uint8_t blackKingP = BitBoardOperations::bsf(pieces.getPieceBitBoard(SIDE::BLACK, PIECE::KING));

    BitBoard whiteKingArea = KingMasks::MASKS[whiteKingP];
    BitBoard blackKingArea = KingMasks::MASKS[blackKingP];

    std::array<std::array<BitBoard, 6>, 2> masks = pieces.getPieceBitBoards();
    int32_t knightMoves = 0;
    int32_t bishopMoves = 0;
    int32_t rookMoves = 0;
    int32_t queenMoves = 0;

    while (masks[SIDE::WHITE][PIECE::KNIGHT]) {
        uint8_t index = BitBoardOperations::bsf(masks[SIDE::WHITE][PIECE::KNIGHT]);
        masks[SIDE::WHITE][PIECE::KNIGHT] = BitBoardOperations::set0(masks[SIDE::WHITE][PIECE::KNIGHT], index);
        knightMoves = knightMoves + BitBoardOperations::count_1(PsLegalMoveGen::generateKnightMask(pieces, index, SIDE::WHITE, false) & blackKingArea);
    }
    while (masks[SIDE::WHITE][PIECE::BISHOP]) {
        uint8_t index = BitBoardOperations::bsf(masks[SIDE::WHITE][PIECE::BISHOP]);
        masks[SIDE::WHITE][PIECE::BISHOP] = BitBoardOperations::set0(masks[SIDE::WHITE][PIECE::BISHOP], index);
        bishopMoves = bishopMoves + BitBoardOperations::count_1(PsLegalMoveGen::generateBishopMask(pieces, index, SIDE::WHITE, false) & blackKingArea);
    }
    while (masks[SIDE::WHITE][PIECE::ROOK]) {
        uint8_t index = BitBoardOperations::bsf(masks[SIDE::WHITE][PIECE::ROOK]);
        masks[SIDE::WHITE][PIECE::ROOK] = BitBoardOperations::set0(masks[SIDE::WHITE][PIECE::ROOK], index);
        rookMoves = rookMoves + BitBoardOperations::count_1(PsLegalMoveGen::generateRookMask(pieces, index, SIDE::WHITE, false) & blackKingArea);
    }
    while (masks[SIDE::WHITE][PIECE::QUEEN]) {
        uint8_t index = BitBoardOperations::bsf(masks[SIDE::WHITE][PIECE::QUEEN]);
        masks[SIDE::WHITE][PIECE::QUEEN] = BitBoardOperations::set0(masks[SIDE::WHITE][PIECE::QUEEN], index);
        queenMoves = queenMoves + BitBoardOperations::count_1(PsLegalMoveGen::generateQueenMask(pieces, index, SIDE::WHITE, false) & blackKingArea);
    }
    while (masks[SIDE::BLACK][PIECE::KNIGHT]) {
        uint8_t index = BitBoardOperations::bsf(masks[SIDE::BLACK][PIECE::KNIGHT]);
        masks[SIDE::BLACK][PIECE::KNIGHT] = BitBoardOperations::set0(masks[SIDE::BLACK][PIECE::KNIGHT], index);
        knightMoves = knightMoves - BitBoardOperations::count_1(PsLegalMoveGen::generateKnightMask(pieces, index, SIDE::BLACK, false) & whiteKingArea);
    }
    while (masks[SIDE::BLACK][PIECE::BISHOP]) {
        uint8_t index = BitBoardOperations::bsf(masks[SIDE::BLACK][PIECE::BISHOP]);
        masks[SIDE::BLACK][PIECE::BISHOP] = BitBoardOperations::set0(masks[SIDE::BLACK][PIECE::BISHOP], index);
        bishopMoves = bishopMoves - BitBoardOperations::count_1(PsLegalMoveGen::generateBishopMask(pieces, index, SIDE::BLACK, false) & whiteKingArea);
    }
    while (masks[SIDE::BLACK][PIECE::ROOK]) {
        uint8_t index = BitBoardOperations::bsf(masks[SIDE::BLACK][PIECE::ROOK]);
        masks[SIDE::BLACK][PIECE::ROOK] = BitBoardOperations::set0(masks[SIDE::BLACK][PIECE::ROOK], index);
        rookMoves = rookMoves - BitBoardOperations::count_1(PsLegalMoveGen::generateRookMask(pieces, index, SIDE::BLACK, false) & whiteKingArea);
    }
    while (masks[SIDE::BLACK][PIECE::QUEEN]) {
        uint8_t index = BitBoardOperations::bsf(masks[SIDE::BLACK][PIECE::QUEEN]);
        masks[SIDE::BLACK][PIECE::QUEEN] = BitBoardOperations::set0(masks[SIDE::BLACK][PIECE::QUEEN], index);
        queenMoves = queenMoves - BitBoardOperations::count_1(PsLegalMoveGen::generateQueenMask(pieces, index, SIDE::BLACK, false) & whiteKingArea);
    }

    kingSafety = kingSafety + KING_SAFETY::KNIGHT * knightMoves;
    kingSafety = kingSafety + KING_SAFETY::BISHOP * bishopMoves;
    kingSafety = kingSafety + KING_SAFETY::ROOK * rookMoves;
    kingSafety = kingSafety + KING_SAFETY::QUEEN * queenMoves;

    return kingSafety;
}

int32_t StaticEvaluator::endgame(Pieces pieces, bool whiteStronger)
{
    int32_t endgame = 0;

    if (BitBoardOperations::count_1(pieces.getAllBitBoard()) > ENDGAME::MAXIMUM_PIECES_FOR_ENDGAME) {
        return endgame;
    }

    uint8_t attackerSide;
    uint8_t defenderSide;
    if (whiteStronger) {
        attackerSide = SIDE::WHITE;
        defenderSide = SIDE::BLACK;
    }
    else {
        attackerSide = SIDE::BLACK;
        defenderSide = SIDE::WHITE;
    }

    uint8_t attackerKingP = BitBoardOperations::bsf(pieces.getPieceBitBoard(attackerSide, PIECE::KING));
    int8_t attackerKingX = attackerKingP % 8;
    int8_t attackerKingY = attackerKingP / 8;

    uint8_t defenderKingP = BitBoardOperations::bsf(pieces.getPieceBitBoard(defenderSide, PIECE::KING));
    int8_t defenderKingX = defenderKingP % 8;
    int8_t defenderKingY = defenderKingP / 8;

    endgame = endgame + ENDGAME::PROXIMITY_KINGS * (16 - std::abs(attackerKingX - defenderKingX) - std::abs(attackerKingY - defenderKingY));
    endgame = endgame + ENDGAME::DISTANCE_WEAK_KING_MIDDLE * (std::abs(defenderKingX - 3) + std::abs(defenderKingY - 4));

    if (!whiteStronger) {
        endgame = -endgame;
    }

    return endgame;
}
