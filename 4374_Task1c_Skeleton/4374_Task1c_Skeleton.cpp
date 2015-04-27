//SKELETON PROGRAM
//---------------------------------
//include libraries
//include standard libraries
#include <iostream >						//for output and input: cin >> and cout <<
#include <iomanip>							//for formatted output in 'cout'
#include <conio.h>							//for getch()
#include <string>							//for string
#include <vector>							//for vectors
#include <fstream>							//for ofstream & ifstream


//include our own libraries
#include "RandomUtils.h"					//for Seed, Random
#include "ConsoleUtils.h"					//for Clrscr, Gotoxy, etc.
#include "TimeUtils.h"						//for GetTime, GetDate
using namespace std;
//---------------------------------
//define constants
//---------------------------------
//define global constants
//defining the size of the grid
const int SIZEY(12);						//vertical dimension
const int SIZEX(20);						//horizontal dimension
const string EXTENSION = ".scr";			//file extension for the playerscore
//defining symbols used for display of the grid and content
const char SPOT('@');						//spot
const char TUNNEL(' ');						//open space
const char WALL('#');						//border
const char HOLE('O');						//hole
const char PILL('*');						//pill
const char ZOMBIE('Z');						//Zombie
//defining the command letters to move spot on the maze
const int  UP(72);							//up arrow
const int  DOWN(80);						//down arrow
const int  RIGHT(77);						//right arrow
const int  LEFT(75);						//left arrow
//defining the other command letters
const char PLAY('P');						//play the game
const char QUIT('Q');						//end the game
const char INFO('I');						//show information
const char BACK('\r');						//go back
const char EXTERMINATE('X');				//kill all zombies
const char FREEZE('F');						//freeze all zombies
const char EAT('E');						//eat all the pills

//data structure to store data for a grid item
struct Item
{
	const char symbol;						//symbol on grid
	int x, y;								//coordinates
	bool isBeingRendered;					//is the Item being renedered
};
//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------

int main()
{
	//function declarations (prototypes)
	int  getKeyPress();
	bool wantToQuit(int k);
	void playGame(string playerName);
	void displayMenu();
	void displayInfo();
	void displayExit();
	string getPlayerName();

	//local variable declarations 
	bool running = true;									//to exit the game
	bool back = false;										//to get back to the menu
	int key(' ');											//create key to store keyboard events
	string playerName;
	//menu
	do {
		displayMenu();										//display the main menu
		key = getKeyPress();								//read in next keyboard event
		//game
		if (toupper(key) == PLAY){
			playerName = getPlayerName();
			playGame(playerName);
		}//quit
		if (wantToQuit(key))
		{
			displayExit();									//display the exit message
			system("pause");
			running = false;								//quit the game
		}//
		//info
		if (toupper(key) == INFO)
		{
			do {
				displayInfo();								//display the info screen
				key = getKeyPress();						//read in next keyboard event
				if (key == BACK)
					back = true;
			} while (!back);
		}
	} while (running);

	return 0;
} //end main

