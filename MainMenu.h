#ifndef MAINMENU_H
#define MAINMENU_H

#include "Menu.h"


class MainMenu : public Menu
{
public:
    MainMenu(Menu* addrBookMenu, Menu* dateBookMenu) : m_addrBookMenu(addrBookMenu), m_dateBookMenu(dateBookMenu)
    {

    }

    void mainLoop();

private:
    void addressBook();
    void dateBook();
    void quit();

    Menu* m_addrBookMenu;
    Menu* m_dateBookMenu;
};

#endif // MAINMENU_H
