
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Buildings.hpp"
#include "Troops.hpp"
#include "Buildings_Types.hpp"
#include "Troops_Types.hpp"

#include <vector>

using std::vector;

class Player
{
private:
    int x,y; //coords
    int health; // hp of Village
    struct{
        int wood =999;
        int stone =999;
        int iron =999;
        int food =999;
        int size = 4;
    }res;

public:
    Player();
    ~Player();
    int getWood();
    int getIron();
    int getStone();
    int getFood();
    int getResSize();
    int RoundEnd();
    int getGen(int BuildingNum);
    
    vector<Buildings *> buildings;
    vector<Troops *> troops;

    bool upgradeBuilding(int amount,int buildingNum,int woodCost,int stoneCost, int ironCost,int foodCost);
    bool increaseBuilding(int amount,int buildingNum,int woodCost,int stoneCost, int ironCost,int foodCost);
    bool upgradeTroops(int amount,int troopNum,int woodCost,int stoneCost, int ironCost,int foodCost);
};

Player::Player()
{
    this->health = 100;
    buildings = {
        new WoodCutter(),
        new StoneMiner(),
        new IronMiner(),
        new BattleTrainer(),
        new ArcheryRange(),
        new KnightingPalace(),
        new DefenderBarracks()};

    troops = {
        new   Archer(4,6,6,5),
        new   Knight(5,5,5,5),
        new Defender(7,3,3,7)};
}

Player::~Player(){
    for(auto i : buildings){
        delete(i);
    }
    buildings.clear();
    for(auto i : troops){
        delete(i);
    }
    troops.clear();
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
    res.wood += getGen(0);
    res.stone +=getGen(1);
    res.iron += getGen(2);
    res.food += getGen(3);

    //troop buildings
    troops[0]->add(getGen(4));
    troops[1]->add(getGen(5));
    troops[2]->add(getGen(6));
    return 1;   
}

int Player::getGen(int BuildingNum)
{
    int amount = buildings[BuildingNum]->getamount();
    int level = buildings[BuildingNum]->getLevel();
    
    if(BuildingNum <= 2){ 
        return amount*2+amount*level;
    }else if(BuildingNum <= 3){
        return amount + amount*level;
    }else{
        if(amount == 0){
            return amount;
        }else{
            return amount+level;
        }
    }
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

#endif