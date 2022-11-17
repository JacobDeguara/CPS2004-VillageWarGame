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
    virtual int getGen();
protected:
    unsigned int amount;
    unsigned int level;
};

/**
 * @brief Construct a new Buildings:: Buildings object
 * -- sets it to level = 1, amount = 0
 * 
 */
Buildings::Buildings()
{
    this->amount = 0;
    this->level = 1;
}

/**
 * @brief Upgrades Buildings
 * 
 * @param num 
 */
void Buildings::upgrade(int num){
    level += num;
}

/**
 * @brief Adds more buildings
 * 
 * @param num 
 */
void Buildings::add(int num){
    amount += num;
}

/**
 * @brief returns the amount
 * 
 * @return int 
 */
int Buildings::getamount()
{
    return amount;
}

/**
 * @brief returns the level
 * 
 * @return int 
 */
int Buildings::getLevel()
{
    return level;
}

/**
 * @brief returns the amount to generate for each resources
 * 
 * @return int 
 */
int Buildings::getGen(){}

#endif