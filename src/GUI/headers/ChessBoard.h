#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPair>
#include <QVector2D>
#include <QCloseEvent>
#include <QMessageBox>
#include <QTransform>
#include <QGraphicsProxyWidget>
#include <QObject>
#include "BoardElement.h"
#include "src/GameEngine/MoveGen/headers/LegalMoveGen.h"


class ChessBoard : public QGraphicsView{
    Q_OBJECT
public:
    ChessBoard(uint8_t sideOfPlayer_, const Position& position_, QGraphicsView* parent = nullptr);
    void drawBoard();
    void addFigures();
    void MoveFigure(uint8_t from, uint8_t to);

    void setPosition(const Position& position_);

    void closeEvent(QCloseEvent* event)override;

    void ChangeSide(uint8_t Side_);

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
