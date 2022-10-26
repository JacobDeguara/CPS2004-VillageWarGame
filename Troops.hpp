#ifndef _TROOP_H_
#define _TROOP_H_

class Troops
{
public:
    Troops(int health ,int damage ,int acc,int amount,int level);
    void upgrade(int num);
    void add(int num);
protected:
    unsigned int amount;
    unsigned int level;
    int health;
    int damage;
    int acc;
};

Troops::Troops(int health ,int damage ,int acc,int amount,int level)
{   
    this->amount = amount;
    this->level = level;
    this->health = health;
    this->damage = damage;
    this->acc =acc;
}

void Troops::upgrade(int num){
    level += num;
}

void Troops::add(int num){
    amount += num;
}

#endif