#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPair>
#include <QCloseEvent>
#include <QMessageBox>
#include <chrono>
#include <QTransform>
#include <QGraphicsProxyWidget>
#include <QObject>
#include <QMutex>
#include <QEventLoop>
#include "src/ChessAI/headers/AI.h"
#include "BoardElement.h"
#include "src/GameEngine/MoveGen/headers/LegalMoveGen.h"
#include "PromotionChoice.h"

enum GameStatus {
    DRAW = 0,

    WHITE_TO_MOVE = 1,
    BLACK_TO_MOVE = 2,

    WHITE_WIN = 3,
    BLACK_WIN = 4,

    WHITE_CHECKED = 5,
    BLACK_CHECKED = 6,

    WHITE_LEFT_GAME = 7,
    BLACK_LEFT_GAME = 8
};

class ChessBoard : public QGraphicsView{
    Q_OBJECT
public:
    explicit ChessBoard(uint8_t sideOfPlayer_, const Position& position_, QGraphicsView* parent = nullptr);
    ~ChessBoard();

    void StopGame() noexcept;
    void ResumeGame() noexcept;
    void drawBoard() noexcept;
    void addFigures() noexcept;
    void MoveFigure(uint8_t from, uint8_t to) noexcept;
    void MoveFigure(const Move& move) noexcept;
    void ChangeLetters(uint8_t Side_) noexcept;
    void setKingChecked(uint8_t side_) noexcept;
    void deleteCheck() noexcept;
    void setPosition(const Position& position_) noexcept;
    void ChangeSide() noexcept;
    void TransformCoordinates(uint8_t& x, uint8_t& y) noexcept;
    void TurnOnAI(uint8_t sideOfAI) noexcept;
    void setFriendFlag(bool flag) noexcept;

    [[nodiscard]] uint8_t getSide() const noexcept;
    [[nodiscard]] uint8_t getStatus() noexcept;
    [[nodiscard]] uint8_t getBlackStatus()  noexcept;
    [[nodiscard]] uint8_t getWhiteStatus() noexcept;
    [[nodiscard]] uint8_t getPromotionChoice() noexcept;
    [[nodiscard]] uint8_t BlackWhiteReverse(uint8_t side) noexcept;
    [[nodiscard]] uint8_t getFlagOfMove(uint8_t from, uint8_t to) noexcept;

    QMutex &getMutex() noexcept;

    bool isInsufficientMaterial() noexcept;
    bool isInCheck(uint8_t side_) noexcept;

    QPair<uint8_t, uint8_t> getTextureName(int32_t x, int32_t y) const noexcept;
    QPair<uint8_t, uint8_t> BlackToWhiteTransform(QPair<uint8_t, uint8_t> pair) noexcept;

    const Position &getPosition() const noexcept;

    static constexpr QSize size{600, 600};

public slots:
    void getFigurePrepared(QPair<uint8_t, uint8_t> figure);
    void getFigureMoved(QPair<uint8_t, uint8_t> to);

signals:
    void UpdatePosition(const Position& position, uint8_t side);
    void Moved(Move move);
    void SentStatus(uint8_t status, QPair<uint8_t, uint8_t> from, QPair<uint8_t, uint8_t> To, uint8_t side_);


private:
    std::unique_ptr<QGraphicsScene> scene;
    std::array<std::array<std::unique_ptr<BoardElement>, 8>, 8> Elements;
    BoardElement* CheckedSquare;
    PromotionChoice* choice;

    QMutex mutex;

    QPair<uint8_t, uint8_t> buffer;
    std::list<uint8_t> LastPossibleMoves;
    Position position;
    uint8_t side;
    uint8_t SideOfAI;

    bool IsFigureChosen;
    bool IsWhiteMove;
    bool FriendFlag;

};

#endif // CHESSBOARD_H
