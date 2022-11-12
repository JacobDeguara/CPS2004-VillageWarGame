
#include <ncurses.h>
#include <vector>
#include <string>
#include "Player.hpp"
#include "Map.hpp"
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
    Map * M;
    
    WINDOW * VisualCreation();

    //Players Resaources and Manipulation
    vector<Player> p;
    
    struct {
        int Max;
        int Current;
        vector<int> removed;
    }PlayersNumbers;

    void StartGame();
    
    int actionTaken(); // will take the selected action and run it
    void endRound();

    int attackProtocal();

    void menuLoop(int &whichMenu);

public:
    /*constuctor*/
    Game(); //constuctor
    ~Game();
    
    int gameLoop();

    int getNum();
};

Game::Game(){

    VC = new ViewController();

}

//This should be done before the game
void Game::StartGame(){
    int numOfPlayer,numOfAi;

    VC->updateStart(numOfPlayer,numOfAi);

    PlayersNumbers.Current = 0;
    PlayersNumbers.Max = numOfPlayer;
    for(int i=0;i < PlayersNumbers.Max;i++){
        p.push_back(Player());
        p[i].setID(i);
    }

    M = new Map(PlayersNumbers.Max);
}

Game::~Game(){
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
            return p[PlayersNumbers.Current].increaseBuilding(VC->getCount(),choice2,VC->getWoodCost(),VC->getStoneCost(),VC->getIronCost(),VC->getFoodCost());
        break;
        case 1:
            return p[PlayersNumbers.Current].upgradeBuilding(VC->getCount(),choice2,VC->getWoodCost(),VC->getStoneCost(),VC->getIronCost(),VC->getFoodCost());
        break;
        case 2:
            return p[PlayersNumbers.Current].upgradeTroops(VC->getCount(),choice2,VC->getWoodCost(),VC->getStoneCost(),VC->getIronCost(),VC->getFoodCost()); 
        break;
            //these will always return false ^^^ (= 0)
        case 3:     
            if(choice2 == 0){ //attack protocal (not done yet)
                return 2;
            }
            if(choice2 == 1){ //end turn protocal
                return 1;
            }
            if(choice2 == 2){
                return -1;
            }
        default:
            return 0;
        
    }
}

void Game::endRound(){
    p[PlayersNumbers.Current].RoundEnd();
}

int Game::gameLoop(){
    int whichMenuNum = -1;
    bool dontExit = true, notSurrendered = true;
    
    StartGame();
    
    do{
        //if true do game loop 
        if(notSurrendered == true){
            dontExit = true;
            //this is the menu loops
            VC->updateMap(M);
            VC->resetMenu();
            do{
                
                whichMenuNum = VC->resetMenuPartial();
                
                menuLoop(whichMenuNum);

                whichMenuNum = actionTaken();
                if(whichMenuNum == 0){
            
                }else if(whichMenuNum == 1){ //ended the his turn
                    endRound();
                    PlayersNumbers.Current++;
                    if(PlayersNumbers.Current == PlayersNumbers.Max){
                        PlayersNumbers.Current = 0;   
                    }
                    dontExit = false;
                    VC->resetMenu();
                }else if(whichMenuNum == -1){ // surrendered
                    dontExit = false;
                    PlayersNumbers.Max--;
                    //remove player from Map
                    M->PlayerIsDead(p.at(PlayersNumbers.Current).getID());

                    p.erase(p.begin() + PlayersNumbers.Current);
                    if(PlayersNumbers.Current == PlayersNumbers.Max){
                        PlayersNumbers.Current = 0;   
                    }
                }else if(whichMenuNum == 2){ //attack protocal
                    
                    
                    VC->resetMenu();
                    endRound();
                }

            }while(dontExit);//end of menu loop

        }//end of if check for player exist

    }while(PlayersNumbers.Max > 1);//end of loop for next player
    return 0;
}

int Game::getNum(){
    return 0;
}

void Game::menuLoop(int &whichMenuNum){
    do{
        VC->update(&p[PlayersNumbers.Current],PlayersNumbers.Current,M);
        VC->refresh();
        while(whichMenuNum == -1){
        VC->update(&p[PlayersNumbers.Current],PlayersNumbers.Current,M);
        VC->refresh();   
        whichMenuNum = VC->getMenuMV();
        }

        if(whichMenuNum == -2){
            VC->prepSubMenu();
        }

        while(whichMenuNum == -2){
            VC->update(&p[PlayersNumbers.Current],PlayersNumbers.Current,M);
            VC->refresh();
            whichMenuNum = VC->getSubMenuMV();
        }   

        if(whichMenuNum == -3){
            VC->prepInputMenu(M);
        }

        while(whichMenuNum == -3){
            VC->update(&p[PlayersNumbers.Current],PlayersNumbers.Current,M);
            VC->refresh();
            whichMenuNum = VC->getInputMenuMV(M);
        }
                    
    }while(whichMenuNum < 0);
}

#endif