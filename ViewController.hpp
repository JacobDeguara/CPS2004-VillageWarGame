#include <ncurses.h>
#include <vector>
#include <string>
#include <array>
#include <cmath>

#include "Attack.hpp"
#include "Player.hpp"
#include "Map.hpp"
#include "Attack.hpp"

#ifndef __VIEWCONTROLLER_H__
#define __VIEWCONTROLLER_H__

using std::string;
using std::vector;
using std::array;

/**
 * @brief the ViewController class Manages the visual and Control aspects of the game
 * 
 */
class ViewController
{
private:

    //Every window being used globally

    WINDOW * menuwin;
    WINDOW * submenuwin;
    WINDOW * reswin;
    WINDOW * inwin;
    WINDOW * mapwin;

    //this is mainly to simply nameing and compatibitly

    struct{
        string menuName;
        vector<string>submenuNames;
        int submenuSize;
    }MenuNames[4];

    //these inputs are related to the output stuff 

    int highlight1 = 0;
    int highlight2 =-1;
    int highlight3 =-1;
    int highlight4row =-1;
    int highlight4col =-1;
    int count = 1,playerCount = 0;
    int archerC=0,knightC=0,defenderC=0;
    int wood=0,stone=0,iron=0,food=0;

    //setting up functions

    WINDOW * VisualCreation(int x1,int y1,int x2,int y2);
    void windowCreation(int yMax, int xMax);

    void choicesCreation();
    void costCreationTroop(int food,Player * currentPlayer); 
    void costCreationBuildings(int wood,int stone,int iron,int food,Player * currentPlayer);
    void updateMap(shared_ptr<Map> map);

    //update windows seperatly

    void menuUpdate(const char * str);
    void submenuUpdate(const char * str);
    void resUpdate(const char * str,Player * currentPlayer);
    void inputUpdate(const char * str,Player * currentPlayer,int playerNum,shared_ptr<Map> map);
    void mapUpdate(const char * str,Player * currentPlayer,shared_ptr<Map> map,Attack AtkList);

public:
    ViewController();
    ~ViewController() = default;

    //These are all menu subsystem is to allow the player get an inputs

    int getMenuMV(); 
    int getSubMenuMV(); 
    int getInputMenuMV(shared_ptr<Map> map); 
    void prepSubMenu();
    void prepInputMenu(shared_ptr<Map> map); 
    void resetMenu(); 
    int resetMenuPartial(); 

    //These are all updates and runners to allow comtaibility 

    void updateStart(int &numOfPlayer, int &numOfAi);
    void update(Player * currentPlayer,int playerNum,shared_ptr<Map> map,Attack AtkList);
    void refresh();
    int StartAttack(shared_ptr<Holder> Attack);
    
    //These are all get stuff for the control aspect

    int getCount();
    int getWoodCost();
    int getStoneCost();
    int getIronCost();
    int getFoodCost();
    int getChoice1();
    int getChoice2();
    int getArcherCount();
    int getKnightCount();
    int getDefenderCount();
    int getAttackChoice();
    int getPlayerCount();

    void sendMsg(vector<string> str);
};

/**
 * @brief Construct a new View Controller:: View Controller object -- Creates the windows and sets up menus
 */
ViewController::ViewController()
{   
    int yMax, xMax;
    getmaxyx(stdscr,yMax,xMax);
    windowCreation(yMax,xMax);
    choicesCreation();
}
/**
 * @brief Takes x1,x2,y1,y2 to use as a divition to create a window and return it
 * 
 * @param x1 
 * @param y1 
 * @param x2 
 * @param y2 
 * @return WINDOW* 
 */
WINDOW * ViewController::VisualCreation(int x1,int y1,int x2,int y2){
    int yMax,xMax;
    getmaxyx(stdscr,yMax,xMax);

    WINDOW * newWin = newwin(yMax/y1,xMax/x1,yMax/y2,xMax/x2); 
    box(newWin,0,0); //create box
    refresh();
    wrefresh(newWin);
    keypad(newWin,true);

    return newWin;
}

/**
 * @brief Creates each window to be used globally
 * 
 * @param yMax 
 * @param xMax 
 */
void ViewController::windowCreation(int yMax,int xMax){
    //windows Created    
        //resourse window
        reswin = newwin(yMax*5/8,xMax*7/16,0,0); 
        box(reswin,0,0); //create box
        refresh();
        wrefresh(reswin);
        keypad(reswin,true);

        //main menu Window
        menuwin = newwin((yMax*3/8)+1,xMax*2/8,yMax*5/8,0); //new window
        box(menuwin,0,0); //create box
        refresh();
        wrefresh(menuwin);
        keypad(menuwin,true);

        //sub menu Window
        submenuwin = newwin((yMax*3/8)+1,xMax*2/8,yMax*5/8,xMax*2/8); //new window
        box(submenuwin,0,0); //create box
        refresh();
        wrefresh(submenuwin);
        keypad(submenuwin,true);
        
        //Input Window
        inwin = newwin((yMax*3/8)+1,xMax*4/8,yMax*5/8,xMax*4/8); //new window
        box(inwin,0,0); //create box
        refresh();
        wrefresh(inwin);
        keypad(inwin,true);

        //Map Window
        mapwin = newwin(yMax*5/8,xMax*9/16,0,xMax*7/16); //new window
        box(mapwin,0,0); //create box
        refresh();
        wrefresh(mapwin);
        keypad(mapwin,true);
}

