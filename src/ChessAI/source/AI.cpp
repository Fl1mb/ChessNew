#include "src/ChessAI/headers/AI.h"


AI::AI() = default;
AI::AI(const std::string &opening_book_path)
{
    this->opening_book = {opening_book_path};
}

Move AI::getBestMove(const Position &position, uint8_t side, int32_t ms)
{
    StaticEvaluator::evaluate(position.getPieces());
    const auto nsecs = std::chrono::high_resolution_clock::now().time_since_epoch().count();

    int64_t start = nsecs;
    SearchInterrupter::getPtr()->resume();

    int32_t eval;
    bool gameWasFinished;
    Move move;

    for(int32_t i = 1; i < 1000; i++){
        std::future<std::tuple<int32_t, bool, Move>> thread =  std::async(AlphaBeta, position, side, i);
        bool continueSearch = true;
        while(thread.wait_for(std::chrono::seconds(0)) != std::future_status::ready){
            if ((nsecs - start) / 1000000 >= ms) {
                continueSearch = false;
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));

            if(continueSearch or i == 1){
                std::tie(eval, gameWasFinished, move) = thread.get();
            }
            else{
                SearchInterrupter::getPtr()->interrupt();
                std::tuple<int32_t, bool, Move> trash = thread.get();
                break;
            }
            if(gameWasFinished)
                break;

        }
    }
    return move;
}

std::tuple<int32_t, bool, Move> AI::AlphaBeta(const Position &position, uint8_t side, int32_t depthLeft)
{
    if(side == WHITE){
        return alphaBetaMax(position, INF::negative, INF::positive, depthLeft);
    }else{
        return alphaBetaMin(position, INF::negative, INF::positive, depthLeft);
    }
}

std::tuple<int32_t, bool, Move> AI::alphaBetaMin(const Position &position, int32_t alpha, int32_t beta, int32_t depthLeft, int32_t depthCurrent)
{
    if(SearchInterrupter::getPtr()->interrupted()){
        return std::make_tuple(0, false, Move());
    }
    if(depthCurrent == 0){
        return std::make_tuple(alphaBetaMinOnlyCaptures(position, alpha, beta), false, Move());
    }
    if(position.fiftyMovesRuleDraw() or position.threefoldRepetitionDraw()){
        return std::make_tuple(0, true, Move());
    }

    MoveList moves = LegalMoveGen::generate(position, BLACK);
    moves = MoveSorter::sort(position.getPieces(), moves);
    Move bestMove;
    uint8_t bestMoveIndex;
    bool gameWasFinishedOnBestMove;

    uint8_t tableResult = TranspositionTable::getPtr()->tryToFindBestMoveIndex(position.getHash());

    if(tableResult < moves.size()){
        std::swap(moves[0], moves[tableResult]);
    }

    bool check = PsLegalMoveGen::inDanger(position.getPieces(), BitBoardOperations::bsf(position.getPieces().getPieceBitBoard(BLACK, KING)), BLACK);

    if(moves.size() == 0){
        if(check){
            return std::make_tuple(INF::positive - depthLeft, true, Move());
        }
        return std::make_tuple(0, true, Move());
    }

    for(uint8_t i = 0 ; i < moves.size(); i++){
        Move move = moves[i];

        Position copy = position;
        copy.move(move);

        std::tuple<int32_t, bool, Move> a = alphaBetaMax(position, alpha, beta, depthLeft - !check, depthCurrent + 1);
        int32_t evaluation = std::get<0>(a);
        bool gameWasFinished = std::get<1>(a);

        if(evaluation <= alpha){
            TranspositionTable::getPtr()->addEntry(position.getHash(), depthCurrent, bestMoveIndex);
            return std::make_tuple(alpha, gameWasFinishedOnBestMove, bestMove);
        }
        if(evaluation < beta){
            bestMove = move;
            bestMoveIndex = i;
            gameWasFinishedOnBestMove = gameWasFinished;
            beta = evaluation;
        }
    }

    TranspositionTable::getPtr()->addEntry(position.getHash(), depthCurrent, bestMoveIndex);
    return std::make_tuple(beta, gameWasFinishedOnBestMove, bestMove);
}

