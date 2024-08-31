#include "src/GUI/headers/ChessBoard.h"

ChessBoard::ChessBoard(uint8_t sideOfPlayer_, const Position& position_, QGraphicsView *parent) : QGraphicsView(parent),
    position(position_),
    side(sideOfPlayer_),
    IsFigureChosen(false),
    LastPossibleMoves()
{
    this->resize(ChessBoard::size);
    scene = new QGraphicsScene;
    this->drawBoard();
    this->addFigures();
    this->setScene(scene);

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void ChessBoard::drawBoard()
{
    if(this->side == WHITE){
        for(uint8_t i = 0; i < 8; i ++){
            for(uint8_t j = 0; j < 8; j++){
                ColorOfSquare color = (i + j) % 2 == 0 ? white : black;
                char num = '/n';
                char let = '/n';
                if(i== 0){
                    num = '8' - j;
                }
                if(j== 7){
                    let = 'a' + i;
                }

                Elements[i][j] = new BoardElement(color, i * BoardElement::width, j * BoardElement::height, i, j, num, let);
                scene->addItem(Elements[i][j]);
                QObject::connect(Elements[i][j], &BoardElement::GetPrepared, this, &ChessBoard::getFigurePrepared);
                QObject::connect(Elements[i][j], &BoardElement::MakeMove, this, &ChessBoard::getFigureMoved);
            }
        }
    }else{
        for(uint8_t i = 0; i < 8; i ++){
            for(uint8_t j = 0; j < 8; j++){
                ColorOfSquare color = (i + j) % 2 == 0 ? black : white;
                char num = '/n';
                char let = '/n';
                if(i== 0){
                    num = '1' + j;
                }
                if(j== 7){
                    let = 'h' - i;
                }

                Elements[i][j] = new BoardElement(color, i * BoardElement::width, j * BoardElement::height, i, j, num, let);
                scene->addItem(Elements[i][j]);
                QObject::connect(Elements[i][j], &BoardElement::GetPrepared, this, &ChessBoard::getFigurePrepared);
                QObject::connect(Elements[i][j], &BoardElement::MakeMove, this, &ChessBoard::getFigureMoved);
            }
        }
    }
}

void ChessBoard::addFigures()
{
    if(this->side == WHITE){
        for(int32_t x = 0; x < 8; x++){
            for(int32_t y = 0; y < 8; y++){
                QPair<uint8_t, uint8_t> SideName = this->getTextureName(x, y);
                if(SideName.first == Position::NONE){
                    Elements[x][y]->setFigure(BoardElement::NONE, BoardElement::NONE);
                    Elements[x][y]->update();
                    continue;
                }

                Elements[x][y]->setFigure(SideName.first, SideName.second);
                Elements[x][y]->setPossible(false);
                Elements[x][y]->update();
            }
        }
    }else{
        for(int32_t x = 0; x < 8; x++){
            for(int32_t y = 0; y < 8; y++){
                QPair<uint8_t, uint8_t> SideName = this->getTextureName(x, y);
                if(SideName.first == Position::NONE){
                    Elements[x][y]->setFigure(BoardElement::NONE, BoardElement::NONE);
                    Elements[x][y]->update();
                    continue;
                }

                Elements[x][y]->setFigure(SideName.first, SideName.second);
                Elements[x][y]->setPossible(false);

                Elements[x][y]->update();
            }
        }
    }


}

void ChessBoard::MoveFigure(uint8_t from, uint8_t to)
{

    uint8_t x1 = from % 8;
    uint8_t y1 = from / 8;

    uint8_t x2 = to % 8;
    uint8_t y2 = to / 8;

    uint8_t flag = this->getFlagOfMove(from, to);

    if(this->side == BLACK){
        from = (7 - y1)* 8 + (7 - x1);
        to = (7 - y2) * 8 + (7 - x2);
    }


    this->position.move(Move(from, to, Elements[x1][y1]->getPiece(), Elements[x1][y1]->getSide(),Elements[x2][y2]->getPiece(), Elements[x2][y2]->getSide(), flag));
    Elements[x2][y2]->setFigure(Elements[x1][y1]->getSide(), Elements[x1][y1]->getPiece());
    Elements[x1][y1]->setFigure(Move::NONE, Move::NONE);

    uint8_t status = getStatus();
    switch(status){
    case GameStatus::DRAW:
        qDebug() <<"draw";
        break;
    case GameStatus::BLACK_WIN:
        qDebug() << "Black Win";
        break;
    case GameStatus::WHITE_WIN:
        qDebug() << "White win";
        break;
    case GameStatus::BLACK_TO_MOVE:
        qDebug() << "Black to move";
        break;
    case GameStatus::WHITE_TO_MOVE:
        qDebug() << "White to move";
        break;
    case GameStatus::BLACK_CHECKED:
        qDebug() << "Black Checked";
        break;
    case GameStatus::WHITE_CHECKED:
        qDebug() << "White checked";
        break;
    }

    for(auto i = 0; i  < 8; i ++){
        for (auto j = 0; j  < 8; j++){
            Elements[i][j]->setPossible(false);
            Elements[i][j]->update();
        }
    }

    Elements[x2][y2]->update();
    Elements[x1][y1]->update();

    IsFigureChosen = false;
    emit Moved();
}

void ChessBoard::ChangeLetters(uint8_t Side_) {
    if (Side_ == WHITE) {
        for (auto i = 0; i < 8; i++) {
            Elements[0][i]->setLetters('8' - i, '\0');
        }
        for (auto i = 0; i < 8; i++) {
            Elements[i][7]->setLetters('\0', 'a' + i);
        }
    } else {
        for (auto i = 0; i < 8; i++) {
            Elements[0][i]->setLetters('1' + i, '\0');
        }
        for (auto i = 0; i < 8; i++) {
            Elements[i][7]->setLetters('\0', 'h' - i);
        }
    }
}

bool ChessBoard::isInsufficientMaterial()
{
    return !this->position.getPieces().getPieceBitBoard(SIDE::WHITE, PIECE::PAWN) &&
            !this->position.getPieces().getPieceBitBoard(SIDE::BLACK, PIECE::PAWN) &&
            !this->position.getPieces().getPieceBitBoard(SIDE::WHITE, PIECE::ROOK) &&
            !this->position.getPieces().getPieceBitBoard(SIDE::BLACK, PIECE::ROOK) &&
            !this->position.getPieces().getPieceBitBoard(SIDE::WHITE, PIECE::QUEEN) &&
            !this->position.getPieces().getPieceBitBoard(SIDE::BLACK, PIECE::QUEEN) &&
            BitBoardOperations::count_1(this->position.getPieces().getPieceBitBoard(SIDE::WHITE, PIECE::KNIGHT) |
                                        this->position.getPieces().getPieceBitBoard(SIDE::WHITE, PIECE::BISHOP)) < 2 &&
            BitBoardOperations::count_1(this->position.getPieces().getPieceBitBoard(SIDE::BLACK, PIECE::KNIGHT) |
                                        this->position.getPieces().getPieceBitBoard(SIDE::BLACK, PIECE::BISHOP)) < 2;

}

bool ChessBoard::isInCheck(uint8_t side_) {
    MoveList moves = LegalMoveGen::generate(this->position, side_);
    for (auto i = 0; i < moves.size(); i++){
        uint8_t x = moves[i].getTo() % 8;
        uint8_t y = moves[i].getTo() / 8;
        if(this->side == BLACK){
            x = 7 - x;
            y = 7 - y;
        }
        if (Elements[x][y]->getPiece() == PIECE::KING && Elements[x][y]->getSide() != side_) {
            return true;
        }
    }
    return false;
}
uint8_t ChessBoard::getBlackStatus() {
    MoveList moves = LegalMoveGen::generate(this->position, WHITE);
    if (moves.size() == 0) {
        if (isInCheck(BLACK)) {
            return GameStatus::WHITE_WIN;
        } else {
            return GameStatus::DRAW;
        }
    } else if (isInCheck(BLACK)) {
        return GameStatus::BLACK_CHECKED;
    } else {
        return GameStatus::BLACK_TO_MOVE;
    }
}

uint8_t ChessBoard::getWhiteStatus() {
    MoveList moves = LegalMoveGen::generate(this->position, BLACK);
    if (moves.size() == 0) {
        if (isInCheck(WHITE)) {
            return GameStatus::BLACK_WIN;
        } else {
            return GameStatus::DRAW;
        }
    } else if (isInCheck(WHITE)) {
        return GameStatus::WHITE_CHECKED;
    } else {
        return GameStatus::WHITE_TO_MOVE;
    }
}

uint8_t ChessBoard::getStatus()
{
    if(this->position.fiftyMovesRuleDraw() || this->position.threefoldRepetitionDraw()){
        return GameStatus::DRAW;
    }
    if(this->isInsufficientMaterial()){
        return GameStatus::DRAW;
    }

    if(this->position.blackToMove()){
        return getBlackStatus();
    }else{
        return getWhiteStatus();
    }
}

void ChessBoard::setPosition(const Position &position_)
{
    this->position = position_;
    this->addFigures();
}

void ChessBoard::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox;
    msgBox.setText("Do you want to exit the game?");
    msgBox.setInformativeText("All unsaved progress will be lost.");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes) {
        event->accept(); // Exit the application
    } else {
        event->ignore(); // Cancel the close event
    }
}