/**
 * @brief Creates the menu names in the stuct MENUNAMES
 */
void ViewController::choicesCreation()
{
    MenuNames[0].menuName = "Create Building";
    MenuNames[1].menuName = "Upgrade Building";
    MenuNames[2].menuName = "Upgrade Troops ";
    MenuNames[3].menuName = "Actions        ";

    string str1[] ={"Wood Cutter      ","Stone Miner      ","Iron Miner       ","Battle Trainer   ","Archery Range    ","Knighting Palace ","Defender Barracks"};
    string str2[] ={"Archer           ","Knight           ","Defender         "};
    string str3[] ={"Attack           ","End Turn         ","Surrender        "};
    int size1 = 7;
    int size2 = 3;
    MenuNames[0].submenuNames.assign(str1,str1+size1);
    MenuNames[0].submenuSize = size1;
    MenuNames[1].submenuNames.assign(str1,str1+size1);
    MenuNames[1].submenuSize = size1;
    MenuNames[2].submenuNames.assign(str2,str2+size2);
    MenuNames[2].submenuSize = size2;
    MenuNames[3].submenuNames.assign(str3,str3+size2);
    MenuNames[3].submenuSize = size2;
}

/**
 * @brief Takes Food and sets the proper cost for visual and control
 * 
 * @param food 
 * @param currentPlayer 
 */
void ViewController::costCreationTroop(int food,Player * currentPlayer){
    this->wood = 0;
    this->stone= 0;
    this->iron = 0;
    this->food = food * count + 1*(currentPlayer->troops[highlight2]->getLevel());
}

/**
 * @brief Takes parameters for each resource to set them for visual and control
 * 
 * @param wood 
 * @param stone 
 * @param iron 
 * @param food 
 * @param currentPlayer 
 */
void ViewController::costCreationBuildings(int wood,int stone,int iron,int food,Player * currentPlayer){
    this->wood = wood *(count *(currentPlayer->buildings[highlight2]->getamount()+currentPlayer->buildings[highlight2]->getLevel()));
    this->stone= stone *(count *(currentPlayer->buildings[highlight2]->getamount()+currentPlayer->buildings[highlight2]->getLevel()));
    this->iron = iron  *(count *(currentPlayer->buildings[highlight2]->getamount()+currentPlayer->buildings[highlight2]->getLevel()));
    this->food = food  *(count *(currentPlayer->buildings[highlight2]->getamount()+currentPlayer->buildings[highlight2]->getLevel()));
}

/**
 * @brief Gets the input from the user  --
 * This menu is (-1)
 * 
 * @return int 
 */
int ViewController::getMenuMV()
{
    int choice = wgetch(menuwin);
    switch(choice){
        case KEY_UP:
            highlight1--;
            if(highlight1 == -1){
                highlight1 = 0;
            }
            break;

        case KEY_DOWN:
            highlight1++;
            if(highlight1 == 4){
                highlight1 = 3;
            }
            break;

        case KEY_RIGHT:
            return -2;
            break;
        default:
            break;
    }
    return -1; // -2 is default loop for main menu
}

/**
 * @brief Gets the input from the user  --
 * This menu is (-2)
 * 
 * @return int 
 */
int ViewController::getSubMenuMV()
{
    int choice2 = wgetch(submenuwin);
    switch(choice2){
        case KEY_UP:
            highlight2--;
            if(highlight2 == -1){
                highlight2 = 0;
            }
            break;

        case KEY_DOWN:
            highlight2++;
            if(highlight2 == MenuNames[highlight1].submenuNames.size()){
                highlight2 = MenuNames[highlight1].submenuNames.size() - 1;
            }
            break;

        case KEY_LEFT:
            highlight2 = -1;
            return -1;
            break;
        
        case KEY_RIGHT:
            highlight3 = 0;
            return -3;
            break;
        default:
            break;
    }
    if(choice2 == 10){
        if(!(highlight2 == 0 && highlight1 == 3)){ // < attack
            return 0; //choice selected
        }
    }
    return -2; // -1 is default loop for sub menu
}

/**
 * @brief Gets the input from the user  --
 * This menu is (-3)
 * 
 * @return int 
 */
