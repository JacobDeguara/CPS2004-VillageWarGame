
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "Building.hpp"
#include "Troops.hpp"
#include "Buildings_Types.hpp"
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

    vector<Building> ResBuildings = {WoodCutter(0,1),StoneMiner(0,1),IronMiner(0,1)};
    vector<Building> TroBuildings = {ArcheryRange(0,1),KnightingPalace(0,1),DefenderBarracks(0,1)};
    BattleTrainer * battletrainer;

public:
    Player(/* args */);
    ~Player();
    int getWood();
    int getIron();
    int getStone();
    int getFood();
    int getResSize();
};

Player::Player(/* args */)
{
    battletrainer = new BattleTrainer(0,1);
}
Player::~Player(/* args */)
{
    delete battletrainer;
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





#endif