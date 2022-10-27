#ifndef __TROOPS_TYPES_H__
#define __TROOPS_TYPES_H__
#include "Troops.hpp"

class Archer: public Troops
{
private:
    int health = 5;
    int damage = 10;
    int acc = 5;
    int carryingCapacity = 15;
public:
    Archer(); 
};

Archer::Archer(): Troops(health ,damage ,acc,carryingCapacity){
};

class Knight : public Troops
{
private:
    int health = 10;
    int damage = 5;
    int acc = 8;
    int carryingCapacity = 10;
public:
    Knight();
};

Knight::Knight(): Troops(health ,damage ,acc,carryingCapacity) {};

class Defender : public Troops
{
private:
    int health = 20;
    int damage = 4;
    int acc = 9;
    int carryingCapacity = 5;
public:
    Defender(/* args */);
    
};

Defender::Defender(/* args */): Troops(health ,damage ,acc,carryingCapacity) {};



#endif // __TROOPS_TYPES_H__