int ViewController::getInputMenuMV(shared_ptr<Map> map){
    if(highlight1 != 3){ // for choice 0-2 
        highlight4col = highlight4row =-1;
        int choice3 = wgetch(inwin);
        switch(choice3){
            case KEY_LEFT:
                highlight3--;
                if(highlight3 == -1){
                    count = 1;
                    return -2;
                }   
                break;

            case KEY_RIGHT:
                highlight3++;
                if(highlight3 == 3){
                    highlight3 = 2;
                }
                break;

            default:
                break;
        }

        if(choice3 == 10){
            if(highlight3==0){
                if(count != 1){
                    count--;
                }
            }else if(highlight3==2){
                count++;
            }else{
                return 0; //choice selected
            }
        }
        return -3;
    }

    if((highlight1 == 3) && (highlight2 == 0)){ //for Attack only
            highlight3 = -1;
        int choice3 = wgetch(inwin);

        switch(choice3){
            case KEY_LEFT:
                highlight4row--;
                if(highlight4row == -1){
                    knightC = archerC = defenderC = 0;
                    highlight4col = -1;
                    return -2;
                }    
                break;

            case KEY_RIGHT:
                highlight4row++;
                if(highlight4row == 3){
                    highlight4row = 2;
                }
                break;

            case KEY_UP:
                highlight4col--;
                if(highlight4col == -1){
                    highlight4col = 0;
                }
                break;

            case KEY_DOWN:
                highlight4col++;
                if(highlight4col == 5){
                    highlight4col = 4;
                }
                break;
            default:
                break;
        }

        if(choice3 == 10){
            if(highlight4col == 4){
                return 0;//selected choice
            }
            int temp;
            if(highlight4row==0){ // selected '<' = decrease by one
                switch(highlight4col){
                    case 0:
                        
                        playerCount--;
                        if(playerCount == -1){
                            playerCount = map->getMax()-1;
                        } 
                        while(map->IsPlayerDead(playerCount)){ //check if current player is dead
                            playerCount--; // reduce by once since thats what it does
                            if(playerCount == -1){ // checks if we whent beyond the 0-Max
                                playerCount = map->getMax()-1; // reverts to Max
                            }
                        }                   

                        break;
                    case 1:
                        if(archerC != 0){
                            archerC--;
                        }
                        break;
                    case 2:
                        if(knightC != 0){
                            knightC--;
                        }
                        break;
                    case 3:
                        if(defenderC != 0){
                            defenderC--;
                        }
                        break;
                }
            }else if(highlight4row==2){ //selected '>' = increase by one
                
                switch(highlight4col){
                    case 0:
                        
                        playerCount++;
                        if (playerCount >= map->getMax())
                        {
                            playerCount = 0;
                        }
                        while (map->IsPlayerDead(playerCount))
                        {
                            playerCount++; // add by once since thats what it does
                            if(playerCount >= map->getMax()){ // checks if we whent beyond the 0-Max
                                playerCount = 0; // reverts to 0
                            }
                        }
                        
                        break;
                    case 1:
                        archerC++;
                        break;
                    case 2:
                        knightC++;
                        break;
                    case 3:
                        defenderC++;
                        break;
                }
            }
        }

        return -3;
    }
    
    highlight3 = -1;
    return -2;
}

/**
 * @brief Prepares the sub menu
 * 
 */
void ViewController::prepSubMenu(){
    if(highlight2 == -1){
        highlight2 = 0;
    }  
}

/**
 * @brief Prepares the Input menu
 * 
 * @param map - pointer
 */
void ViewController::prepInputMenu(shared_ptr<Map> map){
    count = 1;
    archerC =0;
    knightC = 0;
    defenderC = 0;
    highlight3 =0;
    highlight4col = 0;
    highlight4row = 0;

    playerCount = 0;
    while(map->IsPlayerDead(playerCount)){
        playerCount++;
    }
}

/**
 * @brief Resets the menu completly from the start
 * 
 */
void ViewController::resetMenu(){
    highlight1 = 0;
    highlight2 = -1;
    highlight3 = -1;
    highlight4col = -1;
    highlight4row = -1;
    archerC =0;
    knightC = 0;
    defenderC = 0;
    count = 1;
    playerCount = 0;
}

/**
 * @brief Resets the menu Partially to not remove the current selection 
 * 
 * @return int 
 */
int ViewController::resetMenuPartial(){
    highlight3 = -1;
    highlight4col = -1;
    highlight4row = -1;
    count = 1;
    playerCount = 0;
    if(highlight2 == -1){
        return -1;
    }else{
        return -2;
    }
}

/**
 * @brief Runs the Main-Menu and returns the number of players and Ai
 * 
 * @param numOfPlayer 
 * @param numOfAi 
 */
