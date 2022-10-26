#ifndef _PLAYER_H_
#define _PLAYER_H_
using namespace std;

class Player
{
private:
    struct{
        int wood =10;
        int stone =10;
        int iron =0;
        int food =3;
        int size = 4;
    }res;
public:
    Player(/* args */);
    int getWood();
    int getIron();
    int getStone();
    int getFood();
    int getResSize();
};

Player::Player(/* args */)
{
}

int Player::getWood(){
    return res.wood;
}

int Player::getIron(){
    return res.iron;
}

int Player::getStone(){
    return res.stone;
}

int Player::getFood(){
    return res.food;
}

int Player::getResSize(){
    return res.size;
}



#endif