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

void refreshWindows(){
    wrefresh(rWin);
    wrefresh(mmWin);
    wrefresh(smWin);
    wrefresh(iWin);
    wrefresh(mapWin);
}

void createWindows(int yMax,int xMax){
    //windows Created    
        //resourse window
        rWin = newwin(yMax*5/8,xMax*3/8,0,0); 
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
        mapWin = newwin(yMax*5/8,xMax*5/8,0,xMax*3/8); //new window
        box(mapWin,0,0); //create box
        refresh();
        wrefresh(mapWin);
        keypad(mapWin,true);
}

int gameLoop(Game * m){
    int mNum=-1;
    //-2 is main menu, -1 is sub menu, 0+ is a selection.
    do{
        m->display();
        refreshWindows();

        while(mNum == -1){
            m->display();
            refreshWindows(); 
            mNum = m->getmv();
        }

        if(mNum == -2){
            m->prepSubMenu();
        }
        
        while(mNum == -2){
            m->display();
            refreshWindows();
            mNum = m->mgetmv();
        }
        
        if(mNum ==-3){
            m->prepInputMenu();
        }

        while(mNum == -3){
            m->display();
            refreshWindows();
            mNum = m->imgetmv();
        }
    }while(mNum < 0);
    

    return 0;
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
    
    Game * m = new Game(mmWin,smWin,rWin,iWin);
    
    gameLoop(m);

    cout << m->getHighlight() << "   " << m->getHighlight2();
    delete m;
    endwin();
	return 0;
}