void ViewController::updateStart(int &numOfPlayer, int &numOfAi){
    
    WINDOW * win = VisualCreation(2,2,4,4);
    bool dontEndStartGame = true;
    int Input = 0;
    highlight1= 0;
    highlight2= 0;
    numOfPlayer = 1;
    numOfAi = 0;

    mvwprintw(win,0,1,"Welcome to Village War Game:");
    mvwprintw(win,1,1,"                  Made by Jacob Deguara");
    mvwprintw(win,2,1," Lets start! By using the Left, Right & Down Keys to move.");
    mvwprintw(win,3,1,"   Then use the Enter Key to move the add Players or Ai.");
    mvwprintw(win,4,1,"   Once your done Select the Start Game button to start.");
    mvwprintw(win,6,1,"Please select the following:");
    do{
        mvwprintw(win,8,1,"Num of players: ");

        if(highlight1==0 && highlight2 ==0){
            wattron(win,A_REVERSE);
        }
        wprintw(win,"<");
        wattroff(win,A_REVERSE);

         wprintw(win,"\t");
        
        if(highlight1==1 && highlight2 ==0){
            wattron(win,A_REVERSE);
        }
        wprintw(win,"%d",numOfPlayer);
        wattroff(win,A_REVERSE);

         wprintw(win,"\t");
        
        if(highlight1==2 && highlight2 ==0){
            wattron(win,A_REVERSE);
        }
        wprintw(win,">");
        wattroff(win,A_REVERSE);

        mvwprintw(win,9,1,"Num of Ai's:\t ");

        if(highlight1==0 && highlight2 ==1){
            wattron(win,A_REVERSE);
        }
        wprintw(win,"<");
        wattroff(win,A_REVERSE);

         wprintw(win,"\t");
        
        if(highlight1==1 && highlight2 ==1){
            wattron(win,A_REVERSE);
        }
        wprintw(win,"%d",numOfAi);
        wattroff(win,A_REVERSE);

         wprintw(win,"\t");
        
        if(highlight1==2 && highlight2 ==1){
            wattron(win,A_REVERSE);
        }
        wprintw(win,">");
        wattroff(win,A_REVERSE);

        mvwprintw(win,10,1,"\t\t ");
        if(highlight2==2){
            wattron(win,A_REVERSE);
        }
        wprintw(win,"> Start Game! <" );
        wattroff(win,A_REVERSE);
        
        Input = wgetch(win);
        switch (Input)
        {
        case KEY_LEFT:
                highlight1--;
                if(highlight2 == 2){
                    highlight1 = 1;
                }else if(highlight1 == -1){
                    highlight1 = 0;
                }
            break;
        
        case KEY_RIGHT:
                highlight1++;
                if(highlight2 == 2){
                    highlight1 = 1;
                }else if(highlight1 == 3){
                    highlight1 = 2;
                }
            break;

        case KEY_UP:
                highlight2--;
                if(highlight2 == -1){
                    highlight2 = 0;
                }
            break;

        case KEY_DOWN:
                highlight2++;
                if(highlight2 == 3){
                    highlight2 = 2;
                }
            break;
        
        case 10:
            if(highlight2 == 2){
                dontEndStartGame = false;
            }else if(highlight1 == 0){ // <
                if(highlight2 == 0){ // players
                    if(numOfPlayer != 1){
                        numOfPlayer--;
                    }
                }else{
                    if(numOfAi != 0){
                        numOfAi--;
                    }
                }
            }else if(highlight1 == 2){ // >
                if(highlight2 == 0){
                    if(numOfPlayer != 10){
                        numOfPlayer++;
                    }
                }else{
                    if(numOfAi != 5){
                        numOfAi++;
                    }
                }
            }

        default:
            break;
        }

    }while(dontEndStartGame); 

    wclear(win);
    wrefresh(win);
}

/**
 * @brief Updates the Windows cash to update any changes
 * 
 * @param currentPlayer The Player that is currently in play
 * @param playerNum The number of players
 * @param map - the map object
 * @param AtkList - The entire attack object for the list
 */
void ViewController::update(Player * currentPlayer,int playerNum,shared_ptr<Map> map,Attack AtkList){
    const char * str;

    //rest boxes incase of clears
    box(menuwin,0,0);
    box(reswin,0,0);
    box(submenuwin,0,0);
    box(mapwin,0,0);
    box(inwin,0,0);

    menuUpdate(str);
    submenuUpdate(str);
    resUpdate(str,currentPlayer);
    inputUpdate(str,currentPlayer,playerNum,map);
    updateMap(map);
    mapUpdate(str,currentPlayer,map,AtkList);
    
}

/**
 * @brief Updates the menu window
 * 
 * @param str 
 */
void ViewController::menuUpdate(const char * str){
    //Menu window  
    mvwprintw(menuwin,0,1,"Menu-(UP/DOWN/LEFT)");

    //loop to print all menuNames and highlight1 the selected option
    for (int i = 0; i < 4; i++)
    {
        if(i == highlight1){ //if highlighted
            wattron(menuwin,A_REVERSE);
        }
        str = (MenuNames[i].menuName).c_str();
        mvwprintw(menuwin,i+1,1,"> %s",str); //print string
        wattroff(menuwin,A_REVERSE);    
    }
}

/**
 * @brief Updates the sub menu window
 * 
 * @param str 
 */
void ViewController::submenuUpdate(const char * str){
    //SubMenu Window
    mvwprintw(submenuwin,0,1,"Submenu-(UP/DOWN/ENTER/LEFT)");

    for (int i = 0; i < 7; i++)
    {   
        //clear any remaining Names that could be left 
        if(i >= MenuNames[highlight1].submenuNames.size()){
            mvwprintw(submenuwin,i+1,1,"                   "); //remove string
            continue;
        }

        //print the new names
        if(!MenuNames[highlight1].submenuNames[i].empty()){ //if not empty
            if(i == highlight2){ //if highlighted
                wattron(submenuwin,A_REVERSE);
            }
            str = (MenuNames[highlight1].submenuNames[i].data());
            mvwprintw(submenuwin,i+1,1,"> %s",str); //print string
        }
        
        wattroff(submenuwin,A_REVERSE);    
    }

}

/**
 * @brief Updates the Res window
 * 
 * @param str 
 * @param currentPlayer 
 */
