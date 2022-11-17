
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

/**
 * @brief The Game Class Holds all the inner Workings for the Game
 * 
 */
class Game
{
private:

    //Visual Control
    shared_ptr<ViewController> VC = make_shared<ViewController>();
    //Map
    shared_ptr<Map> M;
    //Attack-List
    Attack Atklist;

    //Players Res
    vector<Player> p;
    
    /**
     * @brief Holds current player and the Max number of players
     * 
     */
    struct {
        int Max;
        int Current;
    }PlayersNumbers;

    void StartGame();

    void endRound();
    void CheckPlayersAlive();
    void menuLoop(int &whichMenu);
    
    int actionTaken(); // will take the selected action and run it
    bool attackProtocal();

    void attackOnTitan();
    int findPlayer(int ID);


public:
    Game(); 
    ~Game() = default;
    
    int gameLoop();
};

/**
 * @brief Construct a new Game:: Game object
 * 
 */
Game::Game(){
    Atklist = Attack();
}

/**
 * @brief The Game Loop of the Game VILLAGE WAR GAME
 * 
 * @return 0 
 */
int Game::gameLoop(){
    int whichMenuNum = -1;
    bool dontExit = true, EndRound = true;
    
    StartGame();
    do{ 
        //This Loop is the 'END-ROUND'
        EndRound = true;
        
        //Attack End of Round
        Atklist.NextRound();

        do{
            //This Loop is the 'TURN'
            dontExit = true;
            VC->resetMenu();
        
            //Refresh the menu
            VC->update(&p[PlayersNumbers.Current],PlayersNumbers.Current,M,Atklist);
            VC->refresh();

            //Attack stuff
            attackOnTitan();

            //Check if the player died or not
            CheckPlayersAlive();

            //Check if only one player is left
            if(PlayersNumbers.Max <= 1){
                return 0;
            }

        do{ 
            //This Loop is the current players 'CURRENT_TURN'

            //Run Menu
            whichMenuNum = VC->resetMenuPartial();
            menuLoop(whichMenuNum);

            //take action
            whichMenuNum = actionTaken();
            switch(whichMenuNum){
                case 1: //End the round
                    //Give player resouces
                    endRound();

                    //Goes to next player
                    PlayersNumbers.Current++;
                    //if next player is Max (which doesn't exist)
                    if(PlayersNumbers.Current == PlayersNumbers.Max){
                        PlayersNumbers.Current = 0; // reset to first player
                        EndRound = false;
                    }

                    //exit
                    dontExit = false;
                break;
                case -1:
                    PlayersNumbers.Max--; // Reduce Max number of players

                    Atklist.PlayerIsDied(p.at(PlayersNumbers.Current).getID()); // remove player from AtkList
                    M->PlayerIsDead(p.at(PlayersNumbers.Current).getID()); //Add player to the dead list in Map
                        
                    p.erase(p.begin() + PlayersNumbers.Current); // remove player from Vector
                    if(PlayersNumbers.Current == PlayersNumbers.Max){ // if current player equal to max player (which doesn't exist)
                        PlayersNumbers.Current = 0; // reset to first player
                        EndRound = false;
                    }

                    //exit
                    dontExit = false;
                break;
                // if case 0: DO NOTHING
            }//end of switch

        }while(dontExit);//end of 'CURRENT-TURN'

        }while(EndRound);//end of the 'TURN'

    }while(PlayersNumbers.Max > 1);//end of the 'ROUND'
    return 0;
}

/**
 * @brief Runs A Menu to get the Number of Players & Ais involved in the game and sets them up.
 */
void Game::StartGame(){
    int numOfPlayer,numOfAi;

    VC->updateStart(numOfPlayer,numOfAi); // returns the number of player and Ai

    PlayersNumbers.Current = 0; // setting current to 0
    PlayersNumbers.Max = numOfPlayer; // setting max to the number retruned
    for(int i=0;i < PlayersNumbers.Max;i++){ // creating each player in vector<PLAYER> p
        p.push_back(Player());
        p[i].setID(i);
    }

    M = make_shared<Map>(PlayersNumbers.Max); // giving each player a coordinate
}

