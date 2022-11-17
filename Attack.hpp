#ifndef __ATTACK_H__
#define __ATTACK_H__

#include <vector>
#include <memory>
#include <cmath>

#include "Map.hpp"
#include "Troops.hpp"
#include "Troops_Types.hpp"

using std::vector;
using std::shared_ptr;
using std::make_shared;

struct Holder{
    vector<shared_ptr<Troops>> troops{
        make_shared<Archer>(4,6,6,5),
        make_shared<Knight>(5,5,5,5),
        make_shared<Defender>(7,3,3,7),
    };
    int Maxtime; // Total time (doesnt change)
    int time; // Current tiime (does change)
    int whoDidIt; // which player did it
    int toWhom; // which player is getting attacked
    int attackChoice; // their choice that determines different out come of bonuses
};
/**
 * @brief The Attack Class Holds the list of attacks and manages it
 * 
 */
class Attack
{
private:
    vector<shared_ptr<Holder>> listOfAttacks;
    int MaxSize = -1;
    
public:
    Attack() = default;
    ~Attack() = default;

    void createNewAttack(Player * p,shared_ptr<Map> m,int toWhom,int whoDidIt,int archerC,int knightC,int DefenderC,int attackChoice);
    int removeAttack(int meID);
    int AmIBeingAttacked(int meID);
    void PlayerIsDied(int meID);
    
    /**
     * @brief Loops through each attack in the list and reduces each time by 1
     */
    void NextRound(){
        for (size_t i = 0; i < listOfAttacks.size(); i++)
        {   
            if(listOfAttacks[i]->time !=0){ // if time == 0 dont do this
                listOfAttacks[i]->time -= 1;
            }
        }
    };

    /*returns the MaxSize of the list (for segmentation issues)*/
    int getSize(){
        return MaxSize;
    };

    /*Returns the Attack list at n*/
    shared_ptr<Holder> getAttack(int n){
        return listOfAttacks[n];
    };

};

/**
 * @brief Creates a new attack in the list
 * 
 * @param p player
 * @param m Map
 * @param toWhom ID
 * @param whoDidIt ID
 * @param archerC Count
 * @param knightC Count
 * @param DefenderC Count
 * @param attackChoice Number
 */
void Attack::createNewAttack(Player * p,shared_ptr<Map> m,int toWhom,int whoDidIt,int archerC,int knightC,int DefenderC,int attackChoice)
{   
    MaxSize++; //increase the size by one 
    listOfAttacks.push_back(make_shared<Holder>()); //and push a new attack

    //             VVVV  is all setting each aspect of the Holder struct

    // basic sets
    listOfAttacks.at(MaxSize)->attackChoice = attackChoice; 
    listOfAttacks.at(MaxSize)->toWhom = toWhom;
    listOfAttacks.at(MaxSize)->whoDidIt = whoDidIt;

    // add troops based on amount
    listOfAttacks.at(MaxSize)->troops[0]->add(archerC);
    listOfAttacks.at(MaxSize)->troops[1]->add(knightC);
    listOfAttacks.at(MaxSize)->troops[2]->add(DefenderC);

    // set level of troops based on player
    listOfAttacks.at(MaxSize)->troops[0]->upgrade(p->troops[0]->getLevel()-1);
    listOfAttacks.at(MaxSize)->troops[1]->upgrade(p->troops[1]->getLevel()-1);
    listOfAttacks.at(MaxSize)->troops[2]->upgrade(p->troops[2]->getLevel()-1);

    // set the time it takes to get from player who to player whom
    double x1 = (double)m->getPosX(whoDidIt), y1 = (double)m->getPosY(whoDidIt);
    double x2 = (double)m->getPosX(toWhom)  , y2 = (double)m->getPosY(toWhom);

    double distance = sqrt( pow(x2-x1,2) + pow(y2-y1,2) ); // formula for distance between 2 points
    distance = round(distance);

    //using distance set a time
    int time;
    if(distance > 75){
        time = 4;
    }else if(distance > 50){
        time = 3;
    }else if(distance > 25){
        time = 2;
    }else{
        time = 1;
    }

    //and set the time
    listOfAttacks.at(MaxSize)->time = time;
    listOfAttacks.at(MaxSize)->Maxtime = time;
}

/**
 * @brief Removes each attack with the pos (Has to be done if player is died or surrenders)
 * 
 * @param pos position of attack in list
 * @return int 
 */
int Attack::removeAttack(int pos){
    //check if player even exists
    if(pos > MaxSize || pos == -1){
        return -1; // return failure
    }
    MaxSize--; //reduces size by 1
    listOfAttacks.erase(listOfAttacks.begin()+pos); //and removes the attack from list
    return 0;
}

/**
 * @brief Removes each attack which was to and from the player 
 * 
 * @param meID - ID of Player
 */
void Attack::PlayerIsDied(int meID){
    int size = listOfAttacks.size();
    for (size_t i = 0; i < size; i++)
    {
        if(listOfAttacks[i]->toWhom == meID){ // check in the list for all attacks to ID player
            removeAttack(i);
        }

        if(listOfAttacks[i]->whoDidIt == meID && i < size){ // check in list for all attacks from ID player + (incase it was the last 2 this one doesnt go out of bounce)
            removeAttack(i);
        }

    }
}

/**
 * @brief Takes the ID of the player and returns the pos of the attack in the list
 * 
 * @param meID - ID 
 * @return '-1'(if no attack) , 'i'(i=0-n)(if true)
 */
int Attack::AmIBeingAttacked(int meID)
{
    for(int i=0; i < listOfAttacks.size(); i++){
        if((listOfAttacks[i]->toWhom == meID) && (listOfAttacks[i]->time == 0)){
            return i;
        }
    }
    return -1;
}

#endif // __ATTACK_H__