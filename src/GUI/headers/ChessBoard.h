#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPair>
#include <QCloseEvent>
#include <QMessageBox>
#include <QTransform>
#include <QGraphicsProxyWidget>
#include <QObject>
#include "BoardElement.h"
#include "src/GameEngine/MoveGen/headers/LegalMoveGen.h"

enum GameStatus {
    DRAW = 0,

    WHITE_TO_MOVE = 1,
    BLACK_TO_MOVE = 2,

    WHITE_WIN = 3,
    BLACK_WIN = 4,

    WHITE_CHECKED = 5,
    BLACK_CHECKED = 6
};

class ChessBoard : public QGraphicsView{
    Q_OBJECT
public:
    ChessBoard(uint8_t sideOfPlayer_, const Position& position_, QGraphicsView* parent = nullptr);
    void drawBoard();
    void addFigures();
    void MoveFigure(uint8_t from, uint8_t to);
    void ChangeLetters(uint8_t Side_);

    uint8_t getStatus();
    bool isInsufficientMaterial();
    bool isInCheck(uint8_t side_);
    uint8_t getBlackStatus();
    uint8_t getWhiteStatus();

    void setPosition(const Position& position_);

    void closeEvent(QCloseEvent* event)override;

    void ChangeSide(uint8_t Side_);
    void TransformCoordinates(uint8_t& x, uint8_t& y);

    uint8_t getPromotionChoice();

    static constexpr QSize size{600, 600};

    QPair<uint8_t, uint8_t> getTextureName(int32_t x, int32_t y);
    QPair<uint8_t, uint8_t> BlackToWhiteTransform(QPair<uint8_t, uint8_t> pair);

    uint8_t BlackWhiteReverse(uint8_t side);

    uint8_t getFlagOfMove(uint8_t from, uint8_t to) ;

public slots:
    void getFigurePrepared(QPair<uint8_t, uint8_t> figure);
    void getFigureMoved(QPair<uint8_t, uint8_t> to);

signals:
    void UpdatePosition(const Position& position, uint8_t side);
    void Moved();
    void SentStatus(uint8_t status);


private:
    QGraphicsScene* scene;
    std::array<std::array<BoardElement*, 8>, 8> Elements;

    QPair<uint8_t, uint8_t> buffer;
    std::list<uint8_t> LastPossibleMoves;
    Position position;
    uint8_t side;

    bool IsFigureChosen;
};

#endif // CHESSBOARD_H
