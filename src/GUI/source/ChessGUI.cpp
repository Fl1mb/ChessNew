#include "src/GUI/headers/ChessGUI.h"


ChessGUI::ChessGUI(StyleOfGame style, QMainWindow *parent) : QMainWindow(parent)
{
    init();
    makeConnections();

}


void ChessGUI::SetStatus(uint8_t status, QPair<uint8_t, uint8_t> from, QPair<uint8_t, uint8_t> to, uint8_t side)
{
    switch(status){
    case GameStatus::DRAW:
        this->addTurnInTable(from, to, status);
        return;
    case GameStatus::WHITE_TO_MOVE:
        Board->deleteCheck();
        break;
    case GameStatus::BLACK_TO_MOVE:
        Board->deleteCheck();
        break;
    case  GameStatus::WHITE_CHECKED:
        Board->setKingChecked(WHITE);
        this->addTurnInTableWithStatus(from, to, status);
        return;
    case GameStatus::BLACK_CHECKED:
        this->addTurnInTableWithStatus(from, to, status);
        Board->setKingChecked(BLACK);
        return;
    case GameStatus::WHITE_WIN:
        this->addTurnInTableWithStatus(from, to, status);
        Board->setKingChecked(BLACK);
        return;
    case GameStatus::BLACK_WIN:
        this->addTurnInTableWithStatus(from, to, status);
        Board->setKingChecked(WHITE);
        return;
    }
    this->addTurnInTable(from, to, side);

}



void ChessGUI::init()
{
    Board = std::make_unique<ChessBoard>(WHITE, this->StartPosition);
    Board->setMinimumSize(ChessBoard::size);

    CentralWidget = std::make_unique<QWidget>();
    VerticalLayout = std::make_unique<QVBoxLayout>();
    BoardAndTable = std::make_unique<QWidget>();
    LayoutForBoard = std::make_unique<QHBoxLayout>();

    TurnLable = std::make_unique<QLabel>("<b>История ходов<b>");
    TurnLable->setAlignment(Qt::AlignCenter);

    TableOfTurns = std::make_unique<QTableWidget>();
    TableOfTurns->setColumnCount(2);
    TableOfTurns->setHorizontalHeaderLabels({"Ход белых", "Ход черных"});
    TableOfTurns->setFixedWidth(210);
    TableOfTurns->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    TurnWidget = std::make_unique<QWidget>();
    TurnLayout = std::make_unique<QVBoxLayout>();

    TurnLayout->addWidget(TurnLable.get());
    TurnLayout->addWidget(TableOfTurns.get());
    TurnWidget->setLayout(TurnLayout.get());

    LayoutForBoard->addWidget(Board.get());
    LayoutForBoard->addWidget(TurnWidget.get());
    BoardAndTable->setLayout(LayoutForBoard.get());

    VerticalLayout->addWidget(BoardAndTable.get());
    CentralWidget->setLayout(VerticalLayout.get());


    this->setCentralWidget(CentralWidget.get());
    this->show();
}

void ChessGUI::makeConnections() noexcept
{
    QObject::connect(Board.get(), &ChessBoard::Moved, Board.get(), [&](){Board->ChangeSide(BLACK);});
    QObject::connect(Board.get(), &ChessBoard::SentStatus, this, &ChessGUI::SetStatus);
}

void ChessGUI::addTurnInTable(QPair<uint8_t, uint8_t> from, QPair<uint8_t, uint8_t> to, uint8_t side_) noexcept
{
    uint8_t x1 = from.first;
    uint8_t y1 = from.second;

    uint8_t x2 = to.first;
    uint8_t y2 = to.second;

    if(side_ != WHITE){
        QString fromStr = QString("%1%2").arg(QChar('a' + x1)).arg(8 - y1 );
        QString toStr = QString("%1%2").arg(QChar('a' + x2)).arg(8 - y2 );

        QTableWidgetItem* item = new QTableWidgetItem(fromStr + "-" + toStr);

        int row = TableOfTurns->rowCount(); // Get the current row count
        TableOfTurns->insertRow(row); // Insert a new row
        TableOfTurns->setItem(row, 0, item);
    }else{
        QString fromStr = QString("%1%2").arg(QChar('h' - x1)).arg(1 +  y1);
        QString toStr = QString("%1%2").arg(QChar('h' - x2)).arg(1 + y2);

        QTableWidgetItem* item = new QTableWidgetItem(fromStr + "-" + toStr);

        int row = TableOfTurns->rowCount(); // Get the current row count
        TableOfTurns->setItem(row - 1, 1, item);
    }


}


void ChessGUI::addTurnInTableWithStatus(QPair<uint8_t, uint8_t> from, QPair<uint8_t, uint8_t> to, uint8_t statusOfGame) noexcept
{
    uint8_t x1 = from.first;
    uint8_t y1 = from.second;

    uint8_t x2 = to.first;
    uint8_t y2 = to.second;

    QString fromStr, toStr;
    int column;

    switch(statusOfGame){
    case WHITE_CHECKED:
        fromStr = QString("%1%2").arg(QChar('h' - x1)).arg(1 +  y1);
        toStr = QString("%1%2").arg(QChar('h' - x2)).arg(1 + y2);
        column = 1;
        break;
    case BLACK_CHECKED:
        fromStr = QString("%1%2").arg(QChar('a' + x1)).arg(8 - y1 );
        toStr = QString("%1%2").arg(QChar('a' + x2)).arg(8 - y2 );
        column = 0;
        break;
    case WHITE_WIN:
        fromStr = QString("%1%2").arg(QChar('a' + x1)).arg(8 - y1 );
        toStr = QString("%1%2").arg(QChar('a' + x2)).arg(8 - y2 );
        column = 0;
        break;
    case BLACK_WIN:
        fromStr = QString("%1%2").arg(QChar('h' - x1)).arg(1 +  y1);
        toStr = QString("%1%2").arg(QChar('h' - x2)).arg(1 + y2);
        column = 1;
        break;
    case DRAW:
        fromStr = "==";
        toStr = "";
        column = 0;
        break;
    }

    QTableWidgetItem* item = new QTableWidgetItem(fromStr + "-" + toStr + (statusOfGame == WHITE_CHECKED || statusOfGame == BLACK_CHECKED ? "+" : (statusOfGame == WHITE_WIN || statusOfGame == BLACK_WIN ? "#" : "")));

    int row = TableOfTurns->rowCount(); // Get the current row count
    TableOfTurns->insertRow(row); // Insert a new row
    TableOfTurns->setItem(row, column, item);
}
