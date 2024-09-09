#ifndef MAINMENU_H
#define MAINMENU_H

#include <QDialog>
#include "ChessGUI.h"

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

private:
    Ui::MainMenu *ui;
    ChessGUI* gui;
};

#endif // MAINMENU_H