void playGame(string playerName)
{
	//function declarations (prototypes)
	void initialiseGame(char grid[][SIZEX], Item& spot, vector<Item>& holes, vector<Item>& pills, vector<Item>& zombies);
	bool wantToQuit(int k);
	bool isArrowKey(int k);
	bool outOfLives(int lives);
	bool outOfZombies(vector<Item> zombies);
	bool outOfPills(int pillsRemaining);
	int  getKeyPress();
	void updateGame(char g[][SIZEX], Item& sp, vector<Item> holes, int k, int& lives, string& mess, vector<Item>& pills, int& pillsRemaining, vector<Item>& zombies, bool frozen);
	void renderGame(const char g[][SIZEX], string mess, int lives, string playerName, int highScore);
	void endProgram(int lives, int key, vector<Item> zombies, int pillsRemaining ,string name, int highscore);
	int getPlayerScore(string name);
	void cheats(int& lives, vector<Item>& zombies, vector<Item>& pills, int key, bool& frozen);


	//local variable declarations 
	char grid[SIZEY][SIZEX];								//grid for display
	int lives(3);											//The number of lives spot has
	int pillsRemaining(5);									//The number of pills still being shown
	const int highscore = getPlayerScore(playerName);					//get the players highest score
	Item spot = { SPOT };									//Spot's symbol and position (0, 0) 
	Item hole = { HOLE };									//Hole's symbol and position (0, 0)
	bool frozen(false);

	Item pill = { PILL };									//Pill's symbol and position (0, 0)
	vector <Item> holes(12, hole);							//Creates a vector of holes, with each element being initialised as hole 
	vector <Item> pills(5, pill);							//Creates a vector of pills, with each element being initialised as pills
	Item zombie = { ZOMBIE };								//Zombies symbol and position (0, 0)
	vector<Item> zombies(4, zombie);				     	//Initialise a vector of zombies, each element will be initialised as zombie

	string message("         LET'S START...          ");	//current message to player

	bool running = true;

	//action...

	initialiseGame(grid, spot, holes, pills, zombies);		//initialise grid (incl. walls and spot)

	int key(' ');											//create key to store keyboard events
	do {
		renderGame(grid, message, lives, playerName, highscore);					//render game state on screen
		message = "                                 ";		//reset message
		key = getKeyPress();								//read in next keyboard event
		if (isArrowKey(key))
			updateGame(grid, spot, holes, key, lives, message, pills, pillsRemaining, zombies, frozen);
		else
			message = "          INVALID KEY!           ";	//set 'Invalid key' message
		cheats(lives, zombies, pills, key, frozen);			//see if there are cheats
		if (wantToQuit(key))								//if player wants to quit
			running = false;
		if (outOfLives(lives))								//if player is out of lives
			running = false;
		if (outOfZombies(zombies) && outOfPills(pillsRemaining))							//if all the zombies are dead
			running = false;
	} while (running);

	endProgram(lives, key, zombies, pillsRemaining, playerName, highscore);
}

void updateGame(char grid[][SIZEX], Item& spot, vector<Item> holes, int key, int& lives, string& message, vector<Item>& pills, int& pillsRemaining, vector<Item>& zombies, bool frozen)
{ //updateGame state
	void updateSpotCoordinates(const char g[][SIZEX], Item& spot, int key, int& lives, string& mess, vector<Item>& pills, int& pillsRemaining);
	void updateZombieCoordinates(const char g[][SIZEX], vector<Item>& zombies, Item spot, int& lives, string& message, bool frozen);
	void updateGrid(char g[][SIZEX], Item spot, vector<Item> holes, vector<Item> pills, vector<Item> zombies);

	updateSpotCoordinates(grid, spot, key, lives, message, pills, pillsRemaining);	//update according to key
	updateZombieCoordinates(grid, zombies, spot, lives, message, frozen);	//update the zombie position based on spots location
	updateGrid(grid, spot, holes, pills, zombies);							//update grid information
}
//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------
void initialiseGame(char grid[][SIZEX], Item& spot, vector<Item>& holes, vector<Item>& pills, vector<Item>& zombies)
{ //initialise grid and place spot in middle
	void setGrid(char[][SIZEX]);
	void setSpotInitialCoordinates(Item& spot);
	void setHoleInitialCoordinates(vector<Item>& holes, char gr[][SIZEX]);
	void setPillsInitialCoordinates(vector<Item>& pills, char gr[][SIZEX]);
	void placeSpot(char gr[][SIZEX], Item spot);
	void placeHoles(char gr[][SIZEX], vector<Item> holes);
	void placePills(char gr[][SIZEX], vector<Item> pills);

	void setZombieInitialCoordinates(vector<Item>& zombies);
	void placeSpot(char gr[][SIZEX], Item spot);
	void placeHoles(char gr[][SIZEX], vector<Item> holes);
	void placeZombies(char gr[][SIZEX], vector<Item> zombies);

	Seed();													//seed random number generator

	//do while?
	setGrid(grid);											//reset empty grid

	setZombieInitialCoordinates(zombies);					//setup the positions of each zombie
	placeZombies(grid, zombies);							//place the zombies in the grid

	setSpotInitialCoordinates(spot);						//initialise spot position
	placeSpot(grid, spot);									//set spot in grid

	setHoleInitialCoordinates(holes, grid);					//intiialise holes position
	placeHoles(grid, holes);								//set holes in grid

	setPillsInitialCoordinates(pills, grid);			//initialise pills position
	placePills(grid, pills);								//set pills in grid
} //end of initialiseGame

