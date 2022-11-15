
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Buildings.hpp"
#include "Troops.hpp"
#include "Buildings_Types.hpp"
#include "Troops_Types.hpp"

#include <vector>
#include <memory>

using std::vector;
using std::shared_ptr;
using std::make_shared;

class Player
{
private:
    int ID;

    int health= 100; // hp of Village
    struct{
        int wood =999;
        int stone =999;
        int iron =999;
        int food =999;
        int size = 4;
    }res;

public:
    Player()=default;
    ~Player()= default;
    void setID(int newID);
    int getID();
    int getWood();
    int getIron();
    int getStone();
    int getFood();
    void addOrRemoveWood(int amount, bool add);
    void addOrRemoveIron(int amount, bool add);
    void addOrRemoveStone(int amount, bool add);
    void addOrRemoveFood(int amount, bool add);
    int getResSize();
    int RoundEnd();
    int takeDamage(int hp);

    vector<shared_ptr<Buildings>> buildings = {
        make_shared<WoodCutter>(),
        make_shared<StoneMiner>(),
        make_shared<IronMiner>(),
        make_shared<BattleTrainer>(),
        make_shared<ArcheryRange>(),
        make_shared<KnightingPalace>(),
        make_shared<DefenderBarracks>(),
    };

    vector<shared_ptr<Troops>> troops = {
        make_shared<Archer>(4,6,6,5),
        make_shared<Knight>(5,5,5,5),
        make_shared<Defender>(7,3,3,7),
    };

    bool upgradeBuilding(int amount,int buildingNum,int woodCost,int stoneCost, int ironCost,int foodCost);
    bool increaseBuilding(int amount,int buildingNum,int woodCost,int stoneCost, int ironCost,int foodCost);
    bool upgradeTroops(int amount,int troopNum,int woodCost,int stoneCost, int ironCost,int foodCost);
};

void Player::setID(int newID){
    ID = newID;
}

int Player::getID(){
    return ID;
}

int Player::getWood(){
    return res.wood;
}

int Player::getIron(){
    return res.iron;
}

int Player::getStone(){
    return res.stone;
}

int Player::getFood(){
    return res.food;
}

int Player::getResSize(){
    return res.size;
}

int Player::RoundEnd()
{
    //res buildings
    res.wood += buildings[0]->getGen();
    res.stone +=buildings[1]->getGen();
    res.iron += buildings[2]->getGen();
    res.food += buildings[3]->getGen();

    //troop buildings
    troops[0]->add(buildings[4]->getGen());
    troops[1]->add(buildings[5]->getGen());
    troops[2]->add(buildings[6]->getGen());
    return 1;   
}

bool Player::upgradeBuilding(int amount,int buildingNum,int woodCost,int stoneCost, int ironCost,int foodCost)
{
    if(woodCost > res.wood){
        return false;
    }
    if(stoneCost > res.stone){
        return false;
    }
    if(ironCost > res.iron){
        return false;
    }
    if(foodCost > res.food){
        return false;
    }
    res.wood -= woodCost;
    res.stone -= stoneCost;
    res.iron -= ironCost;
    res.food -= foodCost;

    buildings[buildingNum]->upgrade(amount);
    return false;
}

bool Player::increaseBuilding(int amount,int buildingNum,int woodCost,int stoneCost, int ironCost,int foodCost)
{
    if(woodCost > res.wood){
        return false;
    }
    if(stoneCost > res.stone){
        return false;
    }
    if(ironCost > res.iron){
        return false;
    }
    if(foodCost > res.food){
        return false;
    }
    res.wood -= woodCost;
    res.stone -= stoneCost;
    res.iron -= ironCost;
    res.food -= foodCost;

    buildings[buildingNum]->add(amount);  
    return false;  
}

bool Player::upgradeTroops(int amount,int troopNum,int woodCost,int stoneCost, int ironCost,int foodCost)
{
    if(woodCost > res.wood){
        return false;
    }
    if(stoneCost > res.stone){
        return false;
    }
    if(ironCost > res.iron){
        return false;
    }
    if(foodCost > res.food){
        return false;
    }
    res.wood -= woodCost;
    res.stone -= stoneCost;
    res.iron -= ironCost;
    res.food -= foodCost;

    troops[troopNum]->upgrade(amount);
    return false;
}

int Player::takeDamage(int hp){
    health -= hp;
    if(health < 1){
        return -1;
    }
    return 0;
}

void Player::addOrRemoveWood(int amount, bool add){
    if(add){
        res.wood +=amount;
    }else{
        res.wood -=amount;
    }
}

void Player::addOrRemoveIron(int amount, bool add){
    if(add){
        res.iron +=amount;
    }else{
        res.iron -=amount;
    }
}

void Player::addOrRemoveStone(int amount, bool add){
    if(add){
        res.stone +=amount;
    }else{
        res.stone -=amount;
    }
}

void Player::addOrRemoveFood(int amount, bool add){
    if(add){
        res.food +=amount;
    }else{
        res.food -=amount;
    }
}

#endif