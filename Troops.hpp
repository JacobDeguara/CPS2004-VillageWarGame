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

//Stating troop stats
Troops::Troops(int health ,int damage ,int acc,int CC)
{   
    this->health = health;
    this->damage = damage;
    this->acc =acc;
    this->carryingCapacity = CC;
}

//upgrades Troops
void Troops::upgrade(int num){
    level += num;
    health += num;
    damage += num;
    carryingCapacity += num;
}

//Adds Troops
void Troops::add(int num){
    amount += num;
}

//gets health
int Troops::getHealth()
{
    return health;
}

//gets carringCapacity
int Troops::getCarryingCapacity()
{
    return carryingCapacity;
}

//gets amount
int Troops::getAmount()
{
    return amount;
}

//gets level
int Troops::getLevel()
{
    return level;
}

//gets Damage
int Troops::getDamage()
{
    return damage;
}

//returns (damage + health *amount)
int Troops::getPowerRating()
{
    return (damage + health) * amount;
}

/*This will remove by the amount
* if(returns 0) => (amount given > amount available)
*/
int Troops::RemoveTroops(int amount)
{   
    int temp;
    if(amount <= this->amount){
        temp = amount;
        this->amount -= amount;
        return temp;
    }
    return 0;
}

#endif
