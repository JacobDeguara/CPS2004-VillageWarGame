#ifndef _BUILDING_TYPES_H_
#define _BUILDING_TYPES_H_
#include "Building.hpp"

class WoodCutter: public Building
{
private:
    
public:
    WoodCutter();
    int RoundEnd();
};

WoodCutter::WoodCutter() :Building(){};

int WoodCutter::RoundEnd()
{
    return (amount*2)+(amount*level);
}

/*---------------------------*/
class StoneMiner: public Building
{
private:
    
public:
    StoneMiner();
    int RoundEnd();
};

StoneMiner::StoneMiner() :Building(){};

int StoneMiner::RoundEnd()
{
    return (amount*2)+(amount*level);
}

/*---------------------------*/
class IronMiner: public Building
{
private:
    
public:
    IronMiner();
    int RoundEnd();
};

IronMiner::IronMiner() :Building(){};

int IronMiner::RoundEnd()
{
    return (amount*2)+(amount*level);
}

/*---------------------------*/
class BattleTrainer : public Building
{
private:
    
public:
    BattleTrainer();
    int RoundEnd();
};

BattleTrainer::BattleTrainer() :Building(){};

int BattleTrainer::RoundEnd()
{
    return (amount)+(amount*level);
}

/*---------------------------*/
class ArcheryRange : public Building
{
private:
    
public:
    ArcheryRange();
    int RoundEnd();
};

ArcheryRange::ArcheryRange() :Building(){};

int ArcheryRange::RoundEnd()
{
    return (amount)+(level);
}

/*---------------------------*/
class KnightingPalace: public Building
{
private:
    
public:
    KnightingPalace();
    int RoundEnd();
};

KnightingPalace::KnightingPalace() :Building(){};

int KnightingPalace::RoundEnd()
{
    return (amount)+(level);
}

/*---------------------------*/
class DefenderBarracks: public Building
{
private:
    
public:
    DefenderBarracks();
    int RoundEnd();
};

DefenderBarracks::DefenderBarracks() :Building(){};

int DefenderBarracks::RoundEnd()
{
    return (amount)+(level);
}


#endif