void ChessBoard::ChangeSide(uint8_t Side_)
{
    this->side = this->side == WHITE? BLACK : WHITE;
    this->ChangeLetters(this->side);
    this->addFigures();
}

void ChessBoard::TransformCoordinates(uint8_t &x, uint8_t &y)
{
    x = 7- x;
    y = 7 - y;
}

uint8_t ChessBoard::getPromotionChoice()
{

}

QPair<uint8_t, uint8_t> ChessBoard::getTextureName(int32_t x, int32_t y)
{
    std::array<std::array<BitBoard, 6>, 2> bs = this->position.getPieces().getPieceBitBoards();
    int32_t id = y * 8 + x;
    if(this->side == BLACK){
        id = (7 - y) * 8 + (7 - x);
    }

    if (BitBoardOperations::getBit(bs[WHITE][PAWN], id)) {
        return QPair<uint8_t, uint8_t>(WHITE, PAWN);
    }
    if (BitBoardOperations::getBit(bs[WHITE][KNIGHT], id)) {
        return QPair<uint8_t, uint8_t>(WHITE, KNIGHT);
    }
    if (BitBoardOperations::getBit(bs[WHITE][BISHOP], id)) {
        return QPair<uint8_t, uint8_t>(WHITE, BISHOP);
    }
    if (BitBoardOperations::getBit(bs[WHITE][ROOK], id)) {
        return QPair<uint8_t, uint8_t>(WHITE, ROOK);
    }
    if (BitBoardOperations::getBit(bs[WHITE][QUEEN], id)) {
        return QPair<uint8_t, uint8_t>(WHITE, QUEEN);
    }
    if (BitBoardOperations::getBit(bs[WHITE][KING], id)) {
        return QPair<uint8_t, uint8_t>(WHITE, KING);
    }
    if (BitBoardOperations::getBit(bs[BLACK][PAWN], id)) {
        return QPair<uint8_t, uint8_t>(BLACK, PAWN);
    }
    if (BitBoardOperations::getBit(bs[BLACK][KNIGHT], id)) {
        return QPair<uint8_t, uint8_t>(BLACK, KNIGHT);
    }
    if (BitBoardOperations::getBit(bs[BLACK][BISHOP], id)) {
        return QPair<uint8_t, uint8_t>(BLACK, BISHOP);
    }
    if (BitBoardOperations::getBit(bs[BLACK][ROOK], id)) {
        return QPair<uint8_t, uint8_t>(BLACK, ROOK);
    }
    if (BitBoardOperations::getBit(bs[BLACK][QUEEN], id)) {
        return QPair<uint8_t, uint8_t>(BLACK, QUEEN);
    }
    if (BitBoardOperations::getBit(bs[BLACK][KING], id)) {
        return QPair<uint8_t, uint8_t>(BLACK, KING);
    }

    return QPair<uint8_t, uint8_t>(Position::NONE, Position::NONE);
}

