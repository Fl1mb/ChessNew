#include <QApplication>
#include "src/GUI/headers/mainmenu.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainMenu menu;
    menu.show();



    return a.exec();
}
