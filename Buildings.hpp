#ifndef _BUILDING_H_
#define _BUILDING_H_

class Buildings
{
public:
    Buildings();
    void upgrade(int num);
    void add(int num);
    int getamount();
    int getLevel();
    virtual int getGen() = 0;
protected:
    unsigned int amount;
    unsigned int level;
};

Buildings::Buildings()
{
    this->amount = 0;
    this->level = 1;
}

void Buildings::upgrade(int num){
    level += num;
}

void Buildings::add(int num){
    amount += num;
}

int Buildings::getamount()
{
    return amount;
}

int Buildings::getLevel()
{
    return level;
}



#endif