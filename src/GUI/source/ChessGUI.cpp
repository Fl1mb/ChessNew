#include "src/GUI/headers/ChessGUI.h"


ChessGUI::ChessGUI(QMainWindow *parent) : QMainWindow(parent)
{
    init();


    makeConnections();

}



void ChessGUI::init()
{
    Board = std::make_unique<ChessBoard>(WHITE, this->StartPosition);
    CentralWidget = std::make_unique<QWidget>();
    VerticalLayout = std::make_unique<QVBoxLayout>();

    VerticalLayout->addWidget(Board.get());
    CentralWidget->setLayout(VerticalLayout.get());

    this->setCentralWidget(CentralWidget.get());
    this->show();
}

void ChessGUI::makeConnections()
{
    QObject::connect(Board.get(), &ChessBoard::Moved, Board.get(), [&](){Board->ChangeSide(BLACK);});
}
