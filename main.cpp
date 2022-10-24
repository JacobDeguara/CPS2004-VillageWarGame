#include <ncurses.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Map.h"

using namespace std;
int main(void){	
	initscr(); 
    noecho();
    cbreak();
    curs_set(0);

    //get screen size
    int yMax, xMax;
    getmaxyx(stdscr,yMax,xMax);

    //windows Created    
        //resourse window
        WINDOW * rWin = newwin(yMax*5/8,xMax*3/8,0,0); 
        box(rWin,0,0); //create box
        refresh();
        wrefresh(rWin);
        keypad(rWin,true);

        //main menu Window
        WINDOW * mmWin = newwin((yMax*3/8)+1,xMax*3/8,yMax*5/8,0); //new window
        box(mmWin,0,0); //create box
        refresh();
        wrefresh(mmWin);
        keypad(mmWin,true);

        //sub menu Window
        WINDOW * smWin = newwin((yMax*3/8)+1,xMax*3/8,yMax*5/8,xMax*3/8); //new window
        box(smWin,0,0); //create box
        refresh();
        wrefresh(smWin);
        keypad(smWin,true);
        
        //Input Window
        WINDOW * iWin = newwin((yMax*3/8)+1,xMax*2/8,yMax*5/8,xMax*6/8); //new window
        box(iWin,0,0); //create box
        refresh();
        wrefresh(iWin);
        keypad(iWin,true);

        //Map Window
        WINDOW * mapWin = newwin(yMax*5/8,xMax*5/8,0,xMax*3/8); //new window
        box(mapWin,0,0); //create box
        refresh();
        wrefresh(mapWin);
        keypad(mapWin,true);
    //Creation
    
    Map * m = new Map(mmWin,smWin,rWin);
    int mNum=-2;

    //-2 is main menu, -1 is sub menu, 0+ is a selection.
    do{
        do{
            m->display();
            wrefresh(mmWin);
            wrefresh(smWin);
            mNum = m->getmv();
        }while(mNum == -2);
        
        m->prepSubMenu();

        do{
            m->display();
            wrefresh(mmWin);
            wrefresh(smWin);
            mNum = m->mgetmv();
        }while(mNum == -1);
    }while(mNum < 0);

    cout << m->getHighlight() << "   " << m->getHighlight2();
    endwin();
	return 0;
}