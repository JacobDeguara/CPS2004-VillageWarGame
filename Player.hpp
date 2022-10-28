
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "Building.hpp"
#include "Troops.hpp"
#include "Buildings_Types.hpp"
#include "Troops_Types.hpp"
#include <vector>
#include <memory>

using std::vector;


class Player
{
private:
    int x,y; //coords
    int health; // hp of Village
    struct{
        int wood =10;
        int stone =10;
        int iron =0;
        int food =3;
        int size = 4;
    }res;

public:
    Player();
    int getWood();
    int getIron();
    int getStone();
    int getFood();
    int getResSize();
    int RoundEnd();
    int getGen(int BuildingNum);
    vector<Building> Buildings = {WoodCutter(),StoneMiner(),IronMiner(),BattleTrainer(),ArcheryRange(),KnightingPalace(),DefenderBarracks()};
    vector<Troops> troops = {Archer(5,15,5,5),Knight(10,5,8,10),Defender(10,4,9,5)};

    bool upgradeBuilding(int amount,int buildingNum,int woodCost,int stoneCost, int ironCost,int foodCost);
    bool increaseBuilding(int amount,int buildingNum,int woodCost,int stoneCost, int ironCost,int foodCost);
    bool upgradeTroops(int amount,int troopNum,int woodCost,int stoneCost, int ironCost,int foodCost);
};

Player::Player()
{
    this->health = 100;
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
    troops[0].add(getGen(4));
    troops[1].add(getGen(5));
    troops[2].add(getGen(6));
    return 1;   
}

int Player::getGen(int BuildingNum)
{
    int amount =Buildings[BuildingNum].getamount();
    int level = Buildings[BuildingNum].getLevel();
    
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

    Buildings[buildingNum].upgrade(amount);
    return true;
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

    Buildings[buildingNum].add(amount);  
    return true;  
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

    troops[troopNum].upgrade(amount);
    return true;
}

#endif