void setSpotInitialCoordinates(Item& spot)
{ //set spot coordinates inside the grid at random at beginning of game
	spot.y = Random(SIZEY - 2);								//vertical coordinate in range [1..(SIZEY - 2)]
	spot.x = Random(SIZEX - 2);								//horizontal coordinate in range [1..(SIZEX - 2)]
} //end of setSpotInitialCoordinates

void setHoleInitialCoordinates(vector<Item>& holes, char gr[][SIZEX])
{ //set hole coordinates inside the grid at random at beginning of game
	bool taken;
	for (int i = 0; i < holes.size(); ++i)
	{
		taken = false;
		holes[i].y = Random(SIZEY - 2);						//vertical coordinate in range [1..(SIZEY - 2)]
		holes[i].x = Random(SIZEX - 2);						//horizontal coordinate in range [1..(SIZEX - 2)]

		if (gr[holes[i].y][holes[i].x] != TUNNEL)
			taken = true;

		for (int h = 0; h < i; ++h)
			if (holes[i].y == holes[h].y && holes[i].x == holes[h].x)
				taken = true;

		if (taken == true)
			i--;
	}
} //end of setHoleInitialCoordinates

void setPillsInitialCoordinates(vector<Item>& pills, char gr[][SIZEX])
{ //set the pills coordinates inside the grid randomly at the beginning of a game, checking theyre not on a taken space)
	bool taken;
	for (int i = 0; i < pills.size(); ++i)
	{
		taken = false;
		pills[i].y = Random(SIZEY - 2);						//vertical coordinate in range [1..(SIZEY - 2)]
		pills[i].x = Random(SIZEX - 2);						//horizontal coordinate in range [1..(SIZEX - 2)]
		pills[i].isBeingRendered = true;					//set the pills to be rendered

		if (gr[pills[i].y][pills[i].x] != TUNNEL)
			taken = true;

		for (int p = 0; p < i; ++p)
			if (pills[i].y == pills[p].y && pills[i].x == pills[p].x)
				taken = true;

		if (taken == true)
			i--;
	}
}//end of setPillsInitialCoordinates 
void setZombieInitialCoordinates(vector<Item>& zombies)
{
	//Will set up all 4 zombies to spawn in a corner
	void resetZombiePosition(vector<Item>& zombies, int arrayIndex);

	for (int i = 0; i < zombies.size(); ++i)
	{
		zombies[i].isBeingRendered = true;
		resetZombiePosition(zombies, i);
	}
}//end of setZombieInitialCoordinates

void setGrid(char grid[][SIZEX])
{ //reset the empty grid configuration
	for (int row(0); row < SIZEY; ++row)				//for each column
	{
		for (int col(0); col < SIZEX; ++col)			//for each col
		{
			if ((row == 0) || (row == SIZEY - 1))		//top and bottom walls
				grid[row][col] = WALL;					//draw a wall symbol
			else
				if ((col == 0) || (col == SIZEX - 1))	//left and right walls
					grid[row][col] = WALL;				//draw a wall symbol
				else
					grid[row][col] = TUNNEL;			//draw a space
		} //end of row-loop
	} //end of col-loop
} //end of setGrid

void placeSpot(char gr[][SIZEX], Item spot)
{ //place spot at its new position in grid
	gr[spot.y][spot.x] = spot.symbol;
} //end of placeSpot

void placeHoles(char gr[][SIZEX], vector<Item> holes)
{ //place holes at their new positions in grid
	for (int i = 0; i < holes.size(); ++i)
		gr[holes[i].y][holes[i].x] = holes[i].symbol;
} //end of placeHoles

