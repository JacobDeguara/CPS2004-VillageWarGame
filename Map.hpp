#ifndef __MAP_H__
#define __MAP_H__

#include "Player.hpp"

#include <vector>

using std::vector;

class Map
{
private:
    vector<vector<int>> Map;
public:
    Map(vector<Player> p;);
    ~Map();
};

Map::Map(/* args */)
{
}

Map::~Map()
{
}


#endif // __MAP_H__