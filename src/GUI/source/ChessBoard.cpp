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

void ChessBoard::MoveFigure(uint8_t from, uint8_t to)
{

    uint8_t x1 = from % 8;
    uint8_t y1 = from / 8;

    uint8_t x2 = to % 8;
    uint8_t y2 = to / 8;

    qDebug() <<  "Move to :"<< x2 << " " << y2;

    uint8_t flag = this->getFlagOfMove(from, to);

    this->position.move(Move(from, to, Elements[x1][y1]->getPiece(), Elements[x1][y1]->getSide(),Elements[x2][y2]->getPiece(), Elements[x2][y2]->getSide(), flag));

    Elements[x2][y2]->setFigure(Elements[x1][y1]->getSide(), Elements[x1][y1]->getPiece());
    Elements[x1][y1]->setFigure(Move::NONE, Move::NONE);

    for(uint8_t iter : LastPossibleMoves){
        uint8_t x = iter % 8;
        uint8_t y =  iter / 8 ;
        Elements[x][y]->setPossible(false);
        Elements[x][y]->update();
    }

    Elements[x2][y2]->update();
    Elements[x1][y1]->update();

    IsFigureChosen = false;
    emit ChessBoard::UpdatePosition(this->position, this->side);
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
    this->side = Side_;
    this->addFigures();
}

QPair<uint8_t, uint8_t> ChessBoard::getTextureName(int32_t x, int32_t y)
{
    std::array<std::array<BitBoard, 6>, 2> bs = this->position.getPieces().getPieceBitBoards();
    int32_t id = y * 8 + x;

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

    if (this->side == BLACK) {
        x1 = 7 - x1;
        y1 = 7 - y1;
        x2 = 7 - x2;
        y2 = 7 - y2;
    }

    uint8_t piece = Elements[x1][y1]->getPiece();

    if(piece == PIECE::PAWN){
        if (y1 == 1 && y2 == 3 && x1 == x2) { // pawn long move
            return Move::FLAGS::PAWN_LONG_MOVE;
        } else if (y1 == 6 && y2 == 4 && x1 == x2) { // pawn long move
            return Move::FLAGS::PAWN_LONG_MOVE;
        } else if (x1 != x2 && Elements[x2][y2]->getPiece() == PAWN && Elements[x2][y2]->getSide() != Elements[x1][y1]->getSide()) { // en passant capture
            return Move::FLAGS::EN_PASSANT_CAPTURE;
        }
    }else if(piece == PIECE::KING){
        if (x1 == 4 && x2 == 6 && y1 == y2) { // white king-side castling
            return Move::FLAGS::WS_CASTLING;
        } else if (x1 == 4 && x2 == 2 && y1 == y2) { // white queen-side castling
            return Move::FLAGS::WL_CASTLING;
        } else if (x1 == 4 && x2 == 6 && y1 == y2) { // black king-side castling
            return Move::FLAGS::BS_CASTLING;
        } else if (x1 == 4 && x2 == 2 && y1 == y2) { // black queen-side castling
            return Move::FLAGS::BL_CASTLING;
        }
    }else if(piece == PIECE::PAWN && y2 == 7){
        return Move::FLAGS::PROMOTE_TO_QUEEN;
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

    qDebug() << this->position.getHash().getValue();

    qDebug () << moves.size();
    for(uint8_t i = 0; i < moves.size();i ++){
        uint8_t fromX = 7 -moves[i].getFrom() % 8;
        uint8_t fromY =7 -  moves[i].getTo() / 8;

        qDebug() << fromX << " " << fromY;

        if(this->side == WHITE){
            if (moves[i].getFrom() == figure.second * 8 + figure.first) {
                LastPossibleMoves.push_back(moves[i].getTo());
                uint8_t x = moves[i].getTo() % 8;
                uint8_t y = moves[i].getTo() / 8;
                qDebug() << "Possible x: " << x << " Possible y: " << y;
                Elements[x][y]->setPossible(true);
            }
        }
        else{
            if (moves[i].getFrom() == (7 - figure.second )* 8 + (7 - figure.first)) {
                LastPossibleMoves.push_back(moves[i].getTo());
                uint8_t x = 7 -moves[i].getTo() % 8;
                uint8_t y = 7 - moves[i].getTo() / 8;
                qDebug() << "Possible x: " << x << " Possible y: " << y;
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



