
#include <ncurses.h>
#include <bits/stdc++.h>
#include <vector>
#include <iostream>
#include "Player.h"

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
    WINDOW * reswin;
    int highlight, highlight2; //this holds the higlighted menu
    int choice, choice2; // this holds the choice done by the user
    struct { // this struct holds each menu  Name followed by its sub menus: Names and size
        string mName;
        vector<string> smNames;
        int choiceSize;
    }choices[5];
    void choicesCreation(); //this is used for formulating the strings for Choice
    
public:

    Player p;

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
    reswin = resourceWindow;
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
    
    string str[] ={"> Wood Cutter   ","> Stone Miner   ","> Iron Miner    ","> Butcher       ","> Battle Trainer"};
    choices[0].smNames.assign(str,str+5);
    choices[2].smNames.assign(str,str+5);
    choices[0].choiceSize = choices[2].choiceSize = 5;

    choices[1].mName = "> Create Troops";
    choices[3].mName = "> Upgrade Troops";
    string str2[] = {"> Archer        ", "> Knight        ", "> Defender      " };
    choices[1].smNames.assign(str2,str2+3);
    choices[3].smNames.assign(str2,str2+3);
    choices[1].choiceSize = choices[3].choiceSize = 3;
    
    choices[4].mName = "> Actions";
    string str3[] = {"> Attack         ","> Skip           ","> Surrender      "};
    choices[4].smNames.assign(str3,str3+3);
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
            if(highlight2 == choices[highlight].smNames.size()){
                highlight2 = choices[highlight].smNames.size() - 1;
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
    const char * str; 
    mvwprintw(menuwin,0,1,"Menu-(up/down/left)");
    for (int i = 0; i < 5; i++)
    {
        if(i == highlight){ //if highlighted
            wattron(menuwin,A_REVERSE);
        }
        str = (choices[i].mName).c_str();
        mvwprintw(menuwin,i+1,1,str); //print string
        wattroff(menuwin,A_REVERSE);    
    }

    mvwprintw(submenuwin,0,1,"Submenu-(up/down/enter)");
    for (int i = 0; i < 5; i++)
    {   
        if(i >= choices[highlight].smNames.size()){
            mvwprintw(submenuwin,i+1,1,"                  "); //remove string
            continue;
        }

        if(choices[highlight].smNames[i]!=""){ //if not empty
            if(i == highlight2){ //if highlighted
                wattron(submenuwin,A_REVERSE);
            }
            str = (choices[highlight].smNames[i].data());
            mvwprintw(submenuwin,i+1,1,str); //print string
        }
        
        wattroff(submenuwin,A_REVERSE);    
    }

    mvwprintw(reswin,0,1,"Resources");
    for(int i = 0; i < p.getResSize(); i++)
    {   
        string t;
        int num;
        switch(i){
            case 0:
                str = "Wood  :";
                num = p.getWood();
                break;
            case 1:
                str = "Stone :";
                num = p.getStone();
                break;
            case 2:
                str = "Iron  :";
                num = p.getIron();
                break;
            case 3:
                str = "Food  :";
                num = p.getFood();
                break;
            case 4:
                break;
            default:
            break;
        }
        mvwprintw(reswin,i+1,1,"%s%d",str,num);

    }
}

int Map::getHighlight(){
    return highlight;
}

int Map::getHighlight2(){
    return highlight2;
}

#endif