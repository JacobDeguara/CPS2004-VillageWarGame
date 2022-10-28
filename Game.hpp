
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
    void costCreation(int wood,int stone,int iron,int food); // this is used to formulate the cost for display
    vector<Player> p;
    int playerNum;
    int wood,stone,iron,food; //this represents the cost of the respective selelected type
public:

    /*constuctor*/
    Game(int NumOfPlayers,WINDOW * menuWindow, WINDOW * submenuWindow, WINDOW * resourceWindow,WINDOW * inputWindow); //constuctor

    /*display*/
    int getmv(); // get move menu 
    int mgetmv(); // get move sub menu
    int imgetmv(); // get move input menu
    void display(); // display Windows
    void prepSubMenu(); // prepare the sub menu (needed before mgetmv())
    void prepInputMenu(); // prepare the input menu (needed before imgetmv())
    void resetMenu(); //resets the menu system
    
    int actionTaken(); // will take the selected action and run it
    void endRound();
};

Game::Game(int NumOfPlayers,WINDOW * menuWindow, WINDOW * submenuWindow, WINDOW * resourceWindow,WINDOW * inputWindow){
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
    playerNum = 0;
    p.assign(NumOfPlayers,Player());
}

//this is where the menu strings are created
void Game::choicesCreation(){
    choices[0].mName = "> Create Building";
    choices[1].mName = "> Upgrade Building";
    string str[] ={"> Wood Cutter      ","> Stone Miner      ","> Iron Miner       ","> Battle Trainer   ","> Archery Range    ", "> Knighting Palace ", "> Defender Barracks"};
    choices[0].smNames.assign(str,str+7);
    choices[1].smNames.assign(str,str+7);
    choices[0].choiceSize = choices[1].choiceSize = 7;

    choices[2].mName = "> Upgrade Troops ";
    string str2[] = {"> Archer           ", "> Knight           ", "> Defender         " };
    choices[2].smNames.assign(str2,str2+3);
    choices[2].choiceSize = 3;
    
    choices[3].mName = "> Actions        ";
    string str3[] = {"> Attack           ","> Skip             ","> Surrender        "};
    choices[3].smNames.assign(str3,str3+3);
    choices[3].choiceSize = 3;
}

