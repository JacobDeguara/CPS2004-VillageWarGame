#ifndef _TROOP_H_
#define _TROOP_H_

class Troops
{
public:
    Troops(int health ,int damage ,int acc,int CC);
    void upgrade(int num);
    void add(int num);
    int getHealth();
    int getCarryingCapacity();
    int getAmount();
    int getLevel();
    int getDamage();
    int getPowerRating();
    void setAmount(int amount);
    
protected:
    unsigned int amount = 0;
    unsigned int level = 1;
    int health;
    int damage;
    int acc;
    int speed;
    int carryingCapacity;
};

Troops::Troops(int health ,int damage ,int acc,int CC)
{   
    this->health = health;
    this->damage = damage;
    this->acc =acc;
    this->carryingCapacity = CC;
}

void Troops::upgrade(int num){
    level += num;
    health += num;
    damage += num;
    carryingCapacity += num;
}

void Troops::add(int num){
    amount += num;
}

int Troops::getHealth()
{
    return health;
}

int Troops::getCarryingCapacity()
{
    return carryingCapacity;
}

int Troops::getAmount()
{
    return amount;
}

int Troops::getLevel()
{
    return level;
}

int Troops::getDamage()
{
    return damage;
}

int Troops::getPowerRating()
{
    return (damage + health) * amount;
}

void Troops::setAmount(int amount)
{
    this->amount = amount;
}

#endif
