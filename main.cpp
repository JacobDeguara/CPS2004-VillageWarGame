#include <ncurses.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include "Game.hpp"

using namespace std;

WINDOW * rWin;
WINDOW * mmWin;
WINDOW * smWin;
WINDOW * iWin;
WINDOW * mapWin;

void createWindows(int yMax,int xMax){
    //windows Created    
        //resourse window
        rWin = newwin(yMax*5/8,xMax*7/16,0,0); 
        box(rWin,0,0); //create box
        refresh();
        wrefresh(rWin);
        keypad(rWin,true);

        //main menu Window
        mmWin = newwin((yMax*3/8)+1,xMax*2/8,yMax*5/8,0); //new window
        box(mmWin,0,0); //create box
        refresh();
        wrefresh(mmWin);
        keypad(mmWin,true);

        //sub menu Window
        smWin = newwin((yMax*3/8)+1,xMax*2/8,yMax*5/8,xMax*2/8); //new window
        box(smWin,0,0); //create box
        refresh();
        wrefresh(smWin);
        keypad(smWin,true);
        
        //Input Window
        iWin = newwin((yMax*3/8)+1,xMax*4/8,yMax*5/8,xMax*4/8); //new window
        box(iWin,0,0); //create box
        refresh();
        wrefresh(iWin);
        keypad(iWin,true);

        //Map Window
        mapWin = newwin(yMax*5/8,xMax*9/16,0,xMax*7/16); //new window
        box(mapWin,0,0); //create box
        refresh();
        wrefresh(mapWin);
        keypad(mapWin,true);
}

int main(void){
    //stuff i need before running ncurses
	initscr(); 
    noecho();
    cbreak();
    curs_set(0);

    //get screen size
    int yMax, xMax;
    getmaxyx(stdscr,yMax,xMax);

    //create Windows
    createWindows(yMax,xMax);
    
    Game * m = new Game(3,mmWin,smWin,rWin,iWin,mapWin);
    
    m->gameLoop();

    cout << m->getNum()<< endl;
    endwin();
    delete m;
	return 0;
}