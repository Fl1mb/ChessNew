#ifndef MAINMENU_H
#define MAINMENU_H

#include <QDialog>
#include "src/OnlinePlaying/headers/Client.h"
#include "ChessGUI.h"
#include <memory>

namespace Ui {
class MainMenu;
}

class MainMenu : public QDialog
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

public slots:
    void StartPlayWithAI();
    void StartPlayWithFriend();
    void StartPlayOnline();

    void closeGame();

private:
    QString UserName;
    QString Password;

    std::unique_ptr<Ui::MainMenu> ui;
    std::unique_ptr<ChessGUI> gui;
    std::unique_ptr<ClientSocket> socket;

};

#endif // MAINMENU_H
