#include "src/ChessAI/headers/AI.h"
#define nsecs std::chrono::high_resolution_clock::now().time_since_epoch().count()

OpeningBook* AI::book = nullptr;

void AI::setOpeningBook(const std::string &path)
{
    if(book == nullptr)
        book = new OpeningBook(path);
}

Move AI::getBestMove(const Position& position, uint8_t side, int32_t ms) {
    uint32_t evaluatuion = StaticEvaluator::evaluate(position.getPieces());
    int64_t start = nsecs;
    SearchInterrupter::getPtr()->resume();

    int32_t eval;
    bool gameWasFinished;
    Move move;

    std::tuple<int32_t, bool, Move> opening_book_result = book->TryToFindMove(position);
    if(std::get<1>(opening_book_result)){
        return std::get<2>(opening_book_result);
    }

    for (int32_t i = 1; i < 1000; i = i + 1) {
        std::future<std::tuple<int32_t, bool, Move>> thread = std::async(alphaBeta, position, side, i);
        bool continueSearch = true;
        while (thread.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
            if ((nsecs - start) / 1000000 >= ms) {
                continueSearch = false;
                break;
            }
        }

        if (continueSearch or i == 1) {
            std::tie(eval, gameWasFinished, move) = thread.get();
        }
        else {
            SearchInterrupter::getPtr()->interrupt();
            std::tuple<int32_t, bool, Move> trash = thread.get();
            break;
        }
        if (gameWasFinished) {
            break;
        }
    }
    return move;
}
std::tuple<int32_t, bool, Move> AI::alphaBeta(const Position& position, uint8_t side, int32_t depthLeft) {
    if (side == SIDE::WHITE) {
        return alphaBetaMax(position, INF::NEGATIVE, INF::POSITIVE, depthLeft);
    }
    return alphaBetaMin(position, INF::NEGATIVE, INF::POSITIVE, depthLeft);
}
std::tuple<int32_t, bool, Move> AI::alphaBetaMin(const Position &position, int32_t alpha, int32_t beta, int32_t depthLeft, int32_t depthCurrent) {
    if (SearchInterrupter::getPtr()->interrupted()) {
        return std::make_tuple(0, false, Move());
    }
    if (depthLeft == 0) {
        return std::make_tuple(alphaBetaMinOnlyCaptures(position, alpha, beta), false, Move());
    }
    if (position.fiftyMovesRuleDraw() or position.threefoldRepetitionDraw()) {
        return std::make_tuple(0, true, Move());
    }

    MoveList moves = LegalMoveGen::generate(position, SIDE::BLACK);
    moves = MoveSorter::sort(position.getPieces(), moves);
    Move bestMove;
    uint8_t bestMoveIndex;
    bool gameWasFinishedOnBestMove;

    uint8_t tableResult = TranspositionTable::getPtr()->tryToFindBestMoveIndex(position.getHash());
    if (tableResult < moves.size()) {
        std::swap(moves[0], moves[tableResult]);
    }

    bool check = PsLegalMoveGen::inDanger(position.getPieces(), BitBoardOperations::bsf(position.getPieces().getPieceBitBoard(SIDE::BLACK, PIECE::KING)), SIDE::BLACK);
    if (moves.size() == 0) {
        if (check) {
            return std::make_tuple(INF::POSITIVE - depthLeft, true, Move());
        }
        return std::make_tuple(0, true, Move());
    }

    for (uint8_t i = 0; i < moves.size(); i = i + 1) {
        Move move = moves[i];

        Position copy = position;
        copy.move(move);
        std::tuple<int32_t, bool, Move> a = alphaBetaMax(copy, alpha, beta, depthLeft - !check, depthCurrent + 1);
        int32_t evaluation = std::get<0>(a);
        bool gameWasFinished = std::get<1>(a);

        if (evaluation <= alpha) {
            TranspositionTable::getPtr()->addEntry(position.getHash(), depthCurrent, bestMoveIndex);
            return std::make_tuple(alpha, gameWasFinishedOnBestMove, bestMove);
        }
        if (evaluation < beta) {
            bestMove = move;
            bestMoveIndex = i;
            gameWasFinishedOnBestMove = gameWasFinished;
            beta = evaluation;
        }
    }

    TranspositionTable::getPtr()->addEntry(position.getHash(), depthCurrent, bestMoveIndex);
    return std::make_tuple(beta, gameWasFinishedOnBestMove, bestMove);
}
std::tuple<int32_t, bool, Move> AI::alphaBetaMax(const Position &position, int32_t alpha, int32_t beta, int32_t depthLeft, int32_t depthCurrent) {
    if (SearchInterrupter::getPtr()->interrupted()) {
        return std::make_tuple(0, false, Move());
    }
    if (depthLeft == 0) {
        return std::make_tuple(alphaBetaMaxOnlyCaptures(position, alpha, beta), false, Move());
    }
    if (position.fiftyMovesRuleDraw() or position.threefoldRepetitionDraw()) {
        return std::make_tuple(0, true, Move());
    }

    MoveList moves = LegalMoveGen::generate(position, SIDE::WHITE);
    moves = MoveSorter::sort(position.getPieces(), moves);
    Move bestMove;
    uint8_t bestMoveIndex;
    bool gameWasFinishedOnBestMove;

    uint8_t tableResult = TranspositionTable::getPtr()->tryToFindBestMoveIndex(position.getHash());
    if (tableResult < moves.size()) {
        std::swap(moves[0], moves[tableResult]);
    }

    bool check = PsLegalMoveGen::inDanger(position.getPieces(), BitBoardOperations::bsf(position.getPieces().getPieceBitBoard(SIDE::WHITE, PIECE::KING)), SIDE::WHITE);
    if (moves.size() == 0) {
        if (check) {
            return std::make_tuple(INF::NEGATIVE + depthLeft, true, Move());
        }
        return std::make_tuple(0, true, Move());
    }

    for (uint8_t i = 0; i < moves.size(); i = i + 1) {
        Move move = moves[i];

        Position copy = position;
        copy.move(move);
        std::tuple<int32_t, bool, Move> a = alphaBetaMin(copy, alpha, beta, depthLeft - !check, depthCurrent + 1);
        int32_t evaluation = std::get<0>(a);
        bool gameWasFinished = std::get<1>(a);

        if (evaluation >= beta) {
            TranspositionTable::getPtr()->addEntry(position.getHash(), depthCurrent, bestMoveIndex);
            return std::make_tuple(beta, gameWasFinishedOnBestMove, bestMove);
        }
        if (evaluation > alpha) {
            bestMove = move;
            bestMoveIndex = i;
            gameWasFinishedOnBestMove = gameWasFinished;
            alpha = evaluation;
        }
    }

    TranspositionTable::getPtr()->addEntry(position.getHash(), depthCurrent, bestMoveIndex);
    return std::make_tuple(alpha, gameWasFinishedOnBestMove, bestMove);
}
int32_t AI::alphaBetaMinOnlyCaptures(const Position& position, int32_t alpha, int32_t beta) {
    if (SearchInterrupter::getPtr()->interrupted()) {
        return 0;
    }

    int32_t evaluation = StaticEvaluator::evaluate(position.getPieces());
    if (evaluation <= alpha) {
        return alpha;
    }
    if (evaluation < beta) {
        beta = evaluation;
    }

    MoveList moves = LegalMoveGen::generate(position, SIDE::BLACK, true);
    moves = MoveSorter::sort(position.getPieces(), moves);

    for (uint8_t i = 0; i < moves.size(); i = i + 1) {
        Move move = moves[i];

        Position copy = position;
        copy.move(move);
        evaluation = alphaBetaMaxOnlyCaptures(copy, alpha, beta);

        if (evaluation <= alpha) {
            return alpha;
        }
        if (evaluation < beta) {
            beta = evaluation;
        }
    }

    return beta;
}
int32_t AI::alphaBetaMaxOnlyCaptures(const Position& position, int32_t alpha, int32_t beta) {
    if (SearchInterrupter::getPtr()->interrupted()) {
        return 0;
    }

    int32_t evaluation = StaticEvaluator::evaluate(position.getPieces());
    if (evaluation >= beta) {
        return beta;
    }
    if (evaluation > alpha) {
        alpha = evaluation;
    }

    MoveList moves = LegalMoveGen::generate(position, SIDE::WHITE, true);
    moves = MoveSorter::sort(position.getPieces(), moves);

    for (uint8_t i = 0; i < moves.size(); i = i + 1) {
        Move move = moves[i];

        Position copy = position;
        copy.move(move);
        evaluation = alphaBetaMinOnlyCaptures(copy, alpha, beta);

        if (evaluation >= beta) {
            return beta;
        }
        if (evaluation > alpha) {
            alpha = evaluation;
        }
    }

    return alpha;
}
