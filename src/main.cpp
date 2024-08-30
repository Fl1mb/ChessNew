#include <QApplication>
#include "src/GUI/headers/ChessGUI.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChessGUI gui;
    gui.show();



    return a.exec();
}