void placePills(char gr[][SIZEX], vector<Item> pills)
{ //place pills at their new positions in grid
	for (int i = 0; i < pills.size(); ++i)					//for every pill
		if (pills[i].isBeingRendered)						//if the pill being rendered
			gr[pills[i].y][pills[i].x] = pills[i].symbol;	//place the pill in the grid
} //end of placePills


void placeZombies(char gr[][SIZEX], vector<Item> zombies)
{
	for (int i = 0; i < zombies.size(); ++i)
	{
		if (zombies[i].isBeingRendered) //If the zombie is being rendered
			gr[zombies[i].y][zombies[i].x] = zombies[i].symbol; //Places a zombie symbol at the x & y of each zombie index in the vector
	}
}//end of placeZombies
//---------------------------------------------------------------------------
//----- update grid state
//---------------------------------------------------------------------------
void updateGrid(char grid[][SIZEX], Item spot, vector<Item> holes, vector<Item> pills, vector<Item> zombies)
{ //update grid configuration after each move
	void setGrid(char[][SIZEX]);
	void placeSpot(char g[][SIZEX], Item spot);
	void placeHoles(char g[][SIZEX], vector<Item> holes);
	void placePills(char g[][SIZEX], vector<Item> pills);
	void placeZombies(char g[][SIZEX], vector<Item> zombies);

	setGrid(grid);							//reset empty grid
	placeHoles(grid, holes);				//set holes in grid
	placePills(grid, pills);				//set pills in grid
	placeSpot(grid, spot);					//set spot in grid
	placeZombies(grid, zombies);			//set zombies in the grid
} //end of updateGrid