void ViewController::resUpdate(const char * str,Player * currentPlayer){
    //Resource Window
    mvwprintw(reswin,0,1,"Resources");
    int j=1,jHold;

    //print basic Resources (wood,stone,iron,food)
    for(int i = 0; i < currentPlayer->getResSize(); i++)
    {   
        int num;
        switch(i){
            case 0:
                str = "Wood : ";
                num = currentPlayer->getWood();
                break;
            case 1:
                str = "Stone: ";
                num = currentPlayer->getStone();
                break;
            case 2:
                str = "Iron : ";
                num = currentPlayer->getIron();
                break;
            case 3:
                str = "Food : ";
                num = currentPlayer->getFood();
                break;
            case 4:
                break;
            default:
            break;
        }
        mvwprintw(reswin,j,1,"%s%d    ",str,num); //print string
        j++;
    }

    //next Resource is the Buildings the village has
    jHold = j;
    j++;
    for(int i = 0; i < 7; i++)
    {   
        
        int amount,level,gen;
        str = MenuNames[0].submenuNames[i].data();
        amount = currentPlayer->buildings[i]->getamount();
        level = currentPlayer->buildings[i]->getLevel();
        gen = currentPlayer->buildings[i]->getGen();
        if(amount != 0){
            mvwprintw(reswin,j,1,"%s: Qty:%d Lvl:%d Gen:%d       ",str,amount,level,gen);
            j++;
        }
    }
    if((jHold+1) != j){ // we are checking if we have any Buildings shown
        mvwprintw(reswin,jHold,1,"Buildings:                               ");
    }else{
        j--;
    }
    
    //next Resources is the Troops the village has
    jHold = j;
    j++;
    for(int i = 0; i < 3; i++)
    {   
        int temp;
        int amount,level,powR;
        str = MenuNames[2].submenuNames[i].data();
        amount = currentPlayer->troops[i]->getAmount();
        level = currentPlayer->troops[i]->getLevel();
        powR = currentPlayer->troops[i]->getPowerRating();
        if(amount != 0){
            mvwprintw(reswin,j,1,"%s: Qty:%d Lvl:%d Power:%d     ",str,amount,level,powR);
            j++;
        }
    }
    if((jHold+1) != j){ // we are checking if we have any Troops shown
        mvwprintw(reswin,jHold,1,"Troops:                                  ");
    }else{
        j--;
    }

    //clear the resaurces slices not used
    for(int i = j;i < 17;i++){
        mvwprintw(reswin,i,1,"                                             ");
    }

}

/**
 * @brief Updates the input window
 * 
 * @param str 
 * @param currentPlayer 
 * @param playerNum 
 * @param map 
 */
