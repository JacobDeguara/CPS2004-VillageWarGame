
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
    //The number identifier for each player
    int ID;

    //The amount of health each Player has
    int health= 50;
    
    //The amount of resources the player has
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
    int getResSize();
    int getHP();

    int RoundEnd();
    int takeDamage(int hp);

    void addOrRemoveWood(int amount, bool add);
    void addOrRemoveIron(int amount, bool add);
    void addOrRemoveStone(int amount, bool add);
    void addOrRemoveFood(int amount, bool add);

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

    // Checks if player is dead
    bool IsPlayerDead(){
        if(health < 1){
            return true;
        }else{
            return false;
        }
    };
};
/**
 * @brief Set the ID
 * 
 * @param newID 
 */
void Player::setID(int newID){
    ID = newID;
}

/**
 * @brief returns the ID
 * 
 * @return int 
 */
int Player::getID(){
    return ID;
}

/**
 * @brief returns the Wood 
 * 
 * @return int 
 */
int Player::getWood(){
    return res.wood;
}

/**
 * @brief returns the Iron
 * 
 * @return int 
 */
int Player::getIron(){
    return res.iron;
}

/**
 * @brief returns the Stone
 * 
 * @return int 
 */
int Player::getStone(){
    return res.stone;
}

/**
 * @brief returns the Food
 * 
 * @return int 
 */
int Player::getFood(){
    return res.food;
}

/**
 * @brief returns the Resource size
 * 
 * @return int 
 */
int Player::getResSize(){
    return res.size;
}

/**
 * @brief Increase the amount of Resources and Troops based on the Buildings
 * 
 * @return int 
 */
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

    health += 2;
    if(health > 50){
        health = 50;
    }
    return 1;   
}

/**
 * @brief returns the Hp
 * 
 * @return int 
 */
int Player::getHP(){
    return health;
}

/**
 * @brief Upgrade Building
 * 
 * @param amount by how much
 * @param buildingNum which building
 * @param woodCost wood removed
 * @param stoneCost stone removed
 * @param ironCost iron removed
 * @param foodCost food removed
 * @return true if success
 * @return false if failure
 */
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
    //^^^^ if the cost is too much returns false

    //consume the resource
    res.wood -= woodCost;
    res.stone -= stoneCost;
    res.iron -= ironCost;
    res.food -= foodCost;

    //Upgrade
    buildings[buildingNum]->upgrade(amount);
    return false;
}

/**
 * @brief Add Building
 * 
 * @param amount by how much
 * @param buildingNum which building
 * @param woodCost wood removed
 * @param stoneCost stone removed
 * @param ironCost iron removed
 * @param foodCost food removed
 * @return true 
 * @return false 
 */
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
    //^^^^ if the cost is too much returns false

    //consume the resource
    res.wood -= woodCost;
    res.stone -= stoneCost;
    res.iron -= ironCost;
    res.food -= foodCost;

    //Add
    buildings[buildingNum]->add(amount);  
    return false;  
}

/**
 * @brief Upgrade the Building
 * 
 * @param amount by how much
 * @param buildingNum which troop
 * @param woodCost wood removed
 * @param stoneCost stone removed
 * @param ironCost iron removed
 * @param foodCost food removed
 * @return true 
 * @return false 
 */
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
    //^^^^ if the cost is too much returns false

    //consume the resource
    res.wood -= woodCost;
    res.stone -= stoneCost;
    res.iron -= ironCost;
    res.food -= foodCost;

    //Upgrade
    troops[troopNum]->upgrade(amount);
    return false;
}

/**
 * @brief Deals damage to the player
 * 
 * @param hp amount of hp lost
 * @return '-1' if dead
 */
int Player::takeDamage(int hp){
    health -= hp;
    if(health < 1){
        return -1; //if dead
    }
    return 0;
}

/**
 * @brief Removes or adds wood
 * 
 * @param amount 
 * @param add Add if true | remove if false
 */
void Player::addOrRemoveWood(int amount, bool add){
    if(add){
        res.wood +=amount;
    }else{
        res.wood -=amount;
    }
}

/**
 * @brief Removes or adds Iron
 * 
 * @param amount 
 * @param add Add if true | remove if false
 */
void Player::addOrRemoveIron(int amount, bool add){
    if(add){
        res.iron +=amount;
    }else{
        res.iron -=amount;
    }
}

/**
 * @brief Removes or adds stone
 * 
 * @param amount 
 * @param add Add if true | remove if false
 */
void Player::addOrRemoveStone(int amount, bool add){
    if(add){
        res.stone +=amount;
    }else{
        res.stone -=amount;
    }
}

/**
 * @brief Removes or adds Food
 * 
 * @param amount 
 * @param add 
 */
void Player::addOrRemoveFood(int amount, bool add){
    if(add){
        res.food +=amount;
    }else{
        res.food -=amount;
    }
}

#endif