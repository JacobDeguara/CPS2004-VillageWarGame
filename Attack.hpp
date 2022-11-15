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
    int Maxtime; // for VC
    int time;
    int whoDidIt;
    int toWhom;
    int attackChoice;
};

class Attack
{
private:
    vector<shared_ptr<Holder>> listOfAttacks;
    int MaxSize = -1;
    
public:
    Attack();
    ~Attack() = default;

    void createNewAttack(Player * p,shared_ptr<Map> m,int toWhom,int whoDidIt,int archerC,int knightC,int DefenderC,int attackChoice);
    int removeAttack(int meID);
    int AmIBeingAttacked(int meID);
    void PlayerIsDied(int meID);
    
    shared_ptr<Holder> getAttack(int Num){
        return listOfAttacks[Num];
    };

    //reduces the time by 1
    void NextTurn(){
        for (size_t i = 0; i < listOfAttacks.size(); i++)
        {   
            if(listOfAttacks[i]->time !=0){
                listOfAttacks[i]->time -= 1;
            }
        }
    };

    int Size(){
        return MaxSize;
    };

    int getNum(int num){
        if(num < MaxSize){
            return listOfAttacks[num]->time;
        }
        return 0;
    }

};

Attack::Attack()
{
}

//Creates a new Attack in the list and pushes it
void Attack::createNewAttack(Player * p,shared_ptr<Map> m,int toWhom,int whoDidIt,int archerC,int knightC,int DefenderC,int attackChoice)
{
    MaxSize++;
    listOfAttacks.push_back(make_shared<Holder>());
    //basic sets
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

    //
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
    
    listOfAttacks.at(MaxSize)->time = time;
    listOfAttacks.at(MaxSize)->Maxtime = time;
}

int Attack::removeAttack(int meID){
    if(meID > MaxSize || meID == -1){
        return -1;
    }
    MaxSize--;
    listOfAttacks.erase(listOfAttacks.begin()+meID);
    return 0;
}

void Attack::PlayerIsDied(int meID){
    for (size_t i = 0; i < listOfAttacks.size(); i++)
    {
        if(listOfAttacks[i]->toWhom == meID){
            removeAttack(i);
            MaxSize--;
        }
    }
}

// returns the list number if true else returns -1
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