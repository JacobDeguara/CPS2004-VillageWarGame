#ifndef _PLAYER_H_
#define _PLAYER_H_

class Building
{
public:
    int level;
    int amount;
    void build(int num1,int num2){
        level = num1;
        amount = num2;
    }
};

class WoodCutter: 
{
private:
public:
    WoodCutter();
};

WoodCutter::WoodCutter()
{

}
