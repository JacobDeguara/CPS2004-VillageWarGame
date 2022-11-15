
#include <ncurses.h>
#include <vector>
#include <string>
#include <memory>
#include <cstdlib>
#include <ctime>

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
    Attack Atklist;

    //Players Resaources and Manipulation
    vector<Player> p;
    
    struct {
        int Max;
        int Current;
    }PlayersNumbers;

    void StartGame();
    int findPlayer(int ID);

    int actionTaken(); // will take the selected action and run it
    void endRound();

    bool attackProtocal();
    int attackOnTitan();
    void CheckPlayersAlive();

    void menuLoop(int &whichMenu);

public:
    /*constuctor*/
    Game(); //constuctor
    ~Game();
    
    int gameLoop();

    int getNum();
};

Game::Game(){
    Atklist = Attack();
}

int Game::findPlayer(int ID){
    for(int i=0; i < p.size(); i++){
        if(p[i].getID() == ID){
            return i;
        }
    }
    return -1;
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
    
    StartGame();
    do{
        //if true do game loop 
        Atklist.NextTurn();
        
        if(notSurrendered == true){
            dontExit = true;

            //this is the menu loops
            VC->updateMap(M);
            VC->resetMenu();
            VC->update(&p[PlayersNumbers.Current],PlayersNumbers.Current,M,Atklist);
            VC->refresh();

            attackOnTitan();

            CheckPlayersAlive();
            if(PlayersNumbers.Max <= 1){
                return 0;
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

                    Atklist.PlayerIsDied(p.at(PlayersNumbers.Current).getID());
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
    return Atklist.Size();
}

void Game::CheckPlayersAlive(){
    for (size_t i = 0; i < PlayersNumbers.Max; i++)
    {
        if(p[i].IsPlayerDead()){
        
            PlayersNumbers.Max--;

            Atklist.PlayerIsDied(p.at(i).getID());
            M->PlayerIsDead(p.at(i).getID());
                    
            p.erase(p.begin() + i);

            if(PlayersNumbers.Current == PlayersNumbers.Max){
                PlayersNumbers.Current = 0;   
            }
        }
    }
    
}

void Game::menuLoop(int &whichMenuNum){
    do{
        VC->update(&p[PlayersNumbers.Current],PlayersNumbers.Current,M,Atklist);
        VC->refresh();
        while(whichMenuNum == -1){
        VC->update(&p[PlayersNumbers.Current],PlayersNumbers.Current,M,Atklist);
        VC->refresh();   
        whichMenuNum = VC->getMenuMV();
        }

        if(whichMenuNum == -2){
            VC->prepSubMenu();
        }

        while(whichMenuNum == -2){
            VC->update(&p[PlayersNumbers.Current],PlayersNumbers.Current,M,Atklist);
            VC->refresh();
            whichMenuNum = VC->getSubMenuMV();
        }   

        if(whichMenuNum == -3){
            VC->prepInputMenu(M);
        }

        while(whichMenuNum == -3){
            VC->update(&p[PlayersNumbers.Current],PlayersNumbers.Current,M,Atklist);
            VC->refresh();
            whichMenuNum = VC->getInputMenuMV(M);
        }
                    
    }while(whichMenuNum < 0);
};

bool Game::attackProtocal(){
    if(VC->getPlayerCount() == PlayersNumbers.Current){
        return false;
    }
    if(p[PlayersNumbers.Current].troops[0]->getAmount() < VC->getArcherCount()){
        return false;
    }
    if(p[PlayersNumbers.Current].troops[1]->getAmount() < VC->getKnightCount()){
        return false;
    }
    if(p[PlayersNumbers.Current].troops[2]->getAmount() < VC->getDefenderCount()){
        return false;
    }
    if(VC->getDefenderCount() == 0 && VC->getKnightCount() == 0 && VC->getArcherCount() == 0){
        return false;
    }
    
    p[PlayersNumbers.Current].troops[0]->RemoveTroops(VC->getArcherCount());
    p[PlayersNumbers.Current].troops[1]->RemoveTroops(VC->getKnightCount());
    p[PlayersNumbers.Current].troops[2]->RemoveTroops(VC->getDefenderCount());

    Atklist.createNewAttack(&p[PlayersNumbers.Current],M,VC->getPlayerCount(),PlayersNumbers.Current,VC->getArcherCount(),VC->getKnightCount(),VC->getDefenderCount(),VC->getAttackChoice());
    return true;
};

int Game::attackOnTitan(){
    srand(time(0));
    int attackLog = -1;
    int choiceDefend;
    int Damage = 0;
    shared_ptr<Holder> Attacker;
    do{
        attackLog = Atklist.AmIBeingAttacked(p[PlayersNumbers.Current].getID());
        if(attackLog != -1){
            choiceDefend = VC->StartAttack(Atklist.getAttack(attackLog));
            Attacker = Atklist.getAttack(attackLog);
            int choiceAttack = Attacker->attackChoice;

            //No what we know we are being attacked and the player has gotten his defends style lets fo back to our handy chart
            /*           Fully  Safely  Gainfully
             *          |Even  |Loss   |Win     |
             *          |Win   |Even   |Loss    |
             *          |Loss  |Win    |Even    |
             * so How are we goin to calulate the losses well mainly we need to check our players troops & pin them against the troops afters us
             * then calculate the Appropriate losses and on random scale + the styles
             * 
             * On defence
             * 
             * Fully means 
             * Disadvantage on ally surviving
             * Disadvantage on resorce saftly 
             * advantage on damage Dealt
             * 
             * Safly means
             * Disadvantage on resorce safty 
             * Disadvantage on damage dealt
             * Advantage on ally survival
             * 
             * Gainfully means
             * Disadvantage on ally survival
             * Disadvantage on damage dealt
             * advantage on resource safty 
             * 
             * On attack is the same but without the resource safty and instead just advatgae on resource gain in the Gainfully thing 
             */
            int AtkUnitHealth = Attacker->troops[0]->getHealth()*Attacker->troops[0]->getAmount() + Attacker->troops[1]->getHealth()*Attacker->troops[1]->getAmount() + Attacker->troops[2]->getHealth()*Attacker->troops[2]->getAmount();
            int AtkUnitDamage = Attacker->troops[0]->getDamage()*Attacker->troops[0]->getAmount() + Attacker->troops[1]->getDamage()*Attacker->troops[1]->getAmount() + Attacker->troops[2]->getDamage()*Attacker->troops[2]->getAmount();
            int AtkUnitAcc = (Attacker->troops[0]->getAcc()+Attacker->troops[1]->getAcc()+Attacker->troops[2]->getAcc())/3;
            int AtkUnitHealthRatio = (Attacker->troops[0]->getHealth()+Attacker->troops[1]->getHealth()+Attacker->troops[2]->getHealth())/3;

            int DefUnitHealth = p[PlayersNumbers.Current].troops[0]->getHealth()*p[PlayersNumbers.Current].troops[0]->getAmount() + p[PlayersNumbers.Current].troops[1]->getHealth()*p[PlayersNumbers.Current].troops[1]->getAmount() + p[PlayersNumbers.Current].troops[2]->getHealth()*p[PlayersNumbers.Current].troops[2]->getAmount();
            int DefUnitDamage = p[PlayersNumbers.Current].troops[0]->getDamage()*p[PlayersNumbers.Current].troops[0]->getAmount() + p[PlayersNumbers.Current].troops[1]->getDamage()*p[PlayersNumbers.Current].troops[1]->getAmount() + p[PlayersNumbers.Current].troops[2]->getDamage()*p[PlayersNumbers.Current].troops[2]->getAmount();
            int DefUnitAcc = (p[PlayersNumbers.Current].troops[0]->getAcc()+p[PlayersNumbers.Current].troops[1]->getAcc()+p[PlayersNumbers.Current].troops[2]->getAcc())/3;
            int DefUnitHealthRatio =(p[PlayersNumbers.Current].troops[0]->getHealth()+p[PlayersNumbers.Current].troops[1]->getHealth()+p[PlayersNumbers.Current].troops[2]->getHealth())/3;

            double AtkBonusSurvivalRate = 0; //the greater the number the Better for Attacker
            double AtkBonusLootRate = 0;
            double AtkBonusDamageRate = 0;
            double DefBonusSurvivalRate = 0;
            double DefBonusDamageRate = 0;


            switch(choiceDefend){
                case 0:
                    if(choiceAttack == 0){ // 
                        AtkBonusSurvivalRate = 0;
                        AtkBonusLootRate = 0;
                        AtkBonusDamageRate = 0;
                        DefBonusSurvivalRate = 5/10;
                        DefBonusDamageRate = 2/10;                                   
                    }else if(choiceAttack == 1){ // loss
                        AtkBonusSurvivalRate = 3/10;
                        AtkBonusLootRate = 2/10;
                        AtkBonusDamageRate = 4/10;
                        DefBonusSurvivalRate = 0;
                        DefBonusDamageRate = 0; 
                    }else if(choiceAttack == 2){ // win
                        AtkBonusSurvivalRate = 2/10;
                        AtkBonusLootRate = 5/10;
                        AtkBonusDamageRate = 0;
                        DefBonusSurvivalRate = 5/10;
                        DefBonusDamageRate = 2/10; 
                    }
                break;
                case 1:
                    if(choiceAttack == 0){
                        AtkBonusSurvivalRate = 0;
                        AtkBonusLootRate = 0;
                        AtkBonusDamageRate = 0;
                        DefBonusSurvivalRate = 4/10;
                        DefBonusDamageRate = 3/10; 
                    }else if(choiceAttack == 1){
                        AtkBonusSurvivalRate = 0;
                        AtkBonusLootRate = 0;
                        AtkBonusDamageRate = 0;
                        DefBonusSurvivalRate = 5/10;
                        DefBonusDamageRate = 2/10; 
                    }else if(choiceAttack == 2){
                        AtkBonusSurvivalRate = 3/10;
                        AtkBonusLootRate = 5/10;
                        AtkBonusDamageRate = 0;
                        DefBonusSurvivalRate = 3/10;
                        DefBonusDamageRate = 0; 
                    }
                break;
                case 2:
                    if(choiceAttack == 0){
                        AtkBonusSurvivalRate = 2/10;
                        AtkBonusLootRate = 1/10;
                        AtkBonusDamageRate = 3/10;
                        DefBonusSurvivalRate = 2/10;
                        DefBonusDamageRate = 0; 
                    }else if(choiceAttack == 1){
                        AtkBonusSurvivalRate = 2/10;
                        AtkBonusLootRate = 1/10;
                        AtkBonusDamageRate = 2/10;
                        DefBonusSurvivalRate = 2/10;
                        DefBonusDamageRate = 2/10; 
                    }else if(choiceAttack == 2){
                        AtkBonusSurvivalRate = 0;
                        AtkBonusLootRate = 1/10;
                        AtkBonusDamageRate = 0;
                        DefBonusSurvivalRate = 5/10;
                        DefBonusDamageRate = 2/10; 
                    }
                break;
            }

            double AtkTrueDamage = AtkUnitDamage*AtkUnitAcc + (rand()%(int)((AtkUnitDamage*AtkBonusDamageRate)+1));
            double DefTrueDamage = DefUnitDamage*DefUnitAcc + (rand()%(int)((DefUnitDamage*DefBonusDamageRate)+1));
            
            double AtkDifferenceOfDamage = AtkTrueDamage/(AtkTrueDamage+DefTrueDamage+1);// the one is there just incase /0 occurs (it shouldnt but just incase)
            double DefDifferenceOfDamage = 1 - AtkDifferenceOfDamage;

            double AtkCalculatedHealth = AtkUnitHealth - (AtkUnitHealth*DefDifferenceOfDamage)*(1-AtkBonusSurvivalRate);
            double DefCalculatedHealth = DefUnitHealth - (DefUnitHealth*AtkDifferenceOfDamage)*(1-DefBonusSurvivalRate);

            double AtkUnitsLeft = AtkCalculatedHealth/AtkUnitHealthRatio;
            double DefUnitsLeft = DefCalculatedHealth/DefUnitHealthRatio;

            AtkUnitsLeft = round(AtkUnitsLeft);
            DefUnitsLeft = round(DefUnitsLeft);

            int AtkUnitCarry =(Attacker->troops[0]->getCarryingCapacity() + Attacker->troops[1]->getCarryingCapacity() + Attacker->troops[2]->getCarryingCapacity())/3;
            double AtkLoot = (AtkUnitsLeft*AtkUnitCarry) + (AtkUnitsLeft*AtkUnitCarry)*(AtkBonusLootRate);

            AtkLoot = round(AtkLoot);
            
            int alive = p[PlayersNumbers.Current].takeDamage((int)AtkUnitsLeft);
            
            vector<string> s;
            s.push_back("You Have Taken ");
            s[0] += std::to_string((int)AtkUnitsLeft);
            s[0] += " Damage ";
            if(alive == -1){
                s[0] += "and you are dead!";
            }
            s.push_back("Additonally you have lost ");
            s[1] += std::to_string((int)AtkLoot);
            s[1] += " material";
        
            VC->sendMsg(s);
        
            //give back the units
            int playerPos1 = findPlayer(Attacker->whoDidIt);
            while(AtkUnitsLeft > 0){
                if(AtkUnitsLeft > 0){
                    p[playerPos1].troops[0]->add(1);
                    AtkUnitsLeft--;
                }
                if(AtkUnitsLeft > 0){
                    p[playerPos1].troops[1]->add(1);
                    AtkUnitsLeft--;
                }
                if(AtkUnitsLeft > 0){
                    p[playerPos1].troops[2]->add(1);
                    AtkUnitsLeft--;
                }
            }
        
            int playerPos2 = findPlayer(Attacker->toWhom);
            while(DefUnitsLeft > 0 ){
                if(DefUnitsLeft > 0 && Attacker->troops[0]->getAmount() !=0){
                    p[playerPos2].troops[0]->add(1);
                    Attacker->troops[0]->RemoveTroops(1);
                    DefUnitsLeft--;
                }
                if(DefUnitsLeft > 0 && Attacker->troops[1]->getAmount() !=0){
                    p[playerPos2].troops[1]->add(1);
                    Attacker->troops[1]->RemoveTroops(1);
                    DefUnitsLeft--;
                }
                if(DefUnitsLeft > 0 && Attacker->troops[2]->getAmount() !=0){
                    p[playerPos2].troops[2]->add(1);
                    Attacker->troops[2]->RemoveTroops(1);
                    DefUnitsLeft--;
                }
            }
        
            while(AtkLoot > 0){
                if(AtkLoot > 0 && (p[playerPos2].getWood() > 0)){
                    p[playerPos1].addOrRemoveWood(1,true);
                    p[playerPos2].addOrRemoveWood(1,false);
                    AtkLoot--;
                }
                if(AtkLoot > 0 && (p[playerPos2].getStone() > 0)){
                    p[playerPos1].addOrRemoveStone(1,true);
                    p[playerPos2].addOrRemoveStone(1,false);
                    AtkLoot--;
                }
                if(AtkLoot > 0 && (p[playerPos2].getIron() > 0)){
                    p[playerPos1].addOrRemoveIron(1,true);
                    p[playerPos2].addOrRemoveIron(1,false);
                    AtkLoot--;
                }
                if(AtkLoot > 0 && (p[playerPos2].getFood() > 0)){
                    p[playerPos1].addOrRemoveFood(1,true);
                    p[playerPos2].addOrRemoveFood(1,false);
                    AtkLoot--;
                }
                if(p[playerPos2].getFood() == 0 &&  p[playerPos2].getIron() == 0 && p[playerPos2].getStone() == 0 && p[playerPos2].getWood() == 0){
                    break;
                }
            }
            
            attackLog = Atklist.removeAttack(attackLog);
        }

    }while(attackLog != -1);
    return 0;
}

#endif