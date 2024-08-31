#ifndef CHESSGUI_H
#define CHESSGUI_H

#include "src/GameEngine/MoveGen/headers/LegalMoveGen.h"
#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include "src/GUI/headers/ChessBoard.h"
#include <memory>

class ChessGUI : public QMainWindow{
    Q_OBJECT
public:
    ChessGUI(QMainWindow* parent = nullptr);

public slots:


private:
    void init();
    void makeConnections();


    std::unique_ptr<ChessBoard> Board;
    std::unique_ptr<QWidget> CentralWidget;
    std::unique_ptr<QVBoxLayout> VerticalLayout;


    Position StartPosition{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR",Position::NONE, true, true, true, true, 1 };
    GameStatus status;
    SIDE playerTurn;
};

#endif // CHESSBOARD_H
