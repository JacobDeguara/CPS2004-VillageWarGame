
#include <ncurses.h>
#include <vector>
#include <string>
#include <tuple>
#include <iostream>
#include "Player.hpp"

#ifndef _Game_H_
#define _Game_H_
using std::string;
using std::vector;
class Game
{
private:
    //Menu Selection Needed Variables
    WINDOW * menuwin;
    WINDOW * submenuwin;
    WINDOW * reswin;
    WINDOW * inwin;
    int highlight, highlight2, highlight3; //this holds the higlighted menu
    struct { // this struct holds each menu  Name followed by its sub menus: Names and size
        string mName;
        vector<string> smNames;
        int choiceSize;
    }choices[4];
    int count=1;
    void choicesCreation(); //this is used for formulating the strings for Choice
    
public:

    Player p;
    //constuctor
    Game(WINDOW * menuWindow, WINDOW * submenuWindow, WINDOW * resourceWindow,WINDOW * inputWindow);
    
    //getters
    int getHighlight();
    int getHighlight2();
    //setters
        //so lonely

    int getmv();
    int mgetmv();
    int imgetmv();
    void display();
    void prepSubMenu();
    void prepInputMenu();
    
};

Game::Game(WINDOW * menuWindow, WINDOW * submenuWindow, WINDOW * resourceWindow,WINDOW * inputWindow){
    //initilize menus and highlights
    menuwin = menuWindow;
    submenuwin = submenuWindow;
    reswin = resourceWindow;
    inwin = inputWindow;
    highlight = 0;
    highlight2 =-1;
    highlight3 =-1;
    count = 1;
    //innitilizeing the menu system
    choicesCreation();

}

//this is where the menu strings are created
void Game::choicesCreation(){
    choices[0].mName = "> Create Building";
    string str[] ={"> Wood Cutter      ","> Stone Miner      ","> Iron Miner       ","> Battle Trainer   "};
    choices[0].smNames.assign(str,str+4);
    choices[0].choiceSize = 4;

    choices[1].mName = "> Create Troops  ";
    choices[2].mName = "> Upgrade Troops ";
    string str2[] = {"> Archery Range    ", "> Knighting Palace ", "> Defender Barracks" };
    choices[1].smNames.assign(str2,str2+3);
    choices[2].smNames.assign(str2,str2+3);
    choices[1].choiceSize = choices[2].choiceSize = 3;
    
    choices[3].mName = "> Actions        ";
    string str3[] = {"> Attack           ","> Skip             ","> Surrender        "};
    choices[3].smNames.assign(str3,str3+3);
    choices[3].choiceSize = 3;
}

// get choice changes for main menu
int Game::getmv(){
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
            if(highlight == 4){
                highlight = 3;
            }
            break;

        case KEY_RIGHT:
            return -2;
            break;
        default:
            break;
    }
    return -1; // -2 is default loop for main menu
}

void Game::prepSubMenu(){
    highlight2 = 0;
}

//get movement from sub menu
int Game::mgetmv(){
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
            return -1;
            break;
        
        case KEY_RIGHT:
            highlight3 = 0;
            return -3;
            break;
        default:
            break;
    }
    if(choice2 == 10){
        return 0; //choice selected
    }
    return -2; // -1 is default loop for sub menu
}

void Game::prepInputMenu(){
    highlight3 = 0;
    count = 1;
}

int Game::imgetmv(){
    if(highlight != 3 || ( highlight==3 && highlight2 ==1 )){
        int choice3 = wgetch(inwin);
        switch(choice3){
            case KEY_LEFT:
                highlight3--;
                if(highlight3 == -1){
                    count = 1;
                    return -2;
                }   
                break;

            case KEY_RIGHT:
                highlight3++;
                if(highlight3 > 3){
                    highlight3 = 2;
                }
                break;

            default:
                break;
        }

        if(choice3 == 10){
            if(highlight3==0){
                if(count != 1){
                    count--;
                }
            }else if(highlight3==2){
                count++;
            }else{
                return 0; //choice selected
            }
        }
        return -3;
    }
    highlight3 = -1;
    return -2;
}

