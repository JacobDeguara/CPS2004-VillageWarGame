
#include <ncurses.h>
#include <bits/stdc++.h>
#include <vector>

#ifndef _MAP_H_
#define _MAP_H_
using namespace std;

class Map
{
private:
    int h,w;

    //Menu Selection Needed Variables
    WINDOW * menuwin;
    WINDOW * submenuwin;
    int highlight, highlight2; //this holds the higlighted menu
    int choice, choice2; // this holds the choice done by the user
    const char * str; // this is a temporary const char for printing
    struct { // this struct holds each menu  Name followed by its sub menus: Names and size
        string mName;
        string smNames[5];
        int choiceSize;
    }choices[5];
    void choicesCreation(); //this is used for formulating the strings for Choice



public:

    Map(WINDOW * menuWindow, WINDOW * submenuWindow, WINDOW * resourceWindow);
    //getters
    int getHighlight();
    int getHighlight2();
    //setters
        //so lonely

    int getmv();
    void display();
    int mgetmv();
    void prepSubMenu();
    
};

Map::Map(WINDOW * menuWindow, WINDOW * submenuWindow, WINDOW * resourceWindow){
    //initilize menus and highlights
    menuwin = menuWindow;
    submenuwin = submenuWindow;
    highlight = 0;
    highlight2 =-1;
    //innitilizeing the menu system
    choicesCreation();

    getmaxyx(menuwin,h,w);
    keypad(menuwin,true);

}

//this is where the menu strings are created
void Map::choicesCreation(){
    choices[0].mName = "> Create Building";
    choices[2].mName = "> Upgrade Building";
    choices[0].smNames[0] = choices[2].smNames[0] = "> Wood Cutter   ";
    choices[0].smNames[1] = choices[2].smNames[1] = "> Stone Miner   ";
    choices[0].smNames[2] = choices[2].smNames[2] = "> Iron Miner    ";
    choices[0].smNames[3] = choices[2].smNames[3] = "> Butcher       ";
    choices[0].smNames[4] = choices[2].smNames[4] = "> Battle Trainer";
    choices[0].choiceSize = choices[2].choiceSize = 5;

    choices[1].mName = "> Create Troops";
    choices[3].mName = "> Upgrade Troops";
    choices[1].smNames[0] = choices[3].smNames[0] = "> Archer        ";
    choices[1].smNames[1] = choices[3].smNames[1] = "> Knight        ";
    choices[1].smNames[2] = choices[3].smNames[2] = "> Defender      ";
    choices[1].choiceSize = choices[3].choiceSize = 3;
    
    choices[4].mName = "> Actions";
    choices[4].smNames[0] = "> Attack         ";
    choices[4].smNames[1] = "> Skip           ";
    choices[4].smNames[2] = "> Surrender      ";
    choices[4].choiceSize = 3;
}

// get choice changes for main menu
int Map::getmv(){
    int choice = wgetch(menuwin);
    switch(choice){
        case KEY_UP:
            highlight--;
            if(highlight == -1){
                highlight = 0;
            }
            break;

        case KEY_DOWN:
            highlight++;
            if(highlight == 5){
                highlight = 4;
            }
            break;

        case KEY_RIGHT:
            return -1;
            break;
        default:
            break;
    }
    return -2; // -2 is default loop for main menu
}

void Map::prepSubMenu(){
    highlight2 = 0;
}

//get movement from sub menu
int Map::mgetmv(){
    int choice2 = wgetch(submenuwin);
    switch(choice2){
        case KEY_UP:
            highlight2--;
            if(highlight2 == -1){
                highlight2 = 0;
            }
            break;

        case KEY_DOWN:
            highlight2++;
            if(highlight2 == (choices[highlight].choiceSize)){
                highlight2 = (choices[highlight].choiceSize)-1;
            }
            break;

        case KEY_LEFT:
            highlight2 = -1;
            return -2;
            break;
        default:
            break;
    }
    if(choice2 == 10){
        return 0; //choice selected
    }
    return -1; // -1 is default loop for sub menu
}


void Map::display(){
    for (int i = 0; i < 5; i++)
    {
        if(i == highlight){ //if highlighted
            wattron(menuwin,A_REVERSE);
        }
        str = (choices[i].mName).c_str();
        mvwprintw(menuwin,i+1,1,str); //print string
        wattroff(menuwin,A_REVERSE);    
    }

    for (int i = 0; i < 5; i++)
    {   
        if(choices[highlight].smNames[i]!=""){ //if not empty
            if(i == highlight2){ //if highlighted
                wattron(submenuwin,A_REVERSE);
            }
            str = (choices[highlight].smNames[i]).c_str();
            mvwprintw(submenuwin,i+1,1,str); //print string
        }else{
            mvwprintw(submenuwin,i+1,1,"                  "); //remove string
        }
        
        wattroff(submenuwin,A_REVERSE);    
    }
}

int Map::getHighlight(){
    return highlight;
}
int Map::getHighlight2(){
    return highlight2;
}

#endif