std::tuple<int32_t, bool, Move> AI::alphaBetaMax(const Position &position, int32_t alpha, int32_t beta, int32_t depthLeft, int32_t depthCurrent)
{
    if(SearchInterrupter::getPtr()->interrupted()){
        return std::make_tuple(0, false, Move());
    }
    if(depthCurrent == 0){
        return std::make_tuple(alphaBetaMinOnlyCaptures(position, alpha, beta), false, Move());
    }
    if(position.fiftyMovesRuleDraw() or position.threefoldRepetitionDraw()){
        return std::make_tuple(0, true, Move());
    }

    MoveList moves = LegalMoveGen::generate(position, WHITE);
    moves = MoveSorter::sort(position.getPieces(), moves);
    Move bestMove;
    uint8_t bestMoveIndex;
    bool gameWasFinishedOnBestMove;

    uint8_t tableResult = TranspositionTable::getPtr()->tryToFindBestMoveIndex(position.getHash());

    if(tableResult < moves.size()){
        std::swap(moves[0], moves[tableResult]);
    }

    bool check = PsLegalMoveGen::inDanger(position.getPieces(), BitBoardOperations::bsf(position.getPieces().getPieceBitBoard(WHITE, KING)), WHITE);

    if(moves.size() == 0){
        if(check){
            return std::make_tuple(INF::negative + depthLeft, true, Move());
        }
        return std::make_tuple(0, true, Move());
    }

    for(uint8_t i = 0 ; i < moves.size(); i++){
        Move move = moves[i];

        Position copy = position;
        copy.move(move);

        std::tuple<int32_t, bool, Move> a = alphaBetaMin(position, alpha, beta, depthLeft - !check, depthCurrent + 1);
        int32_t evaluation = std::get<0>(a);
        bool gameWasFinished = std::get<1>(a);

        if(evaluation >= beta){
            TranspositionTable::getPtr()->addEntry(position.getHash(), depthCurrent, bestMoveIndex);
            return std::make_tuple(beta, gameWasFinishedOnBestMove, bestMove);
        }
        if(evaluation > alpha){
            bestMove = move;
            bestMoveIndex = i;
            gameWasFinishedOnBestMove = gameWasFinished;
            alpha = evaluation;
        }
    }

    TranspositionTable::getPtr()->addEntry(position.getHash(), depthCurrent, bestMoveIndex);
    return std::make_tuple(alpha, gameWasFinishedOnBestMove, bestMove);
}

int32_t AI::alphaBetaMinOnlyCaptures(const Position &position, int32_t alpha, int32_t beta)
{
    if(SearchInterrupter::getPtr()->interrupted()){
        return 0;
    }
    int32_t evaluation = StaticEvaluator::evaluate(position.getPieces());
    if(evaluation <= alpha)
        return alpha;
    if(evaluation < beta)
        return evaluation;

    MoveList moves = LegalMoveGen::generate(position, BLACK, true);
    moves = MoveSorter::sort(position.getPieces(), moves);

    for(uint8_t i = 0; i < moves.size(); i++){
        Move move = moves[i];

        Position copy = position;
        copy.move(move);

        evaluation = alphaBetaMaxOnlyCaptures(copy, alpha, beta);
        if(evaluation <= alpha)
            return alpha;
        if(evaluation < beta)
            return evaluation;
    }

    return beta;

}

int32_t AI::alphaBetaMaxOnlyCaptures(const Position &position, int32_t alpha, int32_t beta)
{
    if(SearchInterrupter::getPtr()->interrupted()){
        return 0;
    }
    int32_t evaluation = StaticEvaluator::evaluate(position.getPieces());
    if(evaluation >= beta)
        return beta;
    if(evaluation > alpha)
        return evaluation;

    MoveList moves = LegalMoveGen::generate(position, WHITE, true);
    moves = MoveSorter::sort(position.getPieces(), moves);

    for(uint8_t i = 0; i < moves.size(); i++){
        Move move = moves[i];

        Position copy = position;
        copy.move(move);

        evaluation = alphaBetaMinOnlyCaptures(copy, alpha, beta);
        if(evaluation >= beta)
            return beta;
        if(evaluation > alpha)
            return evaluation;
    }

    return alpha;

}
