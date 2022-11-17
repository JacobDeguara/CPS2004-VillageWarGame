#ifndef _BUILDING_TYPES_H_
#define _BUILDING_TYPES_H_
#include "Buildings.hpp"
/*---------------------------*/
//generates Wood
class WoodCutter: public Buildings
{
private:
    
public:
    WoodCutter();
    int getGen();
};

WoodCutter::WoodCutter() :Buildings(){}

int WoodCutter::getGen()
{
    return amount*2 + amount*level;
};

/*---------------------------*/
//Generates stone
class StoneMiner: public Buildings
{
private:
    
public:
    StoneMiner();
    int getGen();
};

StoneMiner::StoneMiner() :Buildings(){}

int StoneMiner::getGen()
{
    return amount*2 + amount*level;
};

/*---------------------------*/
//Generates Iron
class IronMiner: public Buildings
{
private:
    
public:
    IronMiner();
    int getGen();
};

IronMiner::IronMiner() :Buildings(){}

int IronMiner::getGen()
{
    return (amount*2) + (amount*level);
};

/*---------------------------*/
//Generates Food
class BattleTrainer : public Buildings
{
private:
    
public:
    BattleTrainer();
    int getGen();
};

BattleTrainer::BattleTrainer() :Buildings(){};

int BattleTrainer::getGen()
{
    return (amount)+(amount*level);
}

/*---------------------------*/
//Generates Aracher troops
class ArcheryRange : public Buildings
{
private:
    
public:
    ArcheryRange();
    int getGen();
};

ArcheryRange::ArcheryRange() :Buildings(){}

int ArcheryRange::getGen()
{
    return amount+(amount*level);
};

/*---------------------------*/
//generates Knights Troops
class KnightingPalace: public Buildings
{
private:
    
public:
    KnightingPalace();
    int getGen();
};

KnightingPalace::KnightingPalace() :Buildings(){}

int KnightingPalace::getGen()
{
    return amount+(amount*level);
};

/*---------------------------*/
//generates Defender troops
class DefenderBarracks: public Buildings
{
private:
    
public:
    DefenderBarracks();
    int getGen();
};

DefenderBarracks::DefenderBarracks() :Buildings(){}

int DefenderBarracks::getGen()
{
    return amount+(amount*level);
};

#endif