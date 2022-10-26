#ifndef _BUILDING_TYPES_H_
#define _BUILDING_TYPES_H_
#include "Building.hpp"

class WoodCutter: public Building
{
private:
    
public:
    WoodCutter(int amount,int level);
    
};

WoodCutter::WoodCutter(int amount,int level) :Building(amount,level){}

class StoneMiner: public Building
{
private:
    
public:
    StoneMiner(int amount,int level);
    
};

StoneMiner::StoneMiner(int amount,int level) :Building(amount,level){}

class IronMiner: public Building
{
private:
    
public:
    IronMiner(int amount,int level);
    
};

IronMiner::IronMiner(int amount,int level) :Building(amount,level){}

class BattleTrainer : public Building
{
private:
    
public:
    BattleTrainer(int amount,int level);
    
};

BattleTrainer::BattleTrainer(int amount,int level) :Building(amount,level){}

class ArcheryRange : public Building
{
private:
    
public:
    ArcheryRange(int amount,int level);
    
};

ArcheryRange::ArcheryRange(int amount,int level) :Building(amount,level){}

class KnightingPalace: public Building
{
private:
    
public:
    KnightingPalace(int amount,int level);
    
};

KnightingPalace::KnightingPalace(int amount,int level) :Building(amount,level){}

class DefenderBarracks: public Building
{
private:
    
public:
    DefenderBarracks(int amount,int level);
    
};

DefenderBarracks::DefenderBarracks(int amount,int level) :Building(amount,level){}

#endif