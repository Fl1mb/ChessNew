#include "src/GUI/headers/ChessGUI.h"


ChessGUI::ChessGUI(QMainWindow *parent) : QMainWindow(parent)
{
    init();
    layout->addWidget(startPlayWithAI.get(),0,0, 1, 1);
    layout->addWidget(startPlayWithFriend.get(), 1,0,1,1);
    centralWidget->setLayout(layout.get());
    this->setCentralWidget(centralWidget.get());

    makeConnections();

}

void ChessGUI::PlayWithAI()
{

}

void ChessGUI::PlayWithFriend()
{
    this->hide();

    QString posis = QString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    BlackBoard = std::make_unique<ChessBoard>(BLACK, Position(posis.toStdString(), Position::NONE, true, true, true, true, 1, true));
    WhiteBoard = std::make_unique<ChessBoard>(WHITE, Position(posis.toStdString(), Position::NONE, true, true, true, true, 1));

    WhiteBoard->show();
}

void ChessGUI::SentPositionToOther(const Position &position, uint8_t FromWho)
{
    if(FromWho == WHITE){
        BlackBoard->setPosition(position);
    }else{
        WhiteBoard->setPosition(position);
    }
}

void ChessGUI::init()
{
    startPlayWithAI = std::make_unique<QPushButton>("Start to play with AI");
    startPlayWithFriend = std::make_unique<QPushButton>("Start to play with friend");
    makeMoveButton = std::make_unique<QPushButton>(QString::fromLatin1("Make Move"));;
    layout  = std::make_unique<QGridLayout>();
    centralWidget = std::make_unique<QWidget>();
}

void ChessGUI::makeConnections()
{
    QObject::connect(startPlayWithAI.get(), &QPushButton::clicked, this, &ChessGUI::PlayWithAI);
    QObject::connect(startPlayWithFriend.get(), &QPushButton::clicked, this, &ChessGUI::PlayWithFriend);
}

QString ChessGUI::ChangeBlackPosToWhite(const QString &position)
{
    QString result = "";
    for(QChar ch : position){
        if(ch.isLetter()){
            if(ch.isLower()){
                result += ch.toUpper();
            }else{
                result += ch.toLower();
            }
        }else{
            result += ch;
        }
    }
    return result;
}
