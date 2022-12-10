#ifndef __MAP_H__
#define __MAP_H__

#include "Player.hpp"

#include <vector>
#include <cstdlib>
#include <ctime>

using std::vector;

/**
 * @brief a struct ment to hold 2 variables representing x,y for positing
 * 
 */
struct pos{
    int x;
    int y;
};

/**
 * @brief The Map Class Holds The position of each player and determines the position in the constucter,
 * @brief it also holds the Dead players and works with the Players their ID's 
 */
class Map
{
private:
    //Holds each position of Player
    vector<pos> position;
    //Holds a bool of each player if their dead
    vector<bool> IsDead;
    //holds the max value of players
    int max;
    int aiMax;
    
    pos make_pos(int x,int y);
public:
    Map(int maxNumOfPlayers,int maxNumOfAi);
    ~Map() = default;

    int getPosX(int i);
    int getPosY(int i);
    int getMax();
    int getMaxMax();
    int getMaxAi();

    void PlayerIsDead(int ID);
    bool IsPlayerDead(int ID);
    
};

/**
 * @brief Creates a pos struct with x,y parameters for the vector to hold
 * 
 * @param x 
 * @param y 
 * @return pos 
 */
pos Map::make_pos(int x, int y) {
    pos mypos = {x, y};
    return mypos;
}

/**
 * @brief Construct a new Map:: Map object,
 * and assign a position to each player
 * 
 * @param maxNumOfPlayers 
 */
Map::Map(int maxNumOfPlayers,int maxNumOfAi)
{   

    /**
     * The way im setting the position of each player is by takeing the size of the map
     * setting the offset and range to be equal to the map / by the number of players, this will give us a box each
     * Each box the player will the put in it useing rand,
     * and we will offset the box by offset so we can space them out better
     * (each box is a square)
     */

    //Randomize rand
    srand(time(0));
    
    max = maxNumOfPlayers;
    aiMax = maxNumOfAi;

    //each ofset and range for x and y for rand
    int offsetX,offsetY;
    int rangeX,rangeY;

    //setting range and offset to me 
    rangeX = offsetX = 60/(max+aiMax);
    rangeY = offsetY = 20/(max+aiMax); 

    int randX,randY; // The random position we use
    int offsetdistX=0,offsetdistY=0;    
    int temp =-2;

    for (offsetdistX = 0; offsetdistX < max; offsetdistX++)
    {   
        do{
            offsetdistY = rand()%max; //get a random offset
        }while(temp == offsetdistY); //that isnt the same as the before (to hopefully prevent any player next to each other)
        temp = offsetdistY; // set temp to offset
        
        //get random x, y cords
        randX = 1 + (offsetX*offsetdistX + rand()%rangeX);
        randY = 1 + (offsetY*offsetdistY + rand()%rangeY);

        while(randY >= 17){ // just incase of out of bouce
            randY = 16;
        }

        while(randX >= 64){ // just incase of out of bouce
            randX = 63;
        }

        position.push_back(make_pos(randX,randY));// put position in vector
    }

    for (; offsetdistX < max + aiMax; offsetdistX++)
    {   
        do{
            offsetdistY = rand()%max; //get a random offset
        }while(temp == offsetdistY); //that isnt the same as the before (to hopefully prevent any player next to each other)
        temp = offsetdistY; // set temp to offset
        
        //get random x, y cords
        randX = 1 + (offsetX*offsetdistX + rand()%rangeX);
        randY = 1 + (offsetY*offsetdistY + rand()%rangeY);

        while(randY >= 17){ // just incase of out of bouce
            randY = 16;
        }

        while(randX >= 64){ // just incase of out of bouce
            randX = 63;
        }

        position.push_back(make_pos(randX,randY));// put position in vector
    }
    
    //assign all player not dead
    IsDead.assign(max+aiMax,false);
    
}

/**
 * @brief Returns the X position of the player using "THEIR ID"
 * 
 * @param i 
 * @return int 
 */
int Map::getPosX(int i){
    return position[i].x;
}

/**
 * @brief Returns the Y position of the player using "THEIR ID"
 * 
 * @param i 
 * @return int 
 */
int Map::getPosY(int i){
    return position[i].y;
}

/**
 * @brief Returns the Max number of players
 * 
 * @return int 
 */
int Map::getMax(){
    return max;
}

int Map::getMaxMax(){
    return max+aiMax;
}

int Map::getMaxAi(){
    return aiMax;
}

/**
 * @brief Sets player to dead based on "THEIR ID"
 * 
 * @param ID 
 */
void Map::PlayerIsDead(int ID){
    IsDead.at(ID) = true;
}

/**
 * @brief Checks if Player is Dead using "THEIR ID"
 * 
 * @param ID 
 * @return true 
 * @return false 
 */
bool Map::IsPlayerDead(int ID){
    return IsDead.at(ID) == true;
}

#endif // __MAP_H__