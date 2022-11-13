
#include <ncurses.h>
#include <vector>
#include <string>
#include <memory>

#include "Player.hpp"
#include "Map.hpp"
#include "ViewController.hpp"
#include "Attack.hpp"

#ifndef _Game_H_
#define _Game_H_
using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;

class Game
{
private:

    //Visual & Control
    shared_ptr<ViewController> VC = make_shared<ViewController>();
    shared_ptr<Map> M;
    Attack ATKlist;
    
    WINDOW * VisualCreation();

    //Players Resaources and Manipulation
    vector<Player> p;
    
    struct {
        int Max;
        int Current;
    }PlayersNumbers;

    void StartGame();
    
    int actionTaken(); // will take the selected action and run it
    void endRound();

    bool attackProtocal();

    void menuLoop(int &whichMenu);

public:
    /*constuctor*/
    Game(); //constuctor
    ~Game();
    
    int gameLoop();

    int getNum();
};

Game::Game(){
    ATKlist = Attack();
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

    M = make_shared<Map>(PlayersNumbers.Max);
}

Game::~Game(){
    p.clear();
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
                if(attackProtocal()){
                    return 1;
                }
                return 0;
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
    int attackLog = -1;
    
    StartGame();
    VC->sendMsg("You have taken 10 Damage!");
    do{
        //if true do game loop 
        if(notSurrendered == true){
            dontExit = true;

            //this is the menu loops
            VC->updateMap(M);
            VC->resetMenu();
            ATKlist.NextTurn();
            VC->update(&p[PlayersNumbers.Current],PlayersNumbers.Current,M);
            VC->refresh();

            attackLog = ATKlist.AmIBeingAttacked(p[PlayersNumbers.Current].getID());
            if(attackLog != -1){
                VC->StartAttack(ATKlist.getAttack(attackLog));
            }

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
                }else if(whichMenuNum == -1){ // surrendered
                    dontExit = false;
                    PlayersNumbers.Max--;
                    //remove player from Map
                    M->PlayerIsDead(p.at(PlayersNumbers.Current).getID());

                    p.erase(p.begin() + PlayersNumbers.Current);
                    if(PlayersNumbers.Current == PlayersNumbers.Max){
                        PlayersNumbers.Current = 0;   
                    }
                }

            }while(dontExit);//end of menu loop

        }//end of if check for player exist

    }while(PlayersNumbers.Max > 1);//end of loop for next player
    return 0;
}

int Game::getNum(){
    return ATKlist.Size();
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
};

bool Game::attackProtocal(){
    if(VC->getPlayerCount() == PlayersNumbers.Current){
        return false;
    }
    if(p[PlayersNumbers.Current].troops[0]->getAmount() < VC->getArcherCount() || VC->getArcherCount() == 0){
        return false;
    }
    if(p[PlayersNumbers.Current].troops[1]->getAmount() < VC->getKnightCount() || VC->getKnightCount() == 0){
        return false;
    }
    if(p[PlayersNumbers.Current].troops[2]->getAmount() < VC->getDefenderCount() || VC->getDefenderCount() == 0){
        return false;
    }
    
    p[PlayersNumbers.Current].troops[0]->RemoveTroops(VC->getArcherCount());
    p[PlayersNumbers.Current].troops[1]->RemoveTroops(VC->getKnightCount());
    p[PlayersNumbers.Current].troops[2]->RemoveTroops(VC->getDefenderCount());

    ATKlist.createNewAttack(&p[PlayersNumbers.Current],M,VC->getPlayerCount(),PlayersNumbers.Current,VC->getArcherCount(),VC->getKnightCount(),VC->getDefenderCount(),VC->getAttackChoice());
    return true;
};

#endif