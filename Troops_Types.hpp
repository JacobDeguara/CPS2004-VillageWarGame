#ifndef __TROOPS_TYPES_H__
#define __TROOPS_TYPES_H__
#include "Troops.hpp"

class Archer: public Troops
{
private:
public:
    Archer(int health,int damage,int acc,int carryingCapacity); 
};

Archer::Archer(int health,int damage,int acc,int carryingCapacity): Troops(health ,damage ,acc,carryingCapacity){
};

class Knight : public Troops
{
private:
public:
    Knight(int health,int damage,int acc,int carryingCapacity);
};

Knight::Knight(int health,int damage,int acc,int arryingCapacity): Troops(health ,damage ,acc,carryingCapacity) {};

class Defender : public Troops
{
private:
public:
    Defender(int health,int damage,int acc,int carryingCapacity);
    void upgrade(int num);
};

Defender::Defender(int health,int damage,int acc,int carryingCapacity): Troops(health ,damage ,acc,carryingCapacity) {}




#endif // __TROOPS_TYPES_H__