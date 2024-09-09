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
#include <QThread>
#include "src/GUI/headers/ChessBoard.h"
#include <memory>
#include <QtMultimedia/QSoundEffect>


class ChessGUI;
class AIThread;


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
    void StartGameWithAI(uint8_t SideOfAI);
    void StartGameWithFriend(uint8_t Side);
    void addTurnInTableWithStatus(QPair<uint8_t, uint8_t> from, QPair<uint8_t, uint8_t> to, uint8_t statusOfGame) noexcept;
    void MakeTurnByAI(const Position& position);


    std::unique_ptr<QWidget> BoardAndTable;
    std::unique_ptr<QHBoxLayout> LayoutForBoard;
    std::unique_ptr<QWidget> TurnWidget;
    std::unique_ptr<QVBoxLayout>TurnLayout;
    std::unique_ptr<QLabel> TurnLable;
    std::unique_ptr<QTableWidget> TableOfTurns;
    std::unique_ptr<ChessBoard> Board;
    std::unique_ptr<QWidget> CentralWidget;
    std::unique_ptr<QVBoxLayout> VerticalLayout;

    uint32_t numberOfMoves;

    Position StartPosition{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR",Position::NONE, true, true, true, true, 1 };
    GameStatus status;
    SIDE playerTurn;

    friend class AIThread;
};


class AIThread : public QThread{
    Q_OBJECT
public:
    AIThread(ChessGUI* gui, const Position& position);

    void run() override;

private:
    ChessGUI* gui;
    Position position;
};

#endif // CHESSBOARD_H
