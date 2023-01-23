# CPS2004 Object-Oriented Programming Assignment

## Intro
---
The assignment given for Object-Oriented Programming was to design and build 2 games, 'Village War Game' and 'Minesweeper' and one is to be done  in Java and other in C++. 

> Village War Game was done in C++ while Minesweeper was done in Java. 


This report will be split into 3 paragraphs
> Intro
> - Specifications : specs needed to run all the games
> - How to play the games : a simple guide on how to play each the game
>
> Village War Game:
> - Design : how the game was designed
> - Special mentions : code that requires further explanation 
> - Ai : discussion on the state of Ai in the game 
>
> Minesweeper
> - Design : how the game was designed
> - Special mentions : code that requires further explanation
> - Input handling : how input where handled
>
> Bugs

---
### Specifications
---

C++ version **cpp 9.4.0** was used and **openjdk 17.0.4** for Java;
![](file:///C:/Users/wisam/Downloads/version.png)

For Village War Game in C++, the **ncurses Library** is required and their is no need to target it when compiling through CMakeLists since its added as a targeted library. Additionally its recommended to play the game in the default command-line window size, If words in the game are overlapping the intended size, increase the size of your window and restart the game. 

For Minesweeper the most recent java version is needed.

---
### How to play the games.
---

For **Village war game**, use the *up, down, left, and right keys* to move in the menu and the enter key to select. The game starts by prompting the user to enter the amount of players and Ai wanted in the given game. 

After that the game starts with the first player, starting with resources ; *wood, stone, iron, and food* the player has to build buildings to gain more resources as well as troop buildings to generate troops for combat. if the player has troops he can keep them to defend his base or use some or all to attack other villages the other player control. Once a player has finished his turn they can select the EndTurn option to go to the next turn.

All costs are shown in the *Input-menu* and even provide an explanations on buildings. All Stats and resources acquired by a given player are shown on their respective *Res-menu*. The *Map-menu* shows all players as well as if their dead or alive, it even shows the enemy troops on their way to their target. 


For **Minesweeper**, use your keyboard to input 2 number either in conjunction or after each other to select the x and y coordinates on the map.

---
## Village War Game

### Design
---

Their are 3 phases the Games goes through; 
> Preparation: Intended to see how many players and Ai are to be playing the game.
>
> Game Loop: Where as the game is played as intended and the basic loop is played out
>
> End Screen: where the final player or Ais are left resulting in the game to end and display a final game screen before ending execution
---

The code structure was separated into multiple classes. As seen in this UML Diagram 
![](file:///C:/Users/wisam/Downloads/Class%20Diagram%20VillageWarGame%20(1).svg)

The **Game** Object Hold The model aspect and interacts with all objects in play. One design pattern i tried to implement was to make all changing of variables, to be done through the **Game** object only hence allowing errors to be spotted more easily and to reduce the amount of collisions to occur. 

The **ViewController** Object was designed to separate the functionally of the the **Game** object from the Output and Input control. A ***Model-View-Controller design pattern*** implemented but since ncurses acts both as the view and controller this was combined into one.

The **Player** object holds all the information of a single player other then his position and and attacks declared.

The **Map** Object Holds all information related to the position of each player since once a player is dead they are deleted and no information is held after death.

The **Attack** Object holds a List of attacks that are going on to keep track of movement, the amount of troops and their stats when an attack is declared.

The classes *Archer, Knight, and Defender* are all bundled in a single file called **Troops_Types** and classes *WoodCutter, StoneMason, IronMiner, BattleTrainer, KnightPalace, and DefenderBarackes* are also bundled in a single class called **Buildings_Types**. 

**Troop_Types** and **Buildings_Types** both inherit from the object **Troops** and **Buildings** respectively and are meant to be used by **Player** which holds all the necessary information for each player.

No input error handling was needed in this game since all inputs are regulated through the enter, up, down, left and right keys such is no way for the user to input any wrong keys since only those keys are accepted. On the other hand if the player has selected something beyond what they can pay or do, the selected action is not done and no resources are taken.

---
### Special mentions

#### Map constructor
---

 The Map constructor was designed to space out each player away from each other with enough distance that only a few player are around one another but not too close or touching.
 
 Such was this designed Random separator:
 ```text
 1. Get the size of the map and divided it by the amount of player in game.
 (x/p | y/p)
 
 2. take a range of a square using the divided measurements to find the position of the player
 (x_am = 1 | y_am = Rand() -> inRange(0 - p))
 (p1 =  x_am * Rand() -> inRange(x - x/p) | p2 = y_am * Rand() -> inRange(y - y/p))
 
 3. After it increases x by 1 and does it again finding a y such that it is not on the y as the previous. 
 
 4. then it does the same thing as Step 2 and 3 for each player p.
 ```
 Here is a visual representation of the following pseudo code for a map of 7 players. the squares represent the ranges, the green squares chosen squares for Rand and their possibly respective positions the Rand can generate.
![](file:///C:/Users/wisam/Downloads/Class%20Diagram%20VillageWarGame%20(2).svg)

```c++
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

```
---
#### Attack Damage control
---

When An attack is declared, the troops have reached their final destination and the player in question has come his turn. An attack is then gone through. The way i designed the attack is that both player have a rock-paper-scissors style dispute where each option has its considered result.

> The Attacker can choose to:
> - Attack Fully > reckless but more damage 
> - Attack Safely > play it safe and to add pressure without losing to much
> - Attack Gainfully > to Loot as much as possible

> The Defender can choose to:
> - Defend Fully > reckless but less damage taken 
> - Defend Safely > play it safe without loosing to much
> - Defend Gainfully > to defend the loot as much as possible

Resulting in a Advantage , Disadvantage or Even playing field based on what was chosen.

After that The stats and piled up , Bonuses will be added based on strategy and a Balance system is taken place were the size and scale of troops will decide on how much damage and loop is taken. 

After calculations are done I reverse engineered the amount of losses, damage, Loot from players and transferred properly.

```c++

            //calulate each Total units Health/Damage & their Avrage Acc & the Health ratio
            int AtkUnitHealth = Attacker->troops[0]->getHealth()*Attacker->troops[0]->getAmount() + Attacker->troops[1]->getHealth()*Attacker->troops[1]->getAmount() + Attacker->troops[2]->getHealth()*Attacker->troops[2]->getAmount();
            int AtkUnitDamage = Attacker->troops[0]->getDamage()*Attacker->troops[0]->getAmount() + Attacker->troops[1]->getDamage()*Attacker->troops[1]->getAmount() + Attacker->troops[2]->getDamage()*Attacker->troops[2]->getAmount();
            int AtkUnitAcc = (Attacker->troops[0]->getAcc()+Attacker->troops[1]->getAcc()+Attacker->troops[2]->getAcc())/3;
            int AtkUnitHealthRatio = (Attacker->troops[0]->getHealth()+Attacker->troops[1]->getHealth()+Attacker->troops[2]->getHealth())/3;

            int DefUnitHealth = player[PlayersNumbers.Current].troops[0]->getHealth()*player[PlayersNumbers.Current].troops[0]->getAmount() + player[PlayersNumbers.Current].troops[1]->getHealth()*player[PlayersNumbers.Current].troops[1]->getAmount() + player[PlayersNumbers.Current].troops[2]->getHealth()*player[PlayersNumbers.Current].troops[2]->getAmount();
            int DefUnitDamage = player[PlayersNumbers.Current].troops[0]->getDamage()*player[PlayersNumbers.Current].troops[0]->getAmount() + player[PlayersNumbers.Current].troops[1]->getDamage()*player[PlayersNumbers.Current].troops[1]->getAmount() + player[PlayersNumbers.Current].troops[2]->getDamage()*player[PlayersNumbers.Current].troops[2]->getAmount();
            int DefUnitAcc = (player[PlayersNumbers.Current].troops[0]->getAcc()+player[PlayersNumbers.Current].troops[1]->getAcc()+player[PlayersNumbers.Current].troops[2]->getAcc())/3;
            int DefUnitHealthRatio =(player[PlayersNumbers.Current].troops[0]->getHealth()+player[PlayersNumbers.Current].troops[1]->getHealth()+player[PlayersNumbers.Current].troops[2]->getHealth())/3;

            //creating each bonus variable
            double AtkBonusSurvivalRate = 0;
            double AtkBonusLootRate = 0;
            double AtkBonusDamageRate = 0;
            double DefBonusSurvivalRate = 0;
            double DefBonusDamageRate = 0;

            //calulate each bonus seperatly
            switch(choiceDefend){
                case 0:
                    if(choiceAttack == 0){
                        AtkBonusSurvivalRate = 0;
                        AtkBonusLootRate = 0;
                        AtkBonusDamageRate = 0;
                        DefBonusSurvivalRate = 5/10;
                        DefBonusDamageRate = 2/10;                                   
                    }else if(choiceAttack == 1){
                        AtkBonusSurvivalRate = 3/10;
                        AtkBonusLootRate = 2/10;
                        AtkBonusDamageRate = 4/10;
                        DefBonusSurvivalRate = 0;
                        DefBonusDamageRate = 0; 
                    }else if(choiceAttack == 2){
                        AtkBonusSurvivalRate = 2/10;
                        AtkBonusLootRate = 5/10;
                        AtkBonusDamageRate = 0;
                        DefBonusSurvivalRate = 5/10;
                        DefBonusDamageRate = 2/10; 
                    }
                break;
                case 1:
                    if(choiceAttack == 0){
                        AtkBonusSurvivalRate = 0;
                        AtkBonusLootRate = 0;
                        AtkBonusDamageRate = 0;
                        DefBonusSurvivalRate = 4/10;
                        DefBonusDamageRate = 3/10; 
                    }else if(choiceAttack == 1){
                        AtkBonusSurvivalRate = 0;
                        AtkBonusLootRate = 0;
                        AtkBonusDamageRate = 0;
                        DefBonusSurvivalRate = 5/10;
                        DefBonusDamageRate = 2/10; 
                    }else if(choiceAttack == 2){
                        AtkBonusSurvivalRate = 3/10;
                        AtkBonusLootRate = 5/10;
                        AtkBonusDamageRate = 0;
                        DefBonusSurvivalRate = 3/10;
                        DefBonusDamageRate = 0; 
                    }
                break;
                case 2:
                    if(choiceAttack == 0){
                        AtkBonusSurvivalRate = 2/10;
                        AtkBonusLootRate = 1/10;
                        AtkBonusDamageRate = 3/10;
                        DefBonusSurvivalRate = 2/10;
                        DefBonusDamageRate = 0; 
                    }else if(choiceAttack == 1){
                        AtkBonusSurvivalRate = 2/10;
                        AtkBonusLootRate = 1/10;
                        AtkBonusDamageRate = 2/10;
                        DefBonusSurvivalRate = 2/10;
                        DefBonusDamageRate = 2/10; 
                    }else if(choiceAttack == 2){
                        AtkBonusSurvivalRate = 0;
                        AtkBonusLootRate = 1/10;
                        AtkBonusDamageRate = 0;
                        DefBonusSurvivalRate = 5/10;
                        DefBonusDamageRate = 2/10; 
                    }
                break;
            }

            // calulate the total damage dealt + bonus
            double AtkTrueDamage = AtkUnitDamage*AtkUnitAcc + (rand()%(int)((AtkUnitDamage*AtkBonusDamageRate)+1));
            double DefTrueDamage = DefUnitDamage*DefUnitAcc + (rand()%(int)((DefUnitDamage*DefBonusDamageRate)+1));
            
            //Calculate the difference In damage
            double AtkDifferenceOfDamage = AtkTrueDamage/(AtkTrueDamage+DefTrueDamage+1);// the one is there just incase /0 occurs (it shouldnt but just incase)
            double DefDifferenceOfDamage = 1 - AtkDifferenceOfDamage;

            //calculate the damage done based on the difference and the bonus
            double AtkCalculatedHealth = AtkUnitHealth - (AtkUnitHealth*DefDifferenceOfDamage)*(1-AtkBonusSurvivalRate);
            double DefCalculatedHealth = DefUnitHealth - (DefUnitHealth*AtkDifferenceOfDamage)*(1-DefBonusSurvivalRate);

            //reverse calulate an estimate of the units left based on the health
            double AtkUnitsLeft = AtkCalculatedHealth/AtkUnitHealthRatio;
            double DefUnitsLeft = DefCalculatedHealth/DefUnitHealthRatio;

            //round it so its has no decimal point  
            AtkUnitsLeft = round(AtkUnitsLeft);
            DefUnitsLeft = round(DefUnitsLeft);

            //calulate the amount of stolen material
            int AtkUnitCarry =(Attacker->troops[0]->getCarryingCapacity() + Attacker->troops[1]->getCarryingCapacity() + Attacker->troops[2]->getCarryingCapacity())/3;
            double AtkLoot = (AtkUnitsLeft*AtkUnitCarry) + (AtkUnitsLeft*AtkUnitCarry)*(AtkBonusLootRate);

            //round it so it has no decimal point
            AtkLoot = round(AtkLoot);

```
---
### Ai
---

The Ai was not fully implemented into the Village war game. The current state that the Ai is currently is 'Ghost'.  
  
What was supposed to be implemented in 3 stages has not fully happened. I am going to explain the 4 stages of which the Ai was going to be implemented as.

> Stage 1 : Ghost
>
> The Ghost state is a state such that the Map recognized the Ai as a player and prints it on the map. the Player cannot interact with the Ghost Ai and neither can it.

> Stage 2: Target Dummy
>
> The Target Dummy state is such that Player are able to interact with the Ai by attacking it and it can also die but The Target Dummy Ai has no functionality and cannot attack, build or upgrade anything it has.

> Stage 3: Basic Ai
>
> The Basic Ai state is such that the Ai is able to do 2 things follow a set of basic instructions so it doesn't soft-lock itself and Randomly upgrade or build, buildings or troops. Not declaration of attacks can be done by the Ai.

> Stage 4: Behavior
>
> The Behavior stage is meant to provide a set of states where the Ai can be at Aggressive, Revenge and Safety and is able to declare attacks in such a way to provide a unique experience. 

The Ai was going to be a separate vector of the type Player. The Id of The Ai was going to be greater then 10 resulting in a way to differentiate between Ai and player since player can only have a id from 0-9. 

To be able to Attack the AI The Attack Object must be able to give a hint that the Attacker or the Defender is a Player or Ai. Additionally The Ai must also have a turn was going to be after each player has played the loop breaks for 1 turn which can allow the Ai to have a turn after all the players turn.

The behaviors would have Been Aggressive in the sense prioritizes Attack, Revenge is more of a passive play style where if the Ai gets attacked, they will attack back, and Safety is priority over putting pressure over the each player.

---
## Minesweeper 

### Design
---

The Minesweeper Game was designed into 2 classes, 
> The **Board** class designed with:
> - A Constructor that creates the board.
> - A function that add bombs to the board.
> - A function that prints the board.
> - A function that takes the players input and applies the game rules.
> - A function to declare the end of the game.

> The **Main** class was designed to:
> 
> Handle inputs from the player, 
>
> Prompt the player to provide a proper Integer, 
>
> Handle outputs from the Board class and act according to it. (such as *same input*, *selecting a bomb* or *end of the game*)

The way the game operates is quite simple. Their are 2 grids one represents all the numbers and bombs the other represents if it should show or not, thus why one is of type integer and the other is of type Boolean. Once the AddBombs() function is called it will add bombs to the integer grid represented as -1 while additionally Increment all squares by one around it. Once a square is chosen to be revealed it will go though the general process required to perform the game of minesweeper. 

---
### Special mentions

#### Select On Board function
---

The select on board function is a very interesting function because it works recursively to reveal all the blocks.
```text
checkOnBoard():: (Int -> Int) -> Int    -- 0 = Fail , 1 = Success, -1 = Lost Game
checkOnBoard(x,y)
    | !(inRangeOfBoard(x) || inRangeOfBoard(y)) = 0  
    -- If out of range of the board return failure
    | PosInShowing(x,y) == True = 0     
    -- If position has already been show return failure
    | IsBomb(x,y) == True = -1          
    -- If position is bomb return lost game
    | PosInGrid(x,y) > 0 = do show(x,y) then return 1
    -- If position is a number greater then 0 show it and return success
    | PosInGrid(x,y) == 0 = do show(x,y) then checkOnBoard(x(+1&&-1),y(+1&&-1)) then return 1
    -- If position is 0 do checkOnBoard on every x & y position around it then return success
    | otherwise = 0
    -- If all else fails then i have no idea.
```
As you can see in this Haskell demo pseudo code variant the checkOnBoard function will check for every possible check as a base case in this recursive function and only recuse if it is a 0. since the 0 might hit a bomb by accident, the bomb check doesn't reveal the bomb.

```java
    public int selectOnBoard(int x,int y){

        boolean Test;
        Test = x >= 0 && y >= 0 && x < board.length && y < board[0].length; //out of bounce
        if(Test != true){
            return 0; //unsucessfull if out of bounce
        }

        if(showing[x][y] == true){
            return 0; //unsuccessfull if selected an already showing selection
        }

        if(board[x][y] == -1){
            return -1; //failer case if selected a bomb
        }

        if(board[x][y] > 0){
            showing[x][y] = true;
            return 1;
        }

        if(board[x][y] == 0){
            showing[x][y] = true;
            selectOnBoard(x+1, y); //recursive checking
            selectOnBoard(x, y+1);
            selectOnBoard(x+1, y+1);
            selectOnBoard(x-1, y);
            selectOnBoard(x, y-1);
            selectOnBoard(x-1, y-1);
            return 1;
        }

        return 0; // unknown unsuccessful case 
    }
```
---
### Input handling 
---

In main the input handler for anything other then integers are not accepted and anything beyond the boundaries aren't either.
```java
try{
    repeat = false;
    num1 = scanner.nextInt();
    num2 = scanner.nextInt();
    } catch(Exception e){
        scanner.next();
        repeat = true;
        System.out.println("You entered the wrong inputs");
        }
    }
```

---
## Bug Report
---

Their were many bugs but all have been fixed. If their are any more bugs I have not seen them or known about them.

But their is a potential bug in Village War Game, in the Attack class. It might be possible that if a player has died and an attack hasn't gone through its possible it might crash the game.
