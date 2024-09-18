#include "mainmenu.h"
#include "ui_mainmenu.h"

MainMenu::MainMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);

    socket = std::make_unique<ClientSocket>();
    if(socket->isConnected)
        ui->PlayWithFriendOnl->setEnabled(true);
    else
        ui->PlayWithFriendOnl->setEnabled(false);

    QObject::connect(ui->PlayWithFriendOFF, &QPushButton::clicked, this, &MainMenu::StartPlayWithFriend);
    QObject::connect(ui->PlayAI, &QPushButton::clicked, this, &MainMenu::StartPlayWithAI);
    QObject::connect(ui->PlayWithFriendOnl, &QPushButton::clicked, this, &MainMenu::StartPlayOnline);
}

MainMenu::~MainMenu()
{
}

void MainMenu::StartPlayWithAI()
{
    if (gui) {
        gui->restartBoard(StyleOfGame::PLAY_WITH_AI, WHITE);
        gui->show();
        this->hide();
        QObject::connect(gui.get(), &ChessGUI::closeGame, this, &MainMenu::closeGame);
        return;
    }
    gui = std::make_unique<ChessGUI>(StyleOfGame::PLAY_WITH_FRIEND, WHITE);
    gui->show();
    this->hide();

    QObject::connect(gui.get(), &ChessGUI::closeGame, this, &MainMenu::closeGame);
}

void MainMenu::StartPlayWithFriend()
{
    if (gui) {
        gui->restartBoard(StyleOfGame::PLAY_WITH_FRIEND, WHITE);
        gui->show();
        this->hide();
        return;

    }
    gui = std::make_unique<ChessGUI>(StyleOfGame::PLAY_WITH_FRIEND, WHITE);
    gui->show();
    this->hide();

    QObject::connect(gui.get(), &ChessGUI::closeGame, this, &MainMenu::closeGame);
}

void MainMenu::StartPlayOnline()
{

}

void MainMenu::closeGame()
{
    this->gui->hide();
    this->show();

}