/**
 * @brief Takes in the Choices from player and runs commands appropriately
 * 
 * @returns END ROUND(1) , DO NOTHING(0) , SURRENDER(-1)
 */
int Game::actionTaken(){
    //Takes the choices decided from VC
    int choice1 = VC->getChoice1();
    int choice2 = VC->getChoice2();
    //Inacts the respective response
    switch(choice1){
        case 0://add buildings
            return p[PlayersNumbers.Current].increaseBuilding(VC->getCount(),choice2,VC->getWoodCost(),VC->getStoneCost(),VC->getIronCost(),VC->getFoodCost());
        break;
        case 1://upgrade buildings
            return p[PlayersNumbers.Current].upgradeBuilding(VC->getCount(),choice2,VC->getWoodCost(),VC->getStoneCost(),VC->getIronCost(),VC->getFoodCost());
        break;
        case 2://upgrade troops
            return p[PlayersNumbers.Current].upgradeTroops(VC->getCount(),choice2,VC->getWoodCost(),VC->getStoneCost(),VC->getIronCost(),VC->getFoodCost()); 
        break;
            //these will always return false ^^^ (= 0)

        case 3:// unique Case (Attack,End_Turn,Surrender)
            if(choice2 == 0){ //attack protocal (not done yet)
                if(attackProtocal()){//tries attackProtocal
                    return 1; // returns 1 (End turn) if success
                }
                return 0; // Do nothing if other wise
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

/**
 * @brief Creates an Attack in Attack List if sufficient input acquired
 * 
 * @return true (Success)
 * @return false (Failure)
 */
bool Game::attackProtocal(){

    if(VC->getPlayerCount() == PlayersNumbers.Current){ // if same Player
        return false;
    }
    if(p[PlayersNumbers.Current].troops[0]->getAmount() < VC->getArcherCount()){ // if more Archers then owned
        return false;
    }
    if(p[PlayersNumbers.Current].troops[1]->getAmount() < VC->getKnightCount()){ // if more Knights then owned
        return false;
    }
    if(p[PlayersNumbers.Current].troops[2]->getAmount() < VC->getDefenderCount()){ // if more Defenders then owned
        return false;
    }
    if(VC->getDefenderCount() == 0 && VC->getKnightCount() == 0 && VC->getArcherCount() == 0){ // if all Troops are 0
        return false;
    }
    
    //Remove troops from Player
    p[PlayersNumbers.Current].troops[0]->RemoveTroops(VC->getArcherCount());
    p[PlayersNumbers.Current].troops[1]->RemoveTroops(VC->getKnightCount());
    p[PlayersNumbers.Current].troops[2]->RemoveTroops(VC->getDefenderCount());

    //Set Attack in Atk-List
    Atklist.createNewAttack(&p[PlayersNumbers.Current],M,VC->getPlayerCount(),PlayersNumbers.Current,VC->getArcherCount(),VC->getKnightCount(),VC->getDefenderCount(),VC->getAttackChoice());
    return true; //return successful
};

/**
 * @brief Runs the Menu Loop 
 * 
 * @param whichMenuNum 
 */
void Game::menuLoop(int &whichMenuNum){
    do{
        //Update brefore start
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

/**
 * @brief Runs the End of the Round for Current Player
 * 
 */
void Game::endRound(){
    p[PlayersNumbers.Current].RoundEnd();
}

/**
 * @brief Check if the Player is Alive, If not removes them and fixes Current-Player
 * 
 */
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

/**
 * @brief Finds Player with ID and returns their current int location in vector<PLAYER> p
 * 
 * @param ID 
 * @return * int 
 */
int Game::findPlayer(int ID){
    for(int i=0; i < p.size(); i++){
        if(p[i].getID() == ID){
            return i;
        }
    }
    return -1;
}

/**
 * @brief Finds every Attack currently Attacking current player and Does the calculations for each
 * 
 */
void Game::attackOnTitan(){
    srand(time(0));
    int attackLog = -1;
    int choiceDefend;
    int Damage = 0;
    shared_ptr<Holder> Attacker;
    do{
        attackLog = Atklist.AmIBeingAttacked(p[PlayersNumbers.Current].getID()); //Check if the player is being attacked
        if(attackLog != -1){ // if 0-n run attack
            choiceDefend = VC->StartAttack(Atklist.getAttack(attackLog)); // take defenders choice 
            Attacker = Atklist.getAttack(attackLog); // get the attack
            int choiceAttack = Attacker->attackChoice; // get the attackers choice

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

            //calulate each Total units Health/Damage & their Avrage Acc & the Health ratio
            int AtkUnitHealth = Attacker->troops[0]->getHealth()*Attacker->troops[0]->getAmount() + Attacker->troops[1]->getHealth()*Attacker->troops[1]->getAmount() + Attacker->troops[2]->getHealth()*Attacker->troops[2]->getAmount();
            int AtkUnitDamage = Attacker->troops[0]->getDamage()*Attacker->troops[0]->getAmount() + Attacker->troops[1]->getDamage()*Attacker->troops[1]->getAmount() + Attacker->troops[2]->getDamage()*Attacker->troops[2]->getAmount();
            int AtkUnitAcc = (Attacker->troops[0]->getAcc()+Attacker->troops[1]->getAcc()+Attacker->troops[2]->getAcc())/3;
            int AtkUnitHealthRatio = (Attacker->troops[0]->getHealth()+Attacker->troops[1]->getHealth()+Attacker->troops[2]->getHealth())/3;

            int DefUnitHealth = p[PlayersNumbers.Current].troops[0]->getHealth()*p[PlayersNumbers.Current].troops[0]->getAmount() + p[PlayersNumbers.Current].troops[1]->getHealth()*p[PlayersNumbers.Current].troops[1]->getAmount() + p[PlayersNumbers.Current].troops[2]->getHealth()*p[PlayersNumbers.Current].troops[2]->getAmount();
            int DefUnitDamage = p[PlayersNumbers.Current].troops[0]->getDamage()*p[PlayersNumbers.Current].troops[0]->getAmount() + p[PlayersNumbers.Current].troops[1]->getDamage()*p[PlayersNumbers.Current].troops[1]->getAmount() + p[PlayersNumbers.Current].troops[2]->getDamage()*p[PlayersNumbers.Current].troops[2]->getAmount();
            int DefUnitAcc = (p[PlayersNumbers.Current].troops[0]->getAcc()+p[PlayersNumbers.Current].troops[1]->getAcc()+p[PlayersNumbers.Current].troops[2]->getAcc())/3;
            int DefUnitHealthRatio =(p[PlayersNumbers.Current].troops[0]->getHealth()+p[PlayersNumbers.Current].troops[1]->getHealth()+p[PlayersNumbers.Current].troops[2]->getHealth())/3;

            //creating each bonus variable
            double AtkBonusSurvivalRate = 0;
            double AtkBonusLootRate = 0;
            double AtkBonusDamageRate = 0;
            double DefBonusSurvivalRate = 0;
            double DefBonusDamageRate = 0;

            //calulate each bonus seperatly
            switch(choiceDefend){
                case 0:
                    if(choiceAttack == 0){
                        AtkBonusSurvivalRate = 0;
                        AtkBonusLootRate = 0;
                        AtkBonusDamageRate = 0;
                        DefBonusSurvivalRate = 5/10;
                        DefBonusDamageRate = 2/10;                                   
                    }else if(choiceAttack == 1){
                        AtkBonusSurvivalRate = 3/10;
                        AtkBonusLootRate = 2/10;
                        AtkBonusDamageRate = 4/10;
                        DefBonusSurvivalRate = 0;
                        DefBonusDamageRate = 0; 
                    }else if(choiceAttack == 2){
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

            // calulate the total damage dealt + bonus
            double AtkTrueDamage = AtkUnitDamage*AtkUnitAcc + (rand()%(int)((AtkUnitDamage*AtkBonusDamageRate)+1));
            double DefTrueDamage = DefUnitDamage*DefUnitAcc + (rand()%(int)((DefUnitDamage*DefBonusDamageRate)+1));
            
            //Calculate the difference In damage
            double AtkDifferenceOfDamage = AtkTrueDamage/(AtkTrueDamage+DefTrueDamage+1);// the one is there just incase /0 occurs (it shouldnt but just incase)
            double DefDifferenceOfDamage = 1 - AtkDifferenceOfDamage;

            //calculate the damage done based on the difference and the bonus
            double AtkCalculatedHealth = AtkUnitHealth - (AtkUnitHealth*DefDifferenceOfDamage)*(1-AtkBonusSurvivalRate);
            double DefCalculatedHealth = DefUnitHealth - (DefUnitHealth*AtkDifferenceOfDamage)*(1-DefBonusSurvivalRate);

            //reverse calulate an estimate of the units left based on the health
            double AtkUnitsLeft = AtkCalculatedHealth/AtkUnitHealthRatio;
            double DefUnitsLeft = DefCalculatedHealth/DefUnitHealthRatio;

            //round it so its has no decimal point  
            AtkUnitsLeft = round(AtkUnitsLeft);
            DefUnitsLeft = round(DefUnitsLeft);

            //calulate the amount of stolen material
            int AtkUnitCarry =(Attacker->troops[0]->getCarryingCapacity() + Attacker->troops[1]->getCarryingCapacity() + Attacker->troops[2]->getCarryingCapacity())/3;
            double AtkLoot = (AtkUnitsLeft*AtkUnitCarry) + (AtkUnitsLeft*AtkUnitCarry)*(AtkBonusLootRate);

            //round it so it has no decimal point
            AtkLoot = round(AtkLoot);
            
            //check if the player died from takeing the damage
            int alive = p[PlayersNumbers.Current].takeDamage((int)AtkUnitsLeft);
            
            //show the player
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

            DefUnitsLeft = (DefUnitHealth/DefUnitHealthRatio) - DefUnitsLeft;
            int playerPos1 = findPlayer(Attacker->whoDidIt);
            
            //give back the units to player1 (Attacker)
            while(AtkUnitsLeft > 0){
                if(AtkUnitsLeft > 0 && Attacker->troops[0]->getAmount() !=0){
                    p[playerPos1].troops[0]->add(1);
                    Attacker->troops[0]->RemoveTroops(1);
                    AtkUnitsLeft--;
                }
                if(AtkUnitsLeft > 0 && Attacker->troops[1]->getAmount() !=0){
                    p[playerPos1].troops[1]->add(1);
                    Attacker->troops[1]->RemoveTroops(1);
                    AtkUnitsLeft--;
                }
                if(AtkUnitsLeft > 0 && Attacker->troops[2]->getAmount() !=0){
                    p[playerPos1].troops[2]->add(1);
                    Attacker->troops[2]->RemoveTroops(1);
                    AtkUnitsLeft--;
                }
                if(Attacker->troops[0]->getAmount() == 0 && Attacker->troops[1]->getAmount() == 0 && Attacker->troops[2]->getAmount() == 0){

                }
            }
            
            //removes all the units that died from Player2 (Defender)
            
            int playerPos2 = findPlayer(Attacker->toWhom);
            while(DefUnitsLeft > 0 ){
                if(DefUnitsLeft > 0 && p[playerPos2].troops[0]->getAmount() != 0){
                    p[playerPos2].troops[0]->RemoveTroops(1);
                    DefUnitsLeft--;
                }
                if(DefUnitsLeft > 0 && p[playerPos2].troops[1]->getAmount() != 0){
                    p[playerPos2].troops[1]->RemoveTroops(1);
                    DefUnitsLeft--;
                }
                if(DefUnitsLeft > 0 && p[playerPos2].troops[2]->getAmount() != 0){
                    p[playerPos2].troops[2]->RemoveTroops(1);
                    DefUnitsLeft--;
                }
                if(p[playerPos2].troops[2]->getAmount() == 0 && p[playerPos2].troops[1]->getAmount() == 0 && p[playerPos2].troops[0]->getAmount() == 0){
                    break;
                }
            }
            
            
            //give the player the loot
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
            
            
            //remove from the list
            attackLog = Atklist.removeAttack(attackLog);

            //if the player died remove all attacks related to him
            if(alive == -1){
                Atklist.PlayerIsDied(p[PlayersNumbers.Current].getID());
            }
        }

    }while(attackLog != -1);
    return;
}

#endif