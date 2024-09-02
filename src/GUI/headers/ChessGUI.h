#ifndef CHESSGUI_H
#define CHESSGUI_H

#include "src/GameEngine/MoveGen/headers/LegalMoveGen.h"
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QString>
#include "src/ChessAI/headers/AI.h"
#include "src/GUI/headers/ChessBoard.h"
#include <memory>

enum class StyleOfGame: uint8_t{
    PLAY_WITH_AI,
    PLAY_WITH_FRIEND
};

class ChessGUI : public QMainWindow{
    Q_OBJECT
public:
    ChessGUI(StyleOfGame style, QMainWindow* parent = nullptr);
    ~ChessGUI() = default;

public slots:
    void SetStatus(uint8_t status, QPair<uint8_t, uint8_t> from, QPair<uint8_t, uint8_t> to, uint8_t side);

private:
    void init();
    void makeConnections() noexcept;
    void addTurnInTable(QPair<uint8_t, uint8_t> from, QPair<uint8_t, uint8_t> to, uint8_t side_) noexcept;
    void addTurnInTableWithStatus(QPair<uint8_t, uint8_t> from, QPair<uint8_t, uint8_t> to, uint8_t statusOfGame) noexcept;




    std::unique_ptr<QWidget> BoardAndTable;
    std::unique_ptr<QHBoxLayout> LayoutForBoard;
    std::unique_ptr<QWidget> TurnWidget;
    std::unique_ptr<QVBoxLayout>TurnLayout;
    std::unique_ptr<QLabel> TurnLable;
    std::unique_ptr<QTableWidget> TableOfTurns;

    std::unique_ptr<ChessBoard> Board;

    std::unique_ptr<QWidget> CentralWidget;
    std::unique_ptr<QVBoxLayout> VerticalLayout;


    Position StartPosition{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR",Position::NONE, true, true, true, true, 1 };
    GameStatus status;
    SIDE playerTurn;
};

#endif // CHESSBOARD_H
