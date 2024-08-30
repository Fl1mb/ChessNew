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

enum GameStatus{
    CHECK,
    MATE,
    STALEMATE
};



class ChessGUI : public QMainWindow{
    Q_OBJECT
public:
    ChessGUI(QMainWindow* parent = nullptr);




public slots:
    void PlayWithAI();
    void PlayWithFriend();

    void SentPositionToOther(const Position& position, uint8_t FromWho);

private:
    void init();
    void makeConnections();

    QString ChangeBlackPosToWhite(const QString& position);

    std::unique_ptr<ChessBoard> BlackBoard;
    std::unique_ptr<ChessBoard> WhiteBoard;


    std::unique_ptr<QPushButton> makeMoveButton;
    std::unique_ptr<QPushButton> startPlayWithAI;
    std::unique_ptr<QPushButton> startPlayWithFriend;
    std::unique_ptr<QGridLayout> layout;
    std::unique_ptr<QWidget> centralWidget;

    GameStatus status;
    SIDE playerTurn;
};

#endif // CHESSBOARD_H
