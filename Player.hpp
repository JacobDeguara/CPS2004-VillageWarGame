
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "Building.hpp"
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
    vector<Building> Buildings = {WoodCutter(),StoneMiner(),IronMiner(),BattleTrainer(),ArcheryRange(),KnightingPalace(),DefenderBarracks()};
    vector<Troops> troops = {Archer(),Knight(),Defender()};

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
    res.wood += Buildings[0].RoundEnd();
    res.stone += Buildings[1].RoundEnd();
    res.iron += Buildings[2].RoundEnd();
    res.food += Buildings[3].RoundEnd();

    //troop buildings
    troops[0].add(Buildings[4].RoundEnd());
    troops[1].add(Buildings[5].RoundEnd());
    troops[2].add(Buildings[6].RoundEnd());

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