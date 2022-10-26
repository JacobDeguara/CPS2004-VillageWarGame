#ifndef _BUILDING_H_
#define _BUILDING_H_

class Building
{
public:
    Building(int amount,int level);
    void upgrade(int num);
    void add(int num);
protected:
    unsigned int amount;
    unsigned int level;
};

Building::Building(int amount,int level)
{
    this->amount = amount;
    this->level = level;
}

void Building::upgrade(int num){
    level += num;
}

void Building::add(int num){
    amount += num;
}

#endif