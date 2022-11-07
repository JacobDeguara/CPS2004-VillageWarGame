#include <ncurses.h>
#include <vector>
#include <string>
#include <array>

#include "Player.hpp"

#ifndef __VIEWCONTROLLER_H__
#define __VIEWCONTROLLER_H__

using std::string;
using std::vector;
using std::array;

/*Brief: ViewController class deals with The visual & control aspects
* Extra: this class also gives the costs & action to be taken by the game 
*/
class ViewController
{
private:
    WINDOW * menuwin;
    WINDOW * submenuwin;
    WINDOW * reswin;
    WINDOW * inwin;
    WINDOW * mapwin;

    struct{
        string menuName;
        vector<string>submenuNames;
        int submenuSize;
    }MenuNames[4];

    int highlight1 = 0;
    int highlight2 =-1;
    int highlight3 =-1;
    int highlight4row =-1;
    int highlight4col =-1;
    int count = 1;
    int archerC=0,knightC=0,defenderC=0;
    int wood=0,stone=0,iron=0,food=0;

    void choicesCreation();
    void costCreationTroop(int food,Player * p); 
    void costCreationBuildings(int wood,int stone,int iron,int food,Player * p);

public:
    ViewController(WINDOW * menuWindow, WINDOW * submenuWindow, WINDOW * resourceWindow,WINDOW * inputWindow,WINDOW * mapWindow);
    ~ViewController();

    int getMenuMV(); 
    int getSubMenuMV(); 
    int getInputMenuMV(); 
    void prepSubMenu();
    void prepInputMenu(); 
    void resetMenu(); 

    int getCount();
    int getWoodCost();
    int getStoneCost();
    int getIronCost();
    int getFoodCost();
    int getChoice1();
    int getChoice2();

    void update(Player * p,int playerNum);
    void refresh();
    
};

//Gets the input from menuWin and changes highlight1
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

//Gets the input from submenuWin and changes highlight2
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
        return 0; //choice selected
    }
    return -2; // -1 is default loop for sub menu
}