//---------------------------------------------------------------------------
//----- move the spot
//---------------------------------------------------------------------------
void updateSpotCoordinates(const char g[][SIZEX], Item& sp, int key, int& lives, string& mess, vector<Item>& pills, int& pillsRemaining)
{ //move spot in required direction
	void setKeyDirection(int k, int& dx, int& dy);
	void removePill(vector<Item>& pills, Item sp, string& message, int& pillsRemaining);

	//calculate direction of movement required by key - if any
	int dx(0), dy(0);
	setKeyDirection(key, dx, dy);			//find direction indicated by key
	//check new target position in grid 
	//and update spot coordinates if move is possible
	const int targetY(sp.y + dy);
	const int targetX(sp.x + dx);
	switch (g[targetY][targetX])
	{//...depending on what's on the target position in grid...
	case PILL:
		sp.y += dy;							//go in that Y direction
		sp.x += dx;							//go in that X direction
		++lives;							//add a life
		removePill(pills, sp, mess, pillsRemaining);		//remove the pill
		break;
	case HOLE:								//can move
		sp.y += dy;							//go in that Y direction
		sp.x += dx;							//go in that X direction
		--lives;
		break;
	case TUNNEL:
		sp.y += dy;							//go in that Y direction
		sp.x += dx;							//go in that X direction
		break;
	case WALL:								//hit a wall and stay there
		std::cout << '\a';						//beep the alarm
		mess = "       CANNOT GO THERE...        ";
		break;
	}
} //end of updateSpotCoordinates
void updateZombieCoordinates(const char g[][SIZEX], vector<Item>& zombies, Item spot, int& lives, string& message, bool frozen)
{
	void resetZombiePosition(vector<Item>& zombies, int arrayIndex); //Will reset the zombies position 
	int dx(0), dy(0); //The maximum amount the player can move by
	int displaceX(0), displaceY(0); //Distance in a vector form from the zombie

	if (frozen == false)
	{
		for (int i = 0; i < zombies.size(); ++i)
		{
			if (zombies[i].isBeingRendered)
			{
				displaceX = (spot.x - zombies[i].x); //Determine whether a positive dx is needed or a negative
				displaceY = (spot.y - zombies[i].y); //Determine whether a positive dy is needed or a negative

				if (displaceX != 0)//If the x displacement isn't 0 
				{
					dx = displaceX / abs(displaceX); //divide the displacement in x by it's absolute value (to give us either +1 or -1 change)
				}
				else //if x displacement is 0
				{
					dx = 0; //set dx to 0 
				}

				if (displaceY != 0) //If the y displacement isn't 0
				{
					dy = displaceY / abs(displaceY);//divide the displacement in y by it's absolute value (to givve us either +1 or -1 change)
				}
				else//if y displacement is 0 
				{
					dy = 0; //set dy to 0 
				}
				const int targetX(zombies[i].x + dx); //Set the target destination of the zombie's x to it's x position + the new change
				const int targetY(zombies[i].y + dy);//Set the target destination of the zombie's y to it's y position + the new change

				//If spot is in the location the zombie is moving into 
				if (spot.x == targetX && spot.y == targetY)
				{
					--lives; //Decrement spots lives
					resetZombiePosition(zombies, i); //reset this zombies position
				}

				for (int j = 0; j < zombies.size(); ++j)
				{
					//If the index i is not the same as j
					if (i != j)
					{
						//Check and see if another zombie (indexed via j) falls on the location that this one is moving to
						if (targetX == zombies[j].x && targetY == zombies[j].y)
						{
							resetZombiePosition(zombies, j);//if so reset the zombie
							resetZombiePosition(zombies, i);//and reset the other one
							dx = 0; 
							dy = 0;
						}
					}
				}
				//Switch statement check this target location
				switch (g[targetY][targetX])
				{
					//If the location on the grid has a pill, then follow the tunnel behaviour and walk onto the pill
				case PILL:
				case TUNNEL:
					//if it's a tunnel walk into the locaiton
					zombies[i].x += dx;
					zombies[i].y += dy;
					break;
				case HOLE:
					//if it's a hole then the zombie is removed from the game render & update loop so it's no longer shown or interacting
					zombies[i].isBeingRendered = false;
					break;
				}
			}
		}
	}

}//end of updateZombieCoordinates
void resetZombiePosition(vector<Item>& zombies, int arrayIndex)
{//A function that knows all the start locations of the zombies based on their index in the array
	switch (arrayIndex)
	{
	case 0://top left
		zombies[arrayIndex].x = 1;
		zombies[arrayIndex].y = 1;
		break;
	case 1://bottom left
		zombies[arrayIndex].x = (SIZEX - 2);
		zombies[arrayIndex].y = 1;
		break;
	case 2://top right
		zombies[arrayIndex].x = 1;
		zombies[arrayIndex].y = (SIZEY - 2);
		break;
	case 3://bottom right
		zombies[arrayIndex].x = (SIZEX - 2);
		zombies[arrayIndex].y = (SIZEY - 2);
		break;
	}
}//end of resetZombiePosition
//---------------------------------------------------------------------------
//----- process key
//---------------------------------------------------------------------------
void setKeyDirection(int key, int& dx, int& dy)
{ //
	switch (key)    //...depending on the selected key...
	{
	case LEFT:			//when LEFT arrow pressed...
		dx = -1;		//decrease the X coordinate
		dy = 0;
		break;
	case RIGHT:			//when RIGHT arrow pressed...
		dx = +1;		//increase the X coordinate
		dy = 0;
		break;
	case UP:			//when UP arrow pressed...
		dx = 0;
		dy = -1;		//decrease the Y coordinate
		break;
	case DOWN:			//when DOWN arrow pressed...
		dx = 0;
		dy = +1;		//increase the Y coordinate
		break;
	}
} //end of setKeyDirection

int getKeyPress()
{ //get key or command selected by user
	int keyPressed;
	keyPressed = getch();			//read in the selected arrow key or command letter
	while (keyPressed == 224)		//ignore symbol following cursor key
		keyPressed = getch();
	return(keyPressed);
} //end of getKeyPress

bool isArrowKey(int key)
{ //check if the key pressed is an arrow key (also accept 'K', 'M', 'H' and 'P')
	return ((key == LEFT) || (key == RIGHT) || (key == UP) || (key == DOWN));
} //end of isArrowKey

bool wantToQuit(int key)
{ //check if the key pressed is 'Q'
	return (toupper(key) == QUIT);
} //end of wantToQuit

