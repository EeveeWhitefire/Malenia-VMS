#include <iostream>

#include <QApplication>

#include "../headers/colors.h"
#include "../headers/Malenia.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Malenia win;

    std::cout << std::endl << Color::RED;
    std::cout << "   ▄▄▄▄███▄▄▄▄      ▄████████  ▄█          ▄████████ ███▄▄▄▄    ▄█     ▄████████" << std::endl;
    std::cout << " ▄██▀▀▀███▀▀▀██▄   ███    ███ ███         ███    ███ ███▀▀▀██▄ ███    ███    ███" << std::endl;
    std::cout << " ███   ███   ███   ███    ███ ███         ███    █▀  ███   ███ ███▌   ███    ███" << std::endl;
    std::cout << " ███   ███   ███   ███    ███ ███        ▄███▄▄▄     ███   ███ ███▌   ███    ███" << std::endl;
    std::cout << " ███   ███   ███ ▀███████████ ███       ▀▀███▀▀▀     ███   ███ ███▌ ▀███████████" << std::endl;
    std::cout << " ███   ███   ███   ███    ███ ███         ███    █▄  ███   ███ ███    ███    ███" << std::endl;
    std::cout << " ███   ███   ███   ███    ███ ███▌    ▄   ███    ███ ███   ███ ███    ███    ███" << std::endl;
    std::cout << "  ▀█   ███   █▀    ███    █▀  █████▄▄██   ██████████  ▀█   █▀  █▀     ███    █▀ " << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << " by T.H" << std::endl;
    std::cout << Color::YELLOW << std::endl;
    std::cout << " I dreamt for so long. My flesh was dull gold... and my blood, rotted. Corpse after corpse, left in my wake..." << std::endl;
    std::cout << " As I awaited... his return.... Heed my words. I am Malenia. Blade of Miquella. And I have never known defeat." << std::endl;
    std::cout << Color::RESET << std::endl;
    std::cout << std::endl;
    
    
    // win.setWindowState(Qt::WindowMaximized);
    win.show();
    return app.exec();
}
