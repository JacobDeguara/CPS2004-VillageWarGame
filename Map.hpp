#ifndef __MAP_H__
#define __MAP_H__

#include "Player.hpp"

#include <vector>
#include <cstdlib>
#include <ctime>

using std::vector;

struct pos{
    int x;
    int y;
};

class Map
{
private:
    vector<pos> position;
    vector<int> IsDead;
    int Max;

    pos make_pos(int x,int y);
public:
    Map(int maxNumOfPlayers);
    ~Map() = default;

    int getPosX(int i);
    int getPosY(int i);
    int getMax();

    void PlayerIsDead(int ID);
    bool IsPlayerDead(int ID);
    
};

pos Map::make_pos(int x, int y) {
    pos mypos = {x, y};
    return mypos;
}

Map::Map(int maxNumOfPlayers)
{
    srand(time(0));
    //we have a map of 16/65 which mean each player has to sit in one of these pos with close to even spacing
    //ok so i got a good idea
    Max = maxNumOfPlayers;

    //the first thing i need to do is spilt the map (65/16) base on the Num of Players evenly
    //next we take the range and we get the coord in that range for x,y

    int offsetX,offsetY;
    int rangeX,rangeY;

    rangeX = offsetX = 63/Max; //we also need to be rounded and with decimal points
    rangeY = offsetY = 16/Max; // this means overlap is possible but hopefully that happenes rarely 
    //range will give is the box size 

    int randX,randY;
    int offsetdistX=0,offsetdistY=0;
    int temp;
    // do 0,0 box
    randX = 1 + (offsetX*offsetdistX + rand()%rangeX);
    randY = 1 + (offsetY*offsetdistY + rand()%rangeY);
    position.push_back(make_pos(randX,randY));

    //we move xoffset by 1 but get a random y offset
    //then get a random pos for the box
    for (offsetdistX = 1; offsetdistX < Max; offsetdistX++)
    {   
        //this is so we dont get the same pos
        do{
            temp = offsetdistY;
            offsetdistY = 0+rand()%(Max+1);
        }while(temp == offsetdistY || temp == offsetdistY+1 || temp == offsetdistY -1);

        randX = 1 + (offsetX*offsetdistX + rand()%rangeX);
        randY = 1 + (offsetY*offsetdistY + rand()%rangeY);

        while(randY >= 17){
            randY--;
        }
        
        position.push_back(make_pos(randX,randY));   
    }


    //assign all player not dead
    IsDead.assign(Max,false);
}

int Map::getPosX(int i){
    return position[i].x;
}

int Map::getPosY(int i){
    return position[i].y;
}

int Map::getMax(){
    return Max;
}

void Map::PlayerIsDead(int ID){
    IsDead.at(ID) = true;
}

bool Map::IsPlayerDead(int ID){
    return IsDead.at(ID) == true;
}

#endif // __MAP_H__