void ViewController::inputUpdate(const char * str,Player * currentPlayer,int playerNum,shared_ptr<Map> map){
    //Input Window
    mvwprintw(inwin,0,1,"Inputs-Menu");
    if((highlight1 != 3)&&(highlight2 != -1)){
        switch (highlight2){
        case 0:
            switch (highlight1){
            case 0:
                costCreationBuildings(2,2,0,0,currentPlayer);                                                                                 //create wood cutter
                mvwprintw(inwin,1,1,"<Create more Wood Cutters> ");
            break;
            case 1:
                costCreationBuildings(2,2,2,1,currentPlayer);
                mvwprintw(inwin,1,1,"<Upgrading the Wood Cutter>");
            break;
            case 2:
                costCreationTroop(1,currentPlayer);
                mvwprintw(inwin,1,1,"<Train the Archers>        ");
            break;
            default:
                costCreationBuildings(0,0,0,0,currentPlayer);
                break;
            }
            mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
        break;

        case 1:
            switch (highlight1){
            case 0:
                costCreationBuildings(2,0,2,0,currentPlayer);
                mvwprintw(inwin,1,1,"<Create more Stone Miners> ");
            break;
            case 1:
                costCreationBuildings(3,0,3,1,currentPlayer);
                mvwprintw(inwin,1,1,"<Upgrades the Stone Miner> ");
            break;
            case 2:
                costCreationTroop(1,currentPlayer);
                mvwprintw(inwin,1,1,"<Train the Knights>        ");
            break;
            default:
                costCreationBuildings(0,0,0,0,currentPlayer);
                break;
            }
            mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
        break;

        case 2:
            switch (highlight1){
            case 0:
                costCreationBuildings(3,2,0,0,currentPlayer);                                                                                 //create iron miner
                mvwprintw(inwin,1,1,"<Create more Iron Miners>  ");
            break;
            case 1:
                costCreationBuildings(3,3,0,1,currentPlayer);
                mvwprintw(inwin,1,1,"<Upgrades the Iron Miner>  ");
            break;
            case 2:
                costCreationTroop(1,currentPlayer);
                mvwprintw(inwin,1,1,"<Train the Defenders>     ");
            break;
            default:
                costCreationBuildings(0,0,0,0,currentPlayer);
                break;
            }
            mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
        break;
        case 3:
            if(highlight1==0){                                                                                   //create Battle Trainer
                costCreationBuildings(2,2,2,0,currentPlayer);
                mvwprintw(inwin,1,1,"<Create more Food stalls>  ");
            }else if(highlight1==1){                                                                             // Upgrade Battle Trainer
                costCreationBuildings(2,2,2,0,currentPlayer);
                mvwprintw(inwin,1,1,"<Upgrades the Battle Tower>");
            }
            mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
        break;

        case 4:
            if(highlight1==0){                                                                                   //create Archer place
                costCreationBuildings(2,2,0,2,currentPlayer);
                mvwprintw(inwin,1,1,"<Creates more Archers>     "); 
            }else if(highlight1==1){  
                costCreationBuildings(2,0,2,1,currentPlayer);                                                                           // Upgrade Archer place
                mvwprintw(inwin,1,1,"<Upgrade The Range>        ");
            }
            mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
        break;

        case 5:
            if(highlight1==0){  
                costCreationBuildings(1,1,1,2,currentPlayer);                                                                                 //create Knight place
                mvwprintw(inwin,1,1,"<Creates more Knights>     ");
            }else if(highlight1==1){                                                                             // Upgrade Knight place
                costCreationBuildings(1,1,1,1,currentPlayer);
                mvwprintw(inwin,1,1,"<Upgrades The Palace>      ");
            }
            mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
        break;

        case 6:
            if(highlight1==0){   
                costCreationBuildings(1,1,1,3,currentPlayer);                                                                                //create Defender place
                mvwprintw(inwin,1,1,"<Creates more Defenders>   ");
            }else if(highlight1==1){                                                                             // Upgrade Defender place
                costCreationBuildings(1,2,1,1,currentPlayer);
                mvwprintw(inwin,1,1,"<Upgrades the Barackes>    ");
            }
            mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
        break;
        }

        if(highlight2 != -1){
                if(highlight3 == 0){
                    wattron(inwin,A_REVERSE);
                }
                mvwprintw(inwin,3,1,"<");
                wattroff(inwin,A_REVERSE);
                
                wprintw(inwin,"\t");
                if(highlight3 == 1){
                    wattron(inwin,A_REVERSE);
                }
                wprintw(inwin,"%d",count);
                wattroff(inwin,A_REVERSE);
                wprintw(inwin,"\t");
                
                if(highlight3 == 2){
                    wattron(inwin,A_REVERSE);
                }
                wprintw(inwin,">");
                wattroff(inwin,A_REVERSE);
            }else{
                mvwprintw(inwin,3,1," \t   \t ");
            }
    }else{
        mvwprintw(inwin,1,1,"                                                  ");
        mvwprintw(inwin,2,1,"                                                  ");
        mvwprintw(inwin,3,1,"                                                  ");
    }

    if(highlight1 == 3 && highlight2 == 0){
        mvwprintw(inwin,1,1,"<Send an amry to attack an enemy>");
        int num = 0;
        size_t i = 0;

        str = "Location: \t";
        mvwprintw(inwin,2+i,1,"%s",str);

        if(highlight4col == i && highlight4row == 0){
            wattron(inwin,A_REVERSE);
        }
        wprintw(inwin,"<");
        wattroff(inwin,A_REVERSE);
            
        wprintw(inwin," ");
        if(highlight4col == i && highlight4row == 1){
            wattron(inwin,A_REVERSE);
        }
        
        int x = map->getPosX(playerCount);
        int y = map->getPosY(playerCount);

        if(playerNum != -1){
            wprintw(inwin,"x:%d y:%d ID:%d",x,y,playerCount);
        }else{
            wprintw(inwin,"KILL SOMEONE YES!");
        }
        wattroff(inwin,A_REVERSE);

        if(!(x >= 10)){
            wprintw(inwin," ");
        }
        if(!(y >= 10)){
            wprintw(inwin," ");
        }
        if(!(playerCount >= 10)){
            wprintw(inwin," ");
        }
       
        if(highlight4col == i && highlight4row == 2){
            wattron(inwin,A_REVERSE);
        }
        wprintw(inwin,">");
        wattroff(inwin,A_REVERSE);

        for (i = 1; i < 4; i++){
            
            switch(i){
                case 1:
                str = "Archers:  \t";
                break;
                case 2:
                str = "Knights:  \t";
                break;
                case 3:
                str = "Defenders:\t";
                break;
            }

            mvwprintw(inwin,2+i,1,"%s",str);

            if(highlight4col == i && highlight4row == 0){
                wattron(inwin,A_REVERSE);
            }
            wprintw(inwin,"<");
            wattroff(inwin,A_REVERSE);
            
            wprintw(inwin,"\t");
            if(highlight4col == i && highlight4row == 1){
                wattron(inwin,A_REVERSE);
            }
            switch(i){
                case 1:
                num = archerC;
                break;
                case 2:
                num = knightC;
                break;
                case 3:
                num = defenderC;
                break;
            }
            wprintw(inwin,"%d",num);
            wattroff(inwin,A_REVERSE);
            wprintw(inwin,"\t ");
                
            if(highlight4col == i && highlight4row == 2){
                wattron(inwin,A_REVERSE);
            }
            wprintw(inwin,">");
            wattroff(inwin,A_REVERSE);
        }

        mvwprintw(inwin,6,1,"After selecting your troops, Select your objective:");

        if(highlight4col == 4 && highlight4row == 0){
            wattron(inwin,A_REVERSE);
        }
        mvwprintw(inwin,7,1,"Attack Fully");
        wattroff(inwin,A_REVERSE);

        wprintw(inwin,"  ");
        if(highlight4col == 4 && highlight4row == 1){
            wattron(inwin,A_REVERSE);
        }        
        wprintw(inwin,"Attack Safely");
        wattroff(inwin,A_REVERSE);

        wprintw(inwin,"  ");
        if(highlight4col == 4 && highlight4row == 2){
            wattron(inwin,A_REVERSE);
        }
        wprintw(inwin,"Attack Gainfully");
        wattroff(inwin,A_REVERSE);
        
    }else if(highlight1 == 3 && highlight2 == 1){
        mvwprintw(inwin,1,1,"<Once your done End your turn>       ");
        mvwprintw(inwin,4,1,"\t\t\t\t\t\t");
        mvwprintw(inwin,5,1,"\t\t\t\t\t\t\t");
        mvwprintw(inwin,6,1,"\t\t\t\t\t\t\t");
        mvwprintw(inwin,7,1,"\t\t\t\t\t\t\t");

    }else if(highlight1 == 3 && highlight2 == 2){
        mvwprintw(inwin,1,1,"<Kill yourself before they kill you>");
        mvwprintw(inwin,4,1,"\t\t\t\t\t\t");
        mvwprintw(inwin,5,1,"\t\t\t\t\t\t\t");
        mvwprintw(inwin,6,1,"\t\t\t\t\t\t\t");
        mvwprintw(inwin,7,1,"\t\t\t\t\t\t\t");
    }else{
        mvwprintw(inwin,4,1,"\t\t\t\t\t\t");
        mvwprintw(inwin,5,1,"\t\t\t\t\t\t\t");
        mvwprintw(inwin,6,1,"\t\t\t\t\t\t\t");
        mvwprintw(inwin,7,1,"\t\t\t\t\t\t\t");
    }

}

