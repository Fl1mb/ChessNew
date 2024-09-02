#include <QApplication>
#include "src/GUI/headers/ChessGUI.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChessGUI gui(StyleOfGame::PLAY_WITH_FRIEND);
    gui.show();



    return a.exec();
}