//Gets the input from inputWin and changes highlight3 accordingly
int ViewController::getInputMenuMV(){
    if(highlight1 != 3){ // if choice1 = 1-3 
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

    if((highlight1 == 3) && (highlight2 == 0)){
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
                if(highlight4col == 4){
                    highlight4col = 3;
                }
                break;
            default:
                break;
        }

        if(choice3 == 10){
            if(highlight4row==0){
                switch(highlight4col){
                    case 0:
                        if(count != 1){
                            count--;
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
            }else if(highlight4row==2){
                switch(highlight4col){
                    case 0:
                        count++;
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
            }else{
                return 0; //choice selected
            }
        }

        return -3;
    }
    
    highlight3 = -1;
    return -2;
}

//prepares the change from menuWin to submenuWin
void ViewController::prepSubMenu(){
    highlight2 = 0;
}

//prepares the change from submenuWin to inputWin
void ViewController::prepInputMenu(){
    count = 1;
    archerC =0;
    knightC = 0;
    defenderC = 0;
    highlight3 =0;
    highlight4col = 0;
    highlight4row = 0;

}

//resets the menu back to menuWin
void ViewController::resetMenu(){
    highlight1 = 0;
    highlight2 = -1;
    highlight3 = -1;
    count = 1;
}

//Gives The stuct MenuNames all the Strings for the menus
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

void ViewController::costCreationTroop(int food,Player * p){
    this->wood = 0;
    this->stone= 0;
    this->iron = 0;
    this->food = food * count + 1*(p->troops[highlight2]->getLevel());
}

void ViewController::costCreationBuildings(int wood,int stone,int iron,int food,Player * p){
    this->wood = wood *count * (p->buildings[highlight2]->getamount()+p->buildings[highlight2]->getLevel());
    this->stone= stone*count * (p->buildings[highlight2]->getamount()+p->buildings[highlight2]->getLevel());
    this->iron = iron *count * (p->buildings[highlight2]->getamount()+p->buildings[highlight2]->getLevel());
    this->food = food *count * (p->buildings[highlight2]->getamount()+p->buildings[highlight2]->getLevel());
}

//Connects all the windows and preps the Menu
ViewController::ViewController(WINDOW * menuWindow, WINDOW * submenuWindow, WINDOW * resourceWindow,WINDOW * inputWindow,WINDOW * mapWindow)
{
    menuwin = menuWindow;
    submenuwin = submenuWindow;
    reswin = resourceWindow;
    inwin = inputWindow;
    mapwin = mapWindow;

    choicesCreation();
}

ViewController::~ViewController()
{

}

//Returns the count
int ViewController::getCount()
{
    return this->count;
}

//Returns the wood cost
int ViewController::getWoodCost()
{
    return this->wood;
}

//Returns the stone cost
int ViewController::getStoneCost()
{
    return this->stone;
}

//Returns the iron cost
int ViewController::getIronCost()
{
    return this->iron;
}

//Returns the food cost
int ViewController::getFoodCost()
{
    return this->food;
}

//Returns the Choice from the first menu
int ViewController::getChoice1()
{
    return this->highlight1;
}

//returns the Choice from the second menu
int ViewController::getChoice2()
{
    return this->highlight2;
}

//Re-Writes(updates) the View cash to change any changes
void ViewController::update(Player * p,int playerNum){
    const char * str;

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

    //Resource Window
    mvwprintw(reswin,0,1,"Resources");
    int j=1,jHold;

    //print basic Resources (wood,stone,iron,food)
    for(int i = 0; i < p->getResSize(); i++)
    {   
        int num;
        switch(i){
            case 0:
                str = "Wood : ";
                num = p->getWood();
                break;
            case 1:
                str = "Stone: ";
                num = p->getStone();
                break;
            case 2:
                str = "Iron : ";
                num = p->getIron();
                break;
            case 3:
                str = "Food : ";
                num = p->getFood();
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
        
        int amount,level;
        str = MenuNames[0].submenuNames[i].data();
        amount = p->buildings[i]->getamount();
        level = p->buildings[i]->getLevel();
        if(amount != 0){
            mvwprintw(reswin,j,1,"%s: Qty:%d Lvl:%d Gen:%d",str,amount,level,p->getGen(i));
            j++;
        }
    }
    if((jHold+1) != j){ // we are checking if we have any Buildings shown
        mvwprintw(reswin,jHold,1,"Buildings:                      ");
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
        amount = p->troops[i]->getAmount();
        level = p->troops[i]->getLevel();
        powR = p->troops[i]->getPowerRating();
        if(amount != 0){
            mvwprintw(reswin,j,1,"%s: Qty:%d Lvl:%d Power:%d",str,amount,level,powR);
            j++;
        }
    }
    if((jHold+1) != j){ // we are checking if we have any Troops shown
        mvwprintw(reswin,jHold,1,"Troops:                      ");
    }else{
        j--;
    }

    //clear the resaurces slices not used
    for(int i = j;i < 17;i++){
        mvwprintw(reswin,i,1,"                                             ");
    }

    //Input Window
    mvwprintw(inwin,0,1,"Inputs-Menu");
    if((highlight1 != 3)&&(highlight2 != -1)){
        switch (highlight2){
        case 0:
            switch (highlight1){
            case 0:
                costCreationBuildings(2,2,0,0,p);                                                                                 //create wood cutter
                mvwprintw(inwin,1,1,"Create more Wood Cutters ");
            break;
            case 1:
                costCreationBuildings(2,2,2,1,p);
                mvwprintw(inwin,1,1,"Upgrading the Wood Cutter");
            break;
            case 2:
                costCreationTroop(1,p);
                mvwprintw(inwin,1,1,"Train the Archers        ");
            break;
            default:
                costCreationBuildings(0,0,0,0,p);
                break;
            }
            mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
        break;

        case 1:
            switch (highlight1){
            case 0:
                costCreationBuildings(2,0,2,0,p);
                mvwprintw(inwin,1,1,"Create more Stone Miners ");
            break;
            case 1:
                costCreationBuildings(3,0,3,1,p);
                mvwprintw(inwin,1,1,"Upgrades the Stone Miner ");
            break;
            case 2:
                costCreationTroop(1,p);
                mvwprintw(inwin,1,1,"Train the Knights        ");
            break;
            default:
                costCreationBuildings(0,0,0,0,p);
                break;
            }
            mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
        break;

        case 2:
            switch (highlight1){
            case 0:
                costCreationBuildings(3,2,0,0,p);                                                                                 //create iron miner
                mvwprintw(inwin,1,1,"Create more Iron Miners  ");
            break;
            case 1:
                costCreationBuildings(3,3,0,1,p);
                mvwprintw(inwin,1,1,"Upgrades the Iron Miner  ");
            break;
            case 2:
                costCreationTroop(1,p);
                mvwprintw(inwin,1,1,"Train the Defenders     ");
            break;
            default:
                costCreationBuildings(0,0,0,0,p);
                break;
            }
            mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
        break;
        case 3:
            if(highlight1==0){                                                                                   //create Battle Trainer
                costCreationBuildings(2,2,2,0,p);
                mvwprintw(inwin,1,1,"Create more Food stalls  ");
            }else if(highlight1==1){                                                                             // Upgrade Battle Trainer
                costCreationBuildings(2,2,2,0,p);
                mvwprintw(inwin,1,1,"Upgrades the Battle Tower");
            }
            mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
        break;

        case 4:
            if(highlight1==0){                                                                                   //create Archer place
                costCreationBuildings(2,2,0,2,p);
                mvwprintw(inwin,1,1,"Creates more Archers     "); 
            }else if(highlight1==1){  
                costCreationBuildings(2,0,2,1,p);                                                                           // Upgrade Archer place
                mvwprintw(inwin,1,1,"Upgrade The Range        ");
            }
            mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
        break;

        case 5:
            if(highlight1==0){  
                costCreationBuildings(1,1,1,2,p);                                                                                 //create Knight place
                mvwprintw(inwin,1,1,"Creates more Knights     ");
            }else if(highlight1==1){                                                                             // Upgrade Knight place
                costCreationBuildings(1,1,1,1,p);
                mvwprintw(inwin,1,1,"Upgrades The Palace      ");
            }
            mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
        break;

        case 6:
            if(highlight1==0){   
                costCreationBuildings(1,1,1,3,p);                                                                                //create Defender place
                mvwprintw(inwin,1,1,"Creates more Defenders   ");
            }else if(highlight1==1){                                                                             // Upgrade Defender place
                costCreationBuildings(1,2,1,1,p);
                mvwprintw(inwin,1,1,"Upgrades the Barackes    ");
            }
            mvwprintw(inwin,2,1,"Wood: %d , Stone: %d , Iron: %d , Food %d         ",wood,stone,iron,food);
        break;

        default:
            
            break;
        }

        if(highlight2 != -1){
                if(highlight3 == 0){
                    wattron(inwin,A_REVERSE);
                }
                mvwprintw(inwin,3,1,"<");
                wattroff(inwin,A_REVERSE);
                
                if(highlight3 == 1){
                    wattron(inwin,A_REVERSE);
                }
                wprintw(inwin,"\t%d\t",count);
                wattroff(inwin,A_REVERSE);
                
                if(highlight3 == 2){
                    wattron(inwin,A_REVERSE);
                }
                wprintw(inwin,">");
                wattroff(inwin,A_REVERSE);
            }else{
                mvwprintw(inwin,3,1," \t   \t ");
            }
    }else{
        mvwprintw(inwin,1,1,"                         ");
        mvwprintw(inwin,2,1,"                                                  ");
        mvwprintw(inwin,3,1," \t   \t ");
    }

    /* <    Locaction   > 1,2,3 clm 0
         * <    Archer      > 1,2,3 clm 1
         * <    Knight      > 1,2,3 clm 2
         * <    Defender    > 1,2,3 clm 3
         */
    if(highlight1 == 3 && highlight2 == 0){
        for (size_t i = 0; i < 4; i++){
            int num = 0;
            if(highlight4col == i && highlight4row == 0){
                wattron(inwin,A_REVERSE);
            }
            mvwprintw(inwin,1+i,1,"<");
            wattroff(inwin,A_REVERSE);
            
            if(highlight4col == i && highlight4row == 1){
                wattron(inwin,A_REVERSE);
            }
            switch(i){
                case 0:
                num = count;
                break;
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
            wprintw(inwin,"\t%d\t",num);
            wattroff(inwin,A_REVERSE);
                
            if(highlight4col == i && highlight4row == 2){
                wattron(inwin,A_REVERSE);
            }
            wprintw(inwin,">");
            wattroff(inwin,A_REVERSE);
        }
        
    }else{
        mvwprintw(inwin,4,1," \t   \t ");
    }
    //Map Window
    mvwprintw(mapwin,0,1,"Map");

}

//Refreshes the View for the Viewer to see then change
void ViewController::refresh(){
    wrefresh(reswin);
    wrefresh(menuwin);
    wrefresh(submenuwin);
    wrefresh(inwin);
    wrefresh(mapwin);
}

#endif // __VIEWCONTROLLER_H__