/**
 * @brief Updates the map window
 * 
 * @param str 
 * @param currentPlayer 
 * @param map 
 * @param AtkList 
 */
void ViewController::mapUpdate(const char * str,Player * currentPlayer,shared_ptr<Map> map,Attack AtkList){
    
    //Map Window
    mvwprintw(mapwin,0,1,"Map-Player:-%d",currentPlayer->getID());

    int x,y;
    x = map->getPosX(currentPlayer->getID());
    y = map->getPosY(currentPlayer->getID());

    wprintw(mapwin,"-x=%d-y=%d",x,y);

    
        
    //bug here dont know why 
    //just doesnt print for some reason
    //bug log | x=32 y=20 | x=53 y=18
        wattron(mapwin,A_BLINK);
        wattron(mapwin,A_REVERSE);
        mvwprintw(mapwin,y,x,"O");
        wattroff(mapwin,A_REVERSE);
        wattroff(mapwin,A_BLINK);

    if(highlight1 == 3 && highlight2 == 0){
        x = map->getPosX(playerCount);
        y = map->getPosY(playerCount);
        
        wattron(mapwin,A_BLINK);
        wattron(mapwin,A_REVERSE);
        mvwprintw(mapwin,y+1,x,"^");
        wattroff(mapwin,A_REVERSE);
        wattroff(mapwin,A_BLINK);
    }
    
    //print each attack going
    
    if(AtkList.getSize() != -1){
        for (size_t i = 0; i < (AtkList.getSize()+1); i++)
        {
            shared_ptr<Holder> h;
            h = AtkList.getAttack(i);

            //get positions of both players
            double x1 = (double)map->getPosX(h->whoDidIt), y1 = (double)map->getPosY(h->whoDidIt);
            double x2 = (double)map->getPosX(h->toWhom)  , y2 = (double)map->getPosY(h->toWhom);
            double x = ((x1-x2)/(h->Maxtime))*(h->Maxtime-h->time); //the piece to be added to towhom
            double y = ((y1-y2)/(h->Maxtime))*(h->Maxtime-h->time); 
            int posy = (int)y1-y;
            int posx = (int)x1-x;

            if(has_colors()){

                start_color();
                init_pair(1,COLOR_GREEN,COLOR_WHITE);
                wattron(mapwin,COLOR_PAIR(1));
            }

            wattron(mapwin,A_BLINK);
            mvwprintw(mapwin,posy,posx,"@");
            wattroff(mapwin,A_BLINK);

            if(has_colors()){
                wattroff(mapwin,COLOR_PAIR(1));
            }
        }
    }
    
    
}

/**
 * @brief A brief Update to change the constants in the map
 * 
 * @param map 
 */
void ViewController::updateMap(shared_ptr<Map> map){
    for (size_t y = 0; y < 16; y++)
    {
        wmove(mapwin,1+y,1);
        for (size_t x = 0; x < 65; x++)
        {
            wprintw(mapwin,"0");
       }
    }

    for (size_t i = 0; i < map->getMax(); i++)
    {
        wattron(mapwin,A_REVERSE);
        
        if(has_colors()){

            start_color();
            init_pair(1,COLOR_WHITE,COLOR_RED);

            if(map->IsPlayerDead(i)){
                wattron(mapwin,COLOR_PAIR(1));
            }
        }

        if(map->IsPlayerDead(i)){
            wattron(mapwin,A_DIM);
        }

        wmove(mapwin,map->getPosY(i),map->getPosX(i));
        wprintw(mapwin,"O");

        if(has_colors()){
            wattroff(mapwin,COLOR_PAIR(1));
        }

        wattroff(mapwin,A_DIM);
        wattroff(mapwin,A_REVERSE);
    }

    
    
}