bool outOfLives(int lives)
{ //check if spot has lives
	if (lives > 0)
		return false;
	else
		return true;
} //end of outOfLives

bool outOfZombies(vector<Item> zombies)
{//Check if all the zombies have died
	int counter = 0;
	for (int i = 0; i < zombies.size(); ++i)
	{
		if (!zombies[i].isBeingRendered)
			++counter;
	}
	return(counter == zombies.size() );
}//end outOfZombies
bool outOfPills(int pillsremaining)
{
	return(pillsremaining == 0);//if there are pills return false, else return true
}//end of outOfPills
//---------------------------------------------------------------------------
//----- display info on screen
//---------------------------------------------------------------------------
void displayMenu()
{
	void showMenuTitle();
	void showDateAndTime();
	void showCredits();
	void showInfo();
	
	SelectBackColour(clBlack);
	Clrscr();
	showMenuTitle();
	showDateAndTime();
	showCredits();
	showInfo();
}
void displayInfo()
{
	void showMenuTitle();
	void showDateAndTime();
	void showHelp();
	void showOptions();

	showMenuTitle();
	showDateAndTime();
	showHelp();
	showOptions();
}

void renderGame(const char gd[][SIZEX], string mess, int lives, string playerName, int highScore)
{ //display game title, messages, maze, spot and apples on screen
	void paintGrid(const char g[][SIZEX]);
	void showGameTitle();
	void showGameOptions();
	void showDateAndTime();
	void showPlayerScore(string playerName, int highScore);
	void showMessage(string, int lives);

	SelectBackColour(clBlack);
	Clrscr();
	Gotoxy(0, 0);
	//display grid contents
	paintGrid(gd);
	//display game title
	showGameTitle();
	showDateAndTime();
	//display game options available
	showGameOptions();
	//display message if any
	showMessage(mess, lives);
	showPlayerScore(playerName, highScore);
} //end of paintGame

void paintGrid(const char g[][SIZEX])
{ //display grid content on screen
	SelectBackColour(clDarkGrey);
	int line(4);
	Gotoxy(10, line);
	for (int row(0); row < SIZEY; ++row)		//for each row (vertically)
	{
		for (int col(0); col < SIZEX; ++col)	//for each column (horizontally)
		{
			bool insideWall;
			if (g[row][col] == SPOT)
				SelectTextColour(clRed);
			if (g[row][col] == PILL)
				SelectTextColour(clCyan);
			if (g[row][col] == ZOMBIE)
				SelectTextColour(clGreen);
			if (g[row][col] == HOLE)
				SelectTextColour(clBlack);
			if (g[row][col] == WALL)
			{
				SelectTextColour(clWhite);
				SelectBackColour(clBlack);
			}
			cout << g[row][col];				//output cell content
			SelectBackColour(clDarkGrey);
		} //end of col-loop
		++line;
		Gotoxy(10, line);
		//cout << endl << "          ";
	} //end of row-loop
} //end of paintGrid
void showPlayerScore(string playerName, int highScore)
{
	SelectBackColour(clBlue);
	SelectTextColour(clCyan);
	Gotoxy(10, 17);
	cout << " NAME - " << playerName << " ";
	Gotoxy(40, 17);
	cout << " SCORE - " << highScore << " ";
}
void showMessage(string m, int lives)
{ //print auxiliary messages if any
	SelectBackColour(clDarkCyan);
	SelectTextColour(clGreen);
	Gotoxy(40, 13);
	cout << "      You have " << lives << " lives left      ";
	Gotoxy(40, 14);
	cout << m;	//display current message
} //end of showMessage

void clearMessage()
{ //clear message area on screen
	SelectBackColour(clDarkCyan);
	SelectTextColour(clGreen);
	Gotoxy(40, 14);
	string str(33, ' ');
	cout << str;								//display blank message
} //end of setMessage

