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
    
    m->gameLoop();

    endwin();
    cout << m->getNum() <<endl;
    delete m;
	return 0;
}