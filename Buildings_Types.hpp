#ifndef _BUILDING_TYPES_H_
#define _BUILDING_TYPES_H_
#include "Building.hpp"

class WoodCutter: public Building
{
private:
    
public:
    WoodCutter();
    int getGen();
};

WoodCutter::WoodCutter() :Building(){}

int WoodCutter::getGen()
{
    return amount*2 + amount*level;
};

/*---------------------------*/
class StoneMiner: public Building
{
private:
    
public:
    StoneMiner();
    int getGen();
};

StoneMiner::StoneMiner() :Building(){}

int StoneMiner::getGen()
{
    return amount*2 + amount*level;
};

/*---------------------------*/
class IronMiner: public Building
{
private:
    
public:
    IronMiner();
    int getGen();
};

IronMiner::IronMiner() :Building(){}

int IronMiner::getGen()
{
    return amount*2 + amount*level;
};

/*---------------------------*/
class BattleTrainer : public Building
{
private:
    
public:
    BattleTrainer();
    int getGen();
};

BattleTrainer::BattleTrainer() :Building(){};

int BattleTrainer::getGen()
{
    return (amount)+(amount*level);
}

/*---------------------------*/
class ArcheryRange : public Building
{
private:
    
public:
    ArcheryRange();
    int getGen();
};

ArcheryRange::ArcheryRange() :Building(){}

int ArcheryRange::getGen()
{
    return amount + level;
};

/*---------------------------*/
class KnightingPalace: public Building
{
private:
    
public:
    KnightingPalace();
    int getGen();
};

KnightingPalace::KnightingPalace() :Building(){}

int KnightingPalace::getGen()
{
    return amount + level;
};

/*---------------------------*/
class DefenderBarracks: public Building
{
private:
    
public:
    DefenderBarracks();
    int getGen();
};

DefenderBarracks::DefenderBarracks() :Building(){}

int DefenderBarracks::getGen()
{
    return amount + level;
};

#endif