void showMenuTitle()
{
	SelectBackColour(clWhite);
	SelectTextColour(clBlack);
	Gotoxy(10, 2);
	cout << "                    ";
	Gotoxy(10, 3);
	cout << "  ----------------  ";
	Gotoxy(10, 4);
	cout << "  LEFT FOR DEAD: 3  ";
	Gotoxy(10, 5);
	cout << "  ----------------  ";
	Gotoxy(10, 6);
	cout << "                    ";
}//end of showMenuTitle

void showGameTitle()
{ //display game title
	SelectBackColour(clWhite);
	SelectTextColour(clBlack);
	Gotoxy(9, 2);
	cout << "   LEFT FOR DEAD: 3   ";
} //end of showTitle

void showDateAndTime()
{
	SelectBackColour(clDarkYellow);
	SelectTextColour(clYellow);
	Gotoxy(40, 2);
	cout << "  ASHLEY SWANSON 1st April 2015  ";
	Gotoxy(40, 3);
	cout << "      " << GetDate() << "   " << GetTime() << "      ";
}//end of showDateAndTime

void showCredits()
{
	SelectBackColour(clBlue);
	SelectTextColour(clWhite);
	Gotoxy(10, 8);
	cout << "  GROUP 1Z:         ";
	Gotoxy(10, 9);
	cout << "  Masimba Walker    ";
	Gotoxy(10, 10);
	cout << "  Kris Taylor       ";
	Gotoxy(10, 11);
	cout << "  Ashley Swanson    ";
	Gotoxy(10, 13);
	cout << " (We are in no way  ";
	Gotoxy(10, 14);
	cout << "  affiliated with   ";
	Gotoxy(10, 15);
	cout << " Valve Corporation) ";
}

void showInfo()
{
	SelectBackColour(clDarkMagenta);
	SelectTextColour(clGrey);
	Gotoxy(40, 5);
	cout << " Left for Dead: 3 is a top-down  ";
	Gotoxy(40, 6);
	cout << " strategy game, where you find   ";
	Gotoxy(40, 7);
	cout << " yourself avoiding the horde AND ";
	Gotoxy(40, 8);
	cout << " feeding your addiction to pixels";
	Gotoxy(40, 9);
	cout << "                                 ";
	Gotoxy(40, 10);
	cout << " PRESS P TO START PLAYING        ";
	Gotoxy(40, 11);
	cout << " PRESS I FOR MORE INFORMATION    ";
}

void showOptions()
{ //show options
	SelectBackColour(clDarkMagenta);
	SelectTextColour(clGrey);
	Gotoxy(40, 5);
	cout << " PRESS ENTER TO GO BACK          ";
	Gotoxy(40, 6);
	cout << " PRESS P TO PLAY, Q TO QUIT      ";
	Gotoxy(40, 7);
	cout << "                                 ";
	Gotoxy(40, 8);
	cout << " MOVE USING THE ARROW KEYS       ";
	Gotoxy(40, 9);
	cout << " PRESS F TO FREEZE THE ZOMBIES   ";
	Gotoxy(40, 10);
	cout << " PRESS X TO KILL ALL ZOMBIES     ";
	Gotoxy(40, 11);
	cout << " PRESS E TO EAT ALL THE PILLS    ";
} //end of showOptions

void showGameOptions()
{ //show game options
	SelectBackColour(clDarkMagenta);
	SelectTextColour(clGrey);
	Gotoxy(40, 5);
	cout << "  Collect Pills - Avoid Zombies  ";
	Gotoxy(40, 6);
	cout << "                                 ";
	Gotoxy(40, 7);
	cout << " MOVE USING THE ARROW KEYS       ";
	Gotoxy(40, 8);
	cout << " PRESS F TO FREEZE THE ZOMBIES   ";
	Gotoxy(40, 9);
	cout << " PRESS X TO KILL ALL ZOMBIES     ";
	Gotoxy(40, 10);
	cout << " PRESS E TO EAT ALL THE PILLS    ";
	Gotoxy(40, 11);
	cout << " PRESS Q TO QUIT THE GAME        ";
} //end of showGameOptions