/**
 * @brief Refresh each window
 * 
 */
void ViewController::refresh(){
    wrefresh(reswin);
    wrefresh(menuwin);
    wrefresh(submenuwin);
    wrefresh(inwin);
    wrefresh(mapwin);
}

/**
 * @brief Runs the Menu for when a player is under attack
 * 
 * @param Attack from the list that is currently being used
 * @return int 
 */
int ViewController::StartAttack(shared_ptr<Holder> Attack){
    WINDOW * win = VisualCreation(2,2,4,4);
    bool hasTheyNotSelectedSomething = true;
    int input = 0;
    highlight1= 0;

    mvwprintw(win,0,1,"YOU-ARE-UNDER-ATTACK!:");
    mvwprintw(win,3,1,"\t       <An attack force from Player:%d>",Attack->whoDidIt);
    mvwprintw(win,4,1,"\t      <There are %d Units attacking you>",Attack->troops[0]->getAmount()+Attack->troops[1]->getAmount()+Attack->troops[2]->getAmount());

    mvwprintw(win,6,1,"        > Select how you want to defend your base:");

    
    mvwprintw(win,9,1,"Gain Grid: DefendV\\Attack>   ");
    mvwprintw(win,10,1,"You:               Fully     ");
    mvwprintw(win,11,1,"                   Safely    ");
    mvwprintw(win,12,1,"                   Gainfully ");

    wattron(win,A_UNDERLINE);
    mvwprintw(win,9,31, " Fully\t Safely\t  Gainfully");
    mvwprintw(win,10,31,"|Even\t|Loss  \t |Win     |");
    mvwprintw(win,11,31,"|Win\t|Even  \t |Loss    |");
    mvwprintw(win,12,31,"|Loss\t|Win   \t |Even    |");
    wattroff(win,A_UNDERLINE);

    do{
        mvwprintw(win,7,1,"    ");
        if(highlight1 == 0){
            wattron(win,A_REVERSE);
        }
        wprintw(win,"> Defend Fully");
        wattroff(win,A_REVERSE);
        
        wprintw(win," ");
        if(highlight1 == 1){
            wattron(win,A_REVERSE);
        }
        wprintw(win,"> Defend Safely");
        wattroff(win,A_REVERSE);
        
        wprintw(win," ");
        if(highlight1 == 2){
            wattron(win,A_REVERSE);
        }
        wprintw(win,"> Defend Gainfully");
        wattroff(win,A_REVERSE);

        input = wgetch(win);
        
        switch(input){
            case KEY_LEFT:
                highlight1--;
                if(highlight1 == -1){
                    highlight1 = 0;
                }
            break;
            case KEY_RIGHT:
                highlight1++;
                if(highlight1 == 3){
                    highlight1 = 2;
                }
            break;
            case 10:
                hasTheyNotSelectedSomething = false;
            break;
        }

    }while(hasTheyNotSelectedSomething);

    wclear(win);
    wrefresh(win);
    return highlight1;
}

/**
 * @brief Returns the count
 * 
 * @return int 
 */
int ViewController::getCount(){
    return this->count;
}

/**
 * @brief returns the wood cost
 * 
 * @return int 
 */
int ViewController::getWoodCost(){
    return this->wood;
}

/**
 * @brief returns the stone cost
 * 
 * @return int 
 */
int ViewController::getStoneCost(){
    return this->stone;
}

/**
 * @brief returns the iron cost
 * 
 * @return int 
 */
int ViewController::getIronCost(){
    return this->iron;
}

/**
 * @brief returns food Cost
 * 
 * @return int 
 */
int ViewController::getFoodCost(){
    return this->food;
}

/**
 * @brief Returns the first choice from the menu
 * 
 * @return int 
 */
int ViewController::getChoice1(){
    return this->highlight1;
}

/**
 * @brief Returns the second choice from the submenu
 * 
 * @return int 
 */
int ViewController::getChoice2(){
    return this->highlight2;
}

/**
 * @brief returns the amount of Knights to attack with
 * 
 * @return int 
 */
int ViewController::getKnightCount(){
    return this->knightC;
}

/**
 * @brief returns the amount of Archers to attack with
 * 
 * @return int 
 */
int ViewController::getArcherCount(){
    return this->archerC;
}

/**
 * @brief returns the amount of Defenders to attack with
 * 
 * @return int 
 */
int ViewController::getDefenderCount(){
    return this->defenderC;
}

/**
 * @brief returns the Choice of attack from the attacker
 * 
 * @return int 
 */
int ViewController::getAttackChoice(){
    return this->highlight4row;
}

/**
 * @brief returns the Player to attack
 * 
 * @return int 
 */
int ViewController::getPlayerCount(){
    return this->playerCount;
}

/**
 * @brief Sends a pop up message and prints str
 * 
 * @param str 
 */
void ViewController::sendMsg(vector<string> str){
    WINDOW * win = VisualCreation(2,2,4,4);
    for (size_t i = 0; i < str.size(); i++)
    {   
        mvwprintw(win,1+i,1,str[i].c_str());
    }
    wgetch(win);

    wclear(win);
    wrefresh(win);
}
#endif // __VIEWCONTROLLER_H__