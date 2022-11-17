#ifndef _TROOP_H_
#define _TROOP_H_

class Troops
{
public:
    Troops(int health ,int damage ,int acc,int CC);
    
    int getHealth();
    int getCarryingCapacity();
    int getAmount();
    int getLevel();
    int getDamage();
    int getAcc();

    virtual void upgrade(int num);
    void add(int num);

    int getPowerRating();
    int RemoveTroops(int amount);
    
protected:
    int amount = 0;
    int level = 1;
    int health;
    int damage;
    int acc;
    int speed;
    int carryingCapacity;
};

/**
 * @brief Construct a new Troops:: Troops object
 * 
 * @param health health
 * @param damage damage
 * @param acc accracy
 * @param CC carrying capacity
 */
Troops::Troops(int health ,int damage ,int acc,int CC)
{   
    //set stats
    this->health = health;
    this->damage = damage;
    this->acc =acc;
    this->carryingCapacity = CC;
}

/**
 * @brief Upgrades the Troops level and stats
 * 
 * @param num 
 */
void Troops::upgrade(int num){
    level += num;
    health += num;
    damage += num;
    carryingCapacity += num;
}

/**
 * @brief add num amount of troops
 * 
 * @param num 
 */
void Troops::add(int num){
    amount += num;
}

/**
 * @brief returns the Health
 * 
 * @return int 
 */
int Troops::getHealth()
{
    return health;
}

/**
 * @brief returns the carrying capacity
 * 
 * @return int 
 */
int Troops::getCarryingCapacity()
{
    return carryingCapacity;
}

/**
 * @brief returns the amount of troops
 * 
 * @return int 
 */
int Troops::getAmount()
{
    return amount;
}

/**
 * @brief returns the level
 * 
 * @return int 
 */
int Troops::getLevel()
{
    return level;
}

/**
 * @brief returns the Damage 
 * 
 * @return int 
 */
int Troops::getDamage()
{
    return damage;
}

/**
 * @brief returns the Accracy
 * 
 * @return int 
 */
int Troops::getAcc()
{
    return acc;
}

/**
 * @brief returns (damage + health *amount)
 * 
 * @return int 
 */
int Troops::getPowerRating()
{
    return (damage + health) * amount;
}

/**
 * @brief Remove the Troops amount by amount
 * 
 * @param amount amount to remove
 * @return int -- the amount removed 
 */
int Troops::RemoveTroops(int amount)
{   
    int temp;
    if(amount <= this->amount){ //check if enough
        temp = amount;
        this->amount -= amount; //remove
        return temp; //return amount removed
    }
    return 0;
}

#endif