void showHelp()
{
	SelectBackColour(clBlue);
	SelectTextColour(clWhite);
	Gotoxy(10, 8);
	cout << "  AIM OF THE GAME:  ";
	Gotoxy(10, 9);
	cout << " - Collect Pills    ";
	Gotoxy(10, 10);
	cout << " - Avoid Zombies    ";
	Gotoxy(10, 11);
	cout << " - Avoid Holes      ";
	Gotoxy(10, 13);
	cout << " YOUR SCORE  IS THE ";
	Gotoxy(10, 14);
	cout << "NUMBER OF LIVES LEFT";
	Gotoxy(10, 15);
	cout << " WHEN THE GAME ENDS ";
}
/*
YOUR SCORE IS THE NUMBER OF LIVES LEFT WHEN THE GAME ENDS
*/
void endProgram(int lives, int key, vector<Item> zombies, int pillsRemaining, string name, int highscore)
{ //end program with appropriate 
	void writeToSaveFile(string name, int lives, int highscore);
	SelectBackColour(clBlack);
	SelectTextColour(clYellow);
	Gotoxy(40, 13);

	if (outOfLives(lives))
		cout << "            YOU LOST!            ";
	if (wantToQuit(key))
		cout << "          PLAYER QUITS!          ";
	if (outOfZombies(zombies) && outOfPills(pillsRemaining))
	{
		cout << "         ALL ZOMBIES DIED!	      ";
	}
	writeToSaveFile(name, lives, highscore);

	//If zombies are not being rendered
	//hold output screen until a keyboard key is hit
	Gotoxy(40, 14);
	cout << " ";
	system("pause");
} //end of endProgram

void displayExit()
{
	SelectBackColour(clBlue);
	SelectTextColour(clWhite);
	Gotoxy(40, 13);
	cout << "       THANKS FOR PLAYING!       ";
	Gotoxy(40, 14);
	cout << " ";
}
void removePill(vector<Item>& pills, Item sp, string& message, int& pillsRemaining)
{
	for (int i = 0; i < pills.size(); i++)					//for every pill
		if (pills[i].x == sp.x && pills[i].y == sp.y)		//check if the pills coordinates equal spots coordinates
		{
			pills[i].isBeingRendered = false;				//if they do, no longer draw that pill
			--pillsRemaining;
		}
}//end of removePill

string getPlayerName()
{
	string name;
	const int eraseStart = 19;
	Gotoxy(10, 17);
	cout << "Please enter your name (20 Characters) - ";
	cin >> name;
	//Remove any letters above the max characters
	for (int i = eraseStart; i < name.size(); ++i)
	{
		name.erase(i);
	}
	return(name);
}
int getPlayerScore(string name)
{
	int highScore = 0; //Where the players highest score will be stored

	ifstream fromFile;

	fromFile.open((name + EXTENSION), ios::in);

	if (fromFile.fail())//If the file failed to open
	{
		//print an error to the screen
		std::cout << "ERROR! Unable to read from save file!";
	}
	else
	{
		if (!fromFile.eof())//If not at the end of the file
		{
			fromFile >> highScore;//read the score
		}
		else//else if there is no score
		{
			highScore = -1;//set highest score to -1
		}
	}
	fromFile.close();//close the file
	
	return(highScore); //return the value of highest score
}
void writeToSaveFile(string name, int lives, int highscore)
{
	ofstream toFile;
	toFile.open((name + EXTENSION), ios::out);

	if (toFile.fail())
	{
		cout << "ERROR! Unable to write save file!";
	}
	else
	{
		if (lives > highscore)
			toFile << lives;
		toFile << lives;
	}
	toFile.close();
}//end of writeToSaveFile
void cheats(int& lives, vector<Item>& zombies, vector<Item>& pills, int key, bool& frozen)
{
	if (toupper(key) == EAT)
	{
		for (int i = 0; i < pills.size(); ++i)
			pills[i].isBeingRendered = false;
	}

	if (toupper(key) == EXTERMINATE)
	{
		for (int i = 0; i < zombies.size(); ++i)
			zombies[i].isBeingRendered = false;
	}

	if (toupper(key) == FREEZE)
	{

		if (frozen == false)
			frozen = true;
		else
			frozen = false;
	}
}
