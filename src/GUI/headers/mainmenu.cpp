#include "mainmenu.h"
#include "ui_mainmenu.h"

MainMenu::MainMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    gui = nullptr;

    QObject::connect(ui->PlayWithFriendOFF, &QPushButton::clicked, this, &MainMenu::StartPlayWithFriend);
    QObject::connect(ui->PlayAI, &QPushButton::clicked, this, &MainMenu::StartPlayWithAI);
    QObject::connect(ui->PlayWithFriendOnl, &QPushButton::clicked, this, &MainMenu::StartPlayOnline);
}

MainMenu::~MainMenu()
{
    delete ui;
    if(gui != nullptr)
        delete gui;
}

void MainMenu::StartPlayWithAI()
{
    gui = new ChessGUI(StyleOfGame::PLAY_WITH_AI);
    gui->show();
    this->hide();
}

void MainMenu::StartPlayWithFriend()
{
    gui = new ChessGUI(StyleOfGame::PLAY_WITH_FRIEND);
    gui->show();
    this->hide();
}

void MainMenu::StartPlayOnline()
{

}
