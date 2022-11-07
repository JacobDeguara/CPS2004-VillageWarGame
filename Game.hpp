
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

    //Visual & Control
    ViewController * VC;

    //Players Resaources and Manipulation
    vector<Player *> p;
    
    struct {
        int Max;
        int Current;
        vector<int> removed;
    }PlayersNumbers;

public:

    /*constuctor*/
    Game(int NumOfPlayers,WINDOW * menuWindow, WINDOW * submenuWindow, WINDOW * resourceWindow,WINDOW * inputWindow,WINDOW * mapWindow); //constuctor
    ~Game();
    
    int actionTaken(); // will take the selected action and run it
    void endRound();

    int getNum();
    int gameLoop();
};

Game::Game(int NumOfPlayers,WINDOW * menuWindow, WINDOW * submenuWindow, WINDOW * resourceWindow,WINDOW * inputWindow,WINDOW * mapWindow){
    
    PlayersNumbers.Current = 0;
    PlayersNumbers.Max = PlayersNumbers.Max = NumOfPlayers;
    for(int i=0;i < NumOfPlayers;i++){
        p.push_back(new Player());
    }
    VC = new ViewController(menuWindow,submenuWindow,resourceWindow,inputWindow,mapWindow);

}

Game::~Game(){
    p.push_back(new Player());
    delete(p[0]);
    p.clear();
    
    delete(VC);
}

/* > Runs the action selected through the VC interface
* if player unsuccessfull or takes an action returns false (0)
* if player takes an action returns true (1) (this is so that the end of the round is declared)
* if player want to exit (surrender) returns exit (-1)
*/
int Game::actionTaken(){
    int choice1 = VC->getChoice1();
    int choice2 = VC->getChoice2();
    switch(choice1){
        case 0:
            return p[PlayersNumbers.Current]->increaseBuilding(VC->getCount(),choice2,VC->getWoodCost(),VC->getStoneCost(),VC->getIronCost(),VC->getFoodCost());
        break;
        case 1:
            return p[PlayersNumbers.Current]->upgradeBuilding(VC->getCount(),choice2,VC->getWoodCost(),VC->getStoneCost(),VC->getIronCost(),VC->getFoodCost());
        break;
        case 2:
            return p[PlayersNumbers.Current]->upgradeTroops(VC->getCount(),choice2,VC->getWoodCost(),VC->getStoneCost(),VC->getIronCost(),VC->getFoodCost()); 
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
    p[PlayersNumbers.Current]->RoundEnd();
}

int Game::gameLoop(){
    int whichMenuNum = -1;
    bool dontExit = true, notSurrendered = true;
    do{
        //if true do game loop 
        if(notSurrendered == true){
            dontExit = true;
            //this is the menu loops
            do{
                VC->resetMenu();
                whichMenuNum = -1;
                do{
                    VC->update(p[PlayersNumbers.Current],PlayersNumbers.Current);
                    VC->refresh();
                    while(whichMenuNum == -1){
                        VC->update(p[PlayersNumbers.Current],PlayersNumbers.Current);
                        VC->refresh();   
                        whichMenuNum = VC->getMenuMV();
                    }

                    if(whichMenuNum == -2){
                        VC->prepSubMenu();
                    }

                    while(whichMenuNum == -2){
                        VC->update(p[PlayersNumbers.Current],PlayersNumbers.Current);
                        VC->refresh();
                        whichMenuNum = VC->getSubMenuMV();
                    }   

                    if(whichMenuNum == -3){
                        VC->prepInputMenu();
                    }

                    while(whichMenuNum == -3){
                        VC->update(p[PlayersNumbers.Current],PlayersNumbers.Current);
                        VC->refresh();
                        whichMenuNum = VC->getInputMenuMV();
                    }
                    
                }while(whichMenuNum < 0);

                whichMenuNum = actionTaken();
                if(whichMenuNum == 0){ //attack
                    //do nothing
                }else if(whichMenuNum == 1){ //ended the his turn
                    endRound();
                    PlayersNumbers.Current++;
                    if(PlayersNumbers.Current == PlayersNumbers.Max){
                        PlayersNumbers.Current = 0;   
                    }
                    dontExit = false;
                }else if(whichMenuNum == -1){ // surrendered
                    dontExit = false;
                    PlayersNumbers.Max--;
                    p.erase(p.begin() + PlayersNumbers.Current);
                    if(PlayersNumbers.Max >= PlayersNumbers.Current){
                       PlayersNumbers.Current = PlayersNumbers.Max -1;
                    }
                }

            }while(dontExit);//end of menu loop
        }//end of if check for player exist

    }while(PlayersNumbers.Max != 0);//end of loop for next player
    return 0;
}

int Game::getNum(){
    return PlayersNumbers.Max;
}

#endif