void Game::costCreation(int wood,int stone,int iron,int food){
    this->wood = wood*count * (p[playerNum].Buildings[highlight2].getamount()+p[playerNum].Buildings[highlight2].getLevel());
    this->stone = stone*count *(p[playerNum].Buildings[highlight2].getamount()+p[playerNum].Buildings[highlight2].getLevel());
    this->iron = iron*count * (p[playerNum].Buildings[highlight2].getamount()+p[playerNum].Buildings[highlight2].getLevel());
    this->food = food*count * (p[playerNum].Buildings[highlight2].getamount()+p[playerNum].Buildings[highlight2].getLevel());
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

void Game::resetMenu(){
    highlight = 0;
    highlight2 = -1;
    highlight3 = -1;
    count = 1;
}

void Game::display(){
    const char * str; 
    //menu window  
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

    //sub menu window
    mvwprintw(submenuwin,0,1,"Submenu-(up/down/enter)");
    for (int i = 0; i < 7; i++)
    {   
        if(i >= choices[highlight].smNames.size()){
            mvwprintw(submenuwin,i+1,1,"                   "); //remove string
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
                if(highlight==0){  
                    costCreation(2,2,0,0);                                                                                 //create wood cutter
                    mvwprintw(inwin,1,1,"Creates Wood             ");
                    mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }else if(highlight==1){                                                                             // Upgrade wood cutter
                    costCreation(2,2,2,1);
                    mvwprintw(inwin,1,1,"Upgrading the Wood Cutter");
                    mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }else if(highlight ==2){                                                                            // Archer troop
                    wood = 0;
                    stone = 0;
                    iron = 0;
                    food = 1*count * (p[playerNum].troops[highlight2].getLevel());  
                    mvwprintw(inwin,1,1,"Train the Archer         ");
                    mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }
                break;
            case 1:
                if(highlight==0){ 
                    costCreation(2,0,2,0);//create stone miner
                    mvwprintw(inwin,1,1,"Creates Stone            ");
                    mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }else if(highlight==1){// Upgrade stone miner
                    costCreation(3,0,3,1);
                    mvwprintw(inwin,1,1,"Upgrade Stone Miner      ");
                    mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }else if(highlight ==2){                                                                            // Knight troop
                    wood = 0;
                    stone = 0;
                    iron = 0;
                    food = 1*count * (p[playerNum].troops[highlight2].getLevel());    
                    mvwprintw(inwin,1,1,"Train Knight             ");
                    mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }
                break;    
            case 2:
                if(highlight==0){
                    costCreation(3,2,0,0);                                                                                 //create iron miner
                    mvwprintw(inwin,1,1,"Creates Iron             ");
                    mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }else if(highlight==1){                                                                             // Upgrade iron miner
                    costCreation(3,3,0,1);
                    mvwprintw(inwin,1,1,"Upgrade the Iron Miner   ");
                    mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }else if(highlight ==2){                                                                            // defender troop
                    wood = 0;
                    stone = 0;
                    iron = 0;
                    food = 2*count * (p[playerNum].troops[highlight2].getLevel()); 
                    mvwprintw(inwin,1,1,"Train Defender           ");
                    mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }
                break;
            case 3:
                if(highlight==0){                                                                                   //create Battle Trainer
                    costCreation(2,2,2,0);
                    mvwprintw(inwin,1,1,"Creates Food             ");
                    mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }else if(highlight==1){                                                                             // Upgrade Battle Trainer
                    costCreation(2,2,2,0);
                    mvwprintw(inwin,1,1,"Upgrade the BattleTower  ");
                    mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }
                break;
            case 4:
                if(highlight==0){                                                                                   //create Archer place
                    costCreation(2,2,0,2);
                    mvwprintw(inwin,1,1,"Creates Archers          ");
                    mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }else if(highlight==1){  
                    costCreation(2,0,2,1);                                                                           // Upgrade Archer place
                    mvwprintw(inwin,1,1,"Upgrade The Range        ");
                    mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }
                break;            
            case 5:
                if(highlight==0){  
                    costCreation(1,1,1,2);                                                                                 //create Knight place
                    mvwprintw(inwin,1,1,"Creates Knights          ");
                    mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }else if(highlight==1){                                                                             // Upgrade Knight place
                    costCreation(1,1,1,1);
                    mvwprintw(inwin,1,1,"Upgrades The Palace      ");
                    mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }
                break;
            case 6:
                if(highlight==0){   
                    costCreation(1,1,1,3);                                                                                //create Defender place
                    mvwprintw(inwin,1,1,"Creates Defenders        ");
                    mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }else if(highlight==1){                                                                             // Upgrade Defender place
                    costCreation(1,2,1,1);
                    mvwprintw(inwin,1,1,"Upgrades the Barackes    ");
                    mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
                }
                break;
            case -1:
                    mvwprintw(inwin,1,1,"                                                  ");
                    mvwprintw(inwin,2,1,"                                                  ");
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
            }else{
                mvwprintw(inwin,3,1," \t   \t ");
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
    for(int i = 0; i < p[playerNum].getResSize(); i++)
    {   
        int num;
        switch(i){
            case 0:
                str = "Wood : ";
                num = p[playerNum].getWood();
                break;
            case 1:
                str = "Stone: ";
                num = p[playerNum].getStone();
                break;
            case 2:
                str = "Iron : ";
                num = p[playerNum].getIron();
                break;
            case 3:
                str = "Food : ";
                num = p[playerNum].getFood();
                break;
            case 4:
                break;
            default:
            break;
        }
        mvwprintw(reswin,i+1,1,"%s%d    ",str,num);
    }

    mvwprintw(reswin,1+p[playerNum].getResSize(),1,"Buildings:");
    for(int i = 0; i < 7; i++)
    {   
        int num1,num2;
        str = choices[0].smNames[i].data();
        num1 = p[playerNum].Buildings[i].getamount();
        num2 = p[playerNum].Buildings[i].getLevel();
        mvwprintw(reswin,i+2+p[playerNum].getResSize(),1,"%s: Qty:%d Lvl:%d Gen:%d",str,num1,num2,p[playerNum].getGen(i));
    }

    mvwprintw(reswin,9+p[playerNum].getResSize(),1,"Troops:");
    for(int i = 0; i < 3; i++)
    {   
        int num1,num2,num3;
        str = choices[2].smNames[i].data();
        num1 = p[playerNum].troops[i].getAmount();
        num2 = p[playerNum].troops[i].getLevel();
        num3 = p[playerNum].troops[i].getPowerRating();
        mvwprintw(reswin,i+10+p[playerNum].getResSize(),1,"%s: Qty:%d Lvl:%d Power:%d",str,num1,num2,num3);
    }
    
}

/* > Runs the action selected
* if unsuccessfull returns false (0)
* if successfull returns true(1)
* if want to exit (surrender) returns exit(-1)
*/
int Game::actionTaken(){  
    if(highlight == 0){ //create Building
        return p[playerNum].increaseBuilding(count,highlight2,wood,stone,iron,food);
    }else if(highlight == 1){//upgrade building
        return p[playerNum].upgradeBuilding(count,highlight2,wood,stone,iron,food);
    }else if(highlight == 2){//upgrade troops   
        return p[playerNum].upgradeTroops(count,highlight2,wood,stone,iron,food);
    }else if(highlight == 3){//actions
        if(highlight2==0){ //action
            //attack protocall
            return 0;
        }else if(highlight2 ==1){ //
            //skipp protocall
            return 1;
        }else if(highlight2 ==2){  
            //surrender protocall
            return -1;
        }
    }
    return false;
}

void Game::endRound(){
    p[playerNum].RoundEnd();
}
#endif