QPair<uint8_t, uint8_t> ChessBoard::BlackToWhiteTransform(QPair<uint8_t, uint8_t> pair)
{
    uint8_t side_ = pair.first == WHITE ? BLACK : WHITE;
    return QPair<uint8_t, uint8_t>(side_, pair.second);
}

uint8_t ChessBoard::BlackWhiteReverse(uint8_t side)
{
    return side == WHITE ? BLACK : WHITE;
}

uint8_t ChessBoard::getFlagOfMove(uint8_t from, uint8_t to)
{
    uint8_t x1 = from % 8;
    uint8_t y1 = from / 8;

    uint8_t x2 = to % 8;
    uint8_t y2 = to / 8;


    uint8_t piece = Elements[x1][y1]->getPiece();
    uint8_t PieceSide = Elements[x1][y1]->getSide();

    uint8_t dy = std::abs(y2 - y1);
    uint8_t dx = x2 - x1;

    if(piece == PIECE::PAWN){
        if(y2 == 0 or y2 == 7){
            return this->getPromotionChoice();
        }
        if(dy == 2)
            return Move::FLAGS::PAWN_LONG_MOVE;
        else if(dy == 1 and dx == 1)
            return Move::FLAGS::EN_PASSANT_CAPTURE;
        else if(dy == 1 and dx != 1)
            return Move::FLAGS::DEFAULT;
    }
    else if(piece == PIECE::KING and dy == 0){
        if(PieceSide != WHITE){
            if(x2 == 2){
                return Move::BL_CASTLING;
            }else{
                return Move::BS_CASTLING;
            }
        }else{
            if(x2 == 1){
                return Move::WS_CASTLING;
            }else{
                return Move::WL_CASTLING;
            }
        }
    }
    return Move::FLAGS::DEFAULT;

}