void Game::display(){
    const char * str;
    int wood,stone,iron,food;    
    mvwprintw(menuwin,0,1,"Menu-(up/down/left)");
    for (int i = 0; i < 4; i++)
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

    //everything inWin related
    mvwprintw(inwin,0,1,"Inputs-Menu");
    if(highlight != 3){
        switch(highlight2){
            case 0:
                if(highlight==0 || highlight ==2){ // wood cutter
                    wood = 2*count,stone = 2*count, iron = 0*count, food = 0*count; //cheapest building
                    mvwprintw(inwin,1,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }else if(highlight==1){ // Archer building
                    wood = 3*count,stone = 2*count, iron = 2*count, food = 0*count;    // more wood
                    mvwprintw(inwin,1,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }else if(highlight ==3){ // Archer troop
                    wood = 2*count,stone = 0*count, iron = 2*count, food = 1*count;   
                    mvwprintw(inwin,1,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }
            break;
            case 1:
                if(highlight==0 || highlight ==2){ // stone miner
                    wood = 4*count,stone = 0*count, iron = 2*count, food = 0*count; //stone miner so no stone needed so must balance
                    mvwprintw(inwin,1,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }else if(highlight==1){ // Knight building
                    wood = 2*count,stone = 2*count, iron = 3*count, food = 0*count; //more iron
                    mvwprintw(inwin,1,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }else if(highlight ==3){ // Knight troop
                    wood = 2*count,stone = 0*count, iron = 2 *count, food = 1*count;   
                    mvwprintw(inwin,1,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }
            break;    
            case 2:
                if(highlight==0 || highlight ==2){ // iron miner
                    wood = 3*count,stone = 3*count, iron = 2*count, food = 0*count; //most expensive since iron is used everywhere must be scares for iron players
                    mvwprintw(inwin,1,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }else if(highlight==1){ // defender building
                    wood = 2*count,stone = 3*count, iron = 2*count, food = 0*count;  //more stone
                    mvwprintw(inwin,1,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }else if(highlight ==3){ // defender troop
                    wood = 2*count,stone = 0*count, iron = 2*count, food = 1*count;   
                    mvwprintw(inwin,1,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }
            break;
            case 3:
                if(highlight==0 || highlight ==2){ // Battle Trainer
                    wood = 2*count,stone = 3*count, iron = 1*count, food = 0*count;
                    mvwprintw(inwin,1,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }
            break;
                case -1:
                    mvwprintw(inwin,1,1,"                                                   ");
                    break;
            }

            if(highlight2 != -1){
                if(highlight3 == 0){
                    wattron(inwin,A_REVERSE);
                }
                mvwprintw(inwin,3,1,"<");
                wattroff(inwin,A_REVERSE);
                
                if(highlight3 == 1){
                    wattron(inwin,A_REVERSE);
                }
                wprintw(inwin,"\t%d\t",count);
                wattroff(inwin,A_REVERSE);
                
                if(highlight3 == 2){
                    wattron(inwin,A_REVERSE);
                }
                wprintw(inwin,">");
                wattroff(inwin,A_REVERSE);
            }
    }else{
        mvwprintw(inwin,3,1," \t   \t ");
    }
    
    if(highlight == 3 && highlight2 == 0 ){
        if(highlight2 != -1){
            if(highlight3 == 0){
                wattron(inwin,A_REVERSE);
            }
            mvwprintw(inwin,3,1,"<");
            wattroff(inwin,A_REVERSE);
            
            if(highlight3 == 1){
                wattron(inwin,A_REVERSE);
            }
            wprintw(inwin,"\tW.I.P\t",count);
            wattroff(inwin,A_REVERSE);
            
            if(highlight3 == 2){
                wattron(inwin,A_REVERSE);
            }
            wprintw(inwin,">");
            wattroff(inwin,A_REVERSE);
        }
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

int Game::getHighlight(){
    return highlight;
}

int Game::getHighlight2(){
    return highlight2;
}

#endif