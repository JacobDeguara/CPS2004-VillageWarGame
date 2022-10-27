#ifndef _BUILDING_H_
#define _BUILDING_H_

class Building
{
public:
    Building();
    void upgrade(int num);
    void add(int num);
    int getamount();
    int getLevel();
    int RoundEnd();
protected:
    unsigned int amount;
    unsigned int level;
};

Building::Building()
{
    this->amount = 0;
    this->level = 1;
}

void Building::upgrade(int num){
    level += num;
}

void Building::add(int num){
    amount += num;
}

int Building::getamount()
{
    return amount;
}

int Building::getLevel()
{
    return level;
}

int Building::RoundEnd()
{
    return 0;
}

#endif