void ChessBoard::getFigurePrepared(QPair<uint8_t, uint8_t> figure)
{
    if(IsFigureChosen){
        for(auto i  = 0; i < 8; i++){
            for(auto j = 0; j < 8; j ++){
                Elements[i][j]->setPossible(false);
            }
        }
        IsFigureChosen = !IsFigureChosen;
    }

    buffer = figure;
    BoardElement* element = Elements[figure.first][figure.second];

    MoveList moves = LegalMoveGen::generate(this->position, element->getSide());
    for(uint8_t i = 0; i < moves.size();i ++){
        if(this->side == WHITE){
            if (moves[i].getFrom() == figure.second * 8 + figure.first) {
                LastPossibleMoves.push_back(moves[i].getTo());
                uint8_t x = moves[i].getTo() % 8;
                uint8_t y = moves[i].getTo() / 8;
                Elements[x][y]->setPossible(true);
            }
        }
        else{
            if (moves[i].getFrom() == (7 - figure.second )* 8 + (7  - figure.first)) {
                LastPossibleMoves.push_back(moves[i].getTo());
                uint8_t x = 7 -moves[i].getTo() % 8;
                uint8_t y = 7 - moves[i].getTo() / 8;
                Elements[x][y]->setPossible(true);
            }
        }

    }
    IsFigureChosen = true;
}

void ChessBoard::getFigureMoved(QPair<uint8_t, uint8_t> to)
{
    uint8_t from = buffer.second * 8 + buffer.first;
    uint8_t To = to.first * 8 + to.second;

    this->MoveFigure(from, To);
}



