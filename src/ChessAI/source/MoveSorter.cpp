#include "src/ChessAI/headers/MoveSorter.h"


MoveList MoveSorter::sort(Pieces pieces, MoveList moves)
{
    for(uint8_t i = 0; i < moves.size(); i++){
        for(uint8_t j = 0; j < moves.size() - i - 1; j++){
            if(MoveSorter::evaluateMove(pieces, moves[j]) <  MoveSorter::evaluateMove(pieces, moves[j + 1])){
                std::swap(moves[j], moves[j + 1]);
            }
        }
    }
    return moves;
}

int32_t MoveSorter::evaluateMove(Pieces pieces, Move move)
{
    int32_t evaluation = 0;

    if (move.getAttackerType() != PIECE::PAWN) {
        BitBoard opponentPawnsAttacks = PsLegalMoveGen::generatePawnLeftCaptureMask(pieces, Pieces::inverse(move.getAttackerSide()), true) |
                                        PsLegalMoveGen::generatePawnRightCaptureMask(pieces, Pieces::inverse(move.getAttackerSide()), true);
        if (BitBoardOperations::getBit(opponentPawnsAttacks, move.getTo())) {
            switch (move.getAttackerType()) {
                case PIECE::KNIGHT:
                    evaluation = evaluation - StaticEvaluator::MATERIAL::KNIGHT;
                    break;
                case PIECE::BISHOP:
                    evaluation = evaluation - StaticEvaluator::MATERIAL::BISHOP;
                    break;
                case PIECE::ROOK:
                    evaluation = evaluation - StaticEvaluator::MATERIAL::ROOK;
                    break;
                case PIECE::QUEEN:
                    evaluation = evaluation - StaticEvaluator::MATERIAL::QUEEN;
                    break;
            }
        }
    }

    if (move.getDefenderType() != Move::NONE) {
        switch (move.getDefenderType()) {
            case PIECE::PAWN:
                evaluation = evaluation + 1000 * StaticEvaluator::MATERIAL::PAWN;
                break;
            case PIECE::KNIGHT:
                evaluation = evaluation + 1000 * StaticEvaluator::MATERIAL::KNIGHT;
                break;
            case PIECE::BISHOP:
                evaluation = evaluation + 1000 * StaticEvaluator::MATERIAL::BISHOP;
                break;
            case PIECE::ROOK:
                evaluation = evaluation + 1000 * StaticEvaluator::MATERIAL::ROOK;
                break;
            case PIECE::QUEEN:
                evaluation = evaluation + 1000 * StaticEvaluator::MATERIAL::QUEEN;
                break;
        }
        switch (move.getAttackerType()) {
            case PIECE::PAWN:
                evaluation = evaluation - StaticEvaluator::MATERIAL::PAWN;
                break;
            case PIECE::KNIGHT:
                evaluation = evaluation - StaticEvaluator::MATERIAL::KNIGHT;
                break;
            case PIECE::BISHOP:
                evaluation = evaluation - StaticEvaluator::MATERIAL::BISHOP;
                break;
            case PIECE::ROOK:
                evaluation = evaluation - StaticEvaluator::MATERIAL::ROOK;
                break;
            case PIECE::QUEEN:
                evaluation = evaluation - StaticEvaluator::MATERIAL::QUEEN;
                break;
        }
    }

    return evaluation;
}
