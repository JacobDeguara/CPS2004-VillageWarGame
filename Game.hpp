
#include <ncurses.h>
#include <vector>
#include <string>
#include "Player.hpp"
#include "ViewController.hpp"

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
    
    vector<Player *> p;
    ViewController * VC;
    int playerNum;
    int wood,stone,iron,food; //this represents the cost of the respective selelected type
public:

    /*constuctor*/
    Game(int NumOfPlayers,WINDOW * menuWindow, WINDOW * submenuWindow, WINDOW * resourceWindow,WINDOW * inputWindow,WINDOW * mapWindow); //constuctor
    ~Game();
    /*display*/
    //getMenuMove()

    int getmv(); /*get move menu*/  
    int mgetmv(); /*get move sub menu */ 
    int imgetmv(); /* get move input menu */
    void prepSubMenu(); /* prepare the sub menu (needed before mgetmv()) */
    void prepInputMenu(); /* prepare the input menu (needed before imgetmv()) */
    void resetMenu(); /* resets the menu system */
    //display()
    
    void display(); /* display Windows */
    
    int actionTaken(); // will take the selected action and run it
    void endRound();

    int gameLoop();
};

Game::Game(int NumOfPlayers,WINDOW * menuWindow, WINDOW * submenuWindow, WINDOW * resourceWindow,WINDOW * inputWindow,WINDOW * mapWindow){
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
    p.assign(NumOfPlayers,new Player());
    VC = new ViewController(menuWindow,submenuWindow,resourceWindow,inputWindow,mapWindow);

}

Game::~Game(){
    for(auto i : p){
        delete(i);
    }
    p.clear();
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
// removed since this Has already been moved to ViewController
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
                    food = 1*count + 1*(p[playerNum]->troops[highlight2]->getLevel());  
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
                    food = 1*count * (p[playerNum]->troops[highlight2]->getLevel());    
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
                    food = 2*count * (p[playerNum]->troops[highlight2]->getLevel()); 
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
    
}

/* > Runs the action selected
* if player unsuccessfull or takes an action returns false (0)
* if player takes an action returns true (1) (this is so that the end of the round is declared)
* if player want to exit (surrender) returns exit (-1)
*/
int Game::actionTaken(){
    int choice1 = VC->getChoice1();
    int choice2 = VC->getChoice2();
    switch(choice1){
        case 0:
            return p[playerNum]->increaseBuilding(VC->getCount(),choice2,VC->getWoodCost(),VC->getStoneCost(),VC->getIronCost(),VC->getFoodCost());
        break;
        case 1:
            return p[playerNum]->upgradeBuilding(VC->getCount(),choice2,VC->getWoodCost(),VC->getStoneCost(),VC->getIronCost(),VC->getFoodCost());
        break;
        case 2:
            return p[playerNum]->upgradeTroops(VC->getCount(),choice2,VC->getWoodCost(),VC->getStoneCost(),VC->getIronCost(),VC->getFoodCost()); 
        break;
            //these will always return false ^^^ (= 0)
        case 3:     
            if(choice2 == 0){ //attack protocal (not done yet)
                return 1;
            }
            if(choice2 == 1){ //end turn protocal
                return 1;
            }
            if(choice2 == 2){
                return -1;
            }
        default:
            return false;
        
    }
}

void Game::endRound(){
    p[playerNum]->RoundEnd();
}

int Game::gameLoop(){
    int whichMenuNum = -1;
    bool dontExit = true;
    do{
        VC->resetMenu();
        whichMenuNum = -1;
        do{
            VC->update(p[playerNum]);
            VC->refresh();
            while(whichMenuNum == -1){
                VC->update(p[playerNum]);
                VC->refresh();   
                whichMenuNum = VC->getMenuMV();
            }

            if(whichMenuNum == -2){
                VC->prepSubMenu();
            }

            while(whichMenuNum == -2){
                VC->update(p[playerNum]);
                VC->refresh();
                whichMenuNum = VC->getSubMenuMV();
            }   

            if(whichMenuNum == -3){
                VC->prepInputMenu();
            }

            while(whichMenuNum == -3){
                VC->update(p[playerNum]);
                VC->refresh();
                whichMenuNum = VC->getInputMenuMV();
            }
            
        }while(whichMenuNum < 0);

        whichMenuNum = actionTaken();
        if(whichMenuNum == 0){
            //do nothing
        }else if(whichMenuNum == 1){ //ended the his turn
            endRound();
        }else if(whichMenuNum == -1){ // surrendered
            dontExit = false;
        }

    }while(dontExit);
    return 0;
}

#endif