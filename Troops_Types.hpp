#ifndef __TROOPS_TYPES_H__
#define __TROOPS_TYPES_H__
#include "Troops.hpp"

//The Archer class is a Troop thats ment to be weak in general but delivers massive damage for the team
class Archer: public Troops
{
private:
public:
    Archer(int health,int damage,int acc,int carryingCapacity); 
    void upgrade(int num);
};

Archer::Archer(int health,int damage,int acc,int carryingCapacity): Troops(health ,damage ,acc,carryingCapacity){};

void Archer::upgrade(int num){
    level += num;
    health += num;
    damage += 4*num;
    carryingCapacity += num;
}//7

//The Knight class is a Troop that is ment to be a more middle ground between tankyness and damage and everything else
class Knight : public Troops
{
private:
public:
    Knight(int health,int damage,int acc,int carryingCapacity);
    void upgrade(int num);
};

Knight::Knight(int health,int damage,int acc,int arryingCapacity): Troops(health ,damage ,acc,carryingCapacity) {};

void Knight::upgrade(int num){
    level += num;
    health += 2*num;
    damage += 2*num;
    carryingCapacity += 2*num;
}//7

//The Defender class is a Troop that is ment to be more tanky then anything and is able to carrylots of weight
class Defender : public Troops
{
private:
public:
    Defender(int health,int damage,int acc,int carryingCapacity);
    void upgrade(int num);
};

Defender::Defender(int health,int damage,int acc,int carryingCapacity): Troops(health ,damage ,acc,carryingCapacity) {};

void Defender::upgrade(int num){
    level += num;
    health += 3*num;
    damage += num;
    carryingCapacity += 2*num;
} //7 nums

#endif // __TROOPS_TYPES_H__