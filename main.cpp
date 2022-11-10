#include <ncurses.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include "Game.hpp"

using namespace std;

int main(void){
    //stuff i need before running ncurses (it cant be done in )
	initscr(); 
    noecho();
    cbreak();
    curs_set(0);

    Game * m = new Game();
    m->StartGame();
    m->gameLoop();

    endwin();
    for (size_t i = 0; i < m->M->getMax(); i++)
    {
        cout << m->M->getPosX(i)<<" "<<m->M->getPosY(i)<< endl;
    }

    delete m;
	return 0;
}