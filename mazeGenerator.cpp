#include <iostream>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <stack>
//#include <fstream>
using namespace std;

//define size of maze, must be odd number
#define SIZE 21

//cell structure
struct Cell {
	bool visited;
	bool topWall;
	bool bottomWall;
	bool leftWall;
	bool rightWall;

	bool vacant;
	bool startingCell;
	bool goalCell;
};

//declare functions
void Initialize(Cell Level[][SIZE]);
void ClearScreen();
void Redraw(Cell Level[][SIZE]); //DISPLAY
void DrawMaze(Cell Level[][SIZE], int &positionX, int &positionY, int &goalX, int & goalY);
int* getStartAndGoalCoords(Cell Level[][SIZE]);
int** generateItems(Cell level[][SIZE], int numItems, int scale);


// int main() {
// 	Cell Level[SIZE][SIZE];
// 	int positionX = 0;
// 	int positionY = 0;
// 	int goalX = 0;
// 	int goalY = 0;

// 	Initialize(Level);
// 	Redraw(Level); //DISPLAY
// 	DrawMaze(Level, positionX, positionY, goalX, goalY);

// 	return 0;
// }

//set up maze
void Initialize(Cell Level[][SIZE]) {
	for(int i = 0; i < SIZE; i++) {
		for(int j = 0; j < SIZE; j++) {
			Level[i][j].vacant = false; //DISPLAY
			Level[i][j].visited = false; //no cells visited
			Level[i][j].topWall = true; //all walls intact, no steps taken
			Level[i][j].bottomWall = true;
			Level[i][j].leftWall = true;
			Level[i][j].rightWall = true;
			Level[i][j].startingCell = false;
			Level[i][j].goalCell = false;
		}
	}
	for(int i = 1; i < SIZE-1; i++) {
		for(int j = 1; j < SIZE-1; j++) {
			//border cells have fewer walls within maze bounds
			Level[1][j].topWall = false;
			Level[SIZE-2][j].bottomWall = false;
			Level[i][1].leftWall = false;
			Level[i][SIZE-2].rightWall = false;
		} 
	}
}

void ClearScreen() {
	HANDLE hOut;
	COORD Position;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}

void Redraw(Cell Level[][SIZE]) {
	for(int i = 0; i < SIZE; i++) {
		cout << endl;
		for(int j = 0; j < SIZE; j++){
			if (Level[i][j].startingCell) cout << " S";
			else if (Level[i][j].goalCell) cout << " E";
			else if(Level[i][j].vacant) cout << "  ";
			else cout << " " << "*";
		}
	}
}

//generate the maze
void DrawMaze(Cell Level[][SIZE]) {
	srand((unsigned)time(NULL)); //pick a random cell to start in
	int random = 0;
	int randomX = ((2*rand())+1)%(SIZE-1); //generate random odd number between 1 and size
	int randomY = ((2*rand())+1)%(SIZE-1);
	int positionX = randomX; //set random numbers as start place
	int positionY = randomY;
	int visitedCells = 1;
	int totalCells = ((SIZE-1)/2)*((SIZE-1)/2);
	int percent = 0;
	stack<int> backTrackX, backTrackY; //need stack to trace the reverse path

	Level[randomY][randomX].vacant = true; //mark start cell with an S //DISPLAY
	Level[randomY][randomX].startingCell = true;
	Level[randomY][randomX].visited = true; //mark start cell as visited

	while(visitedCells < totalCells) {
		if(((Level[randomY-2][randomX].visited == false) && (Level[randomY][randomX].topWall == true && Level[randomY-2][randomX].bottomWall == true)) ||
		   ((Level[randomY+2][randomX].visited == false) && (Level[randomY][randomX].bottomWall == true && Level[randomY+2][randomX].topWall == true)) ||
		   ((Level[randomY][randomX-2].visited == false) && (Level[randomY][randomX].leftWall == true && Level[randomY][randomX-2].rightWall == true)) ||
		   ((Level[randomY][randomX+2].visited == false) && (Level[randomY][randomX].rightWall == true && Level[randomY][randomX+2].leftWall == true))) 
		{
			random = (rand() % 4) + 1; //pick wall in random direction to knock down

			//choose upper wall
			if((random == 1) && (randomY > 1)) {
				if(Level[randomY-2][randomX].visited == false) { //if cell not visited
					Level[randomY-1][randomX].vacant = true; //mark as wall
					Level[randomY-1][randomX].visited = true; //change cell status to visited
					Level[randomY][randomX].topWall = false; //knock down top wall

					backTrackX.push(randomX); //push x for back track
					backTrackY.push(randomY); //push y for back track

					randomY -= 2; //move to next cell
					Level[randomY][randomX].visited = true; //mark cell moved to as visited
					Level[randomY][randomX].vacant = true; //mark as wall
					Level[randomY][randomX].bottomWall = false; //knock down wall
					visitedCells++; //increase visited cells counter
				}
				else
					continue;
			}

			//choose lower wall
			else if((random == 2) && (randomY < SIZE-2)) {
				if(Level[randomY+2][randomX].visited == false) { //if cell not visited
					Level[randomY+1][randomX].vacant = true; //mark as wall
					Level[randomY+1][randomX].visited = true; //change cell status to visited
					Level[randomY][randomX].bottomWall = false; //knock down bottom wall

					backTrackX.push(randomX); //push x for back track
					backTrackY.push(randomY); //push y for back track

					randomY += 2; //move to next cell
					Level[randomY][randomX].visited = true; //mark cell moved to as visited
					Level[randomY][randomX].vacant = true; //mark as wall
					Level[randomY][randomX].topWall = false; //knock down wall
					visitedCells++; //increase visited cells counter
				}
				else
					continue;
			}

			//choose left wall
			else if((random == 3) && (randomX > 1)) {
				if(Level[randomY][randomX-2].visited == false) { //if cell not visited
					Level[randomY][randomX-1].vacant = true; //mark as wall
					Level[randomY][randomX-1].visited = true; //change cell status to visited
					Level[randomY][randomX].leftWall = false; //knock down left wall

					backTrackX.push(randomX); //push x for back track
					backTrackY.push(randomY); //push y for back track

					randomX -= 2; //move to next cell
					Level[randomY][randomX].visited = true; //mark cell moved to as visited
					Level[randomY][randomX].vacant = true; //mark as wall
					Level[randomY][randomX].rightWall = false; //knock down wall
					visitedCells++; //increase visited cells counter
				}
				else
					continue;
			}

			//choose right wall
			else if((random == 4) && (randomX < SIZE-2)) {
				if(Level[randomY][randomX+2].visited == false) { //if cell not visited
					Level[randomY][randomX+1].vacant = true; //mark as wall
					Level[randomY][randomX+1].visited = true; //change cell status to visited
					Level[randomY][randomX].rightWall = false; //knock down right wall

					backTrackX.push(randomX); //push x for back track
					backTrackY.push(randomY); //push y for back track

					randomX += 2; //move to next cell
					Level[randomY][randomX].visited = true; //mark cell moved to as visited
					Level[randomY][randomX].vacant = true; //mark as wall
					Level[randomY][randomX].leftWall = false; //knock down wall
					visitedCells++; //increase visited cells counter
				}
				else
					continue;
			}

			percent = (visitedCells*100/totalCells*100)/100; // progress in percentage
			//cout << endl << "	Generating a Random Maze... " << percent << "%" << endl;
		}

		else {
			randomX = backTrackX.top();
			backTrackX.pop();

			randomY = backTrackY.top();
			backTrackY.pop();
		}

		//ClearScreen();
		//Redraw(Level, positionX, positionY, -1, -1);
	}

	int goalX = randomX;
	int goalY = randomY;
	Level[goalY][goalX].vacant = true;
	Level[goalY][goalX].goalCell = true;
	//system("cls");
	//ClearScreen();
	//Redraw(Level);
	//cout << endl << "\t	Complete!" << endl;
}

int* getStartAndGoalCoords(Cell Level[][SIZE]){
	int* coords = new int[4];

	for (int x = 0; x < SIZE; x++){
		for (int z = 0; z < SIZE; z++){
			if (Level[x][z].startingCell){
				coords[0] = x;
				coords[1] = z;
			}
			else if (Level[x][z].goalCell){
				coords[2] = x;
				coords[3] = z;
			}
		}
	}
	return coords;
}

vector<int*>* getWalls(Cell Level[][SIZE], int mazeScale){
	vector<int*>* walls = new vector<int*>;

	for (int x = 0; x < SIZE; x++){
		for (int z = 0; z < SIZE; z++){
			if (!Level[x][z].vacant){
				int* wall = new int[2];
				wall[0] = x*mazeScale;
				wall[1] = z*mazeScale;
				walls->push_back(wall);
			}
		}
	}
}

bool wallIntersection(vector<int*>* walls, float x, float z){
	for (int i = 0; i < walls->size(); i++){
		float wallX = (float)walls->at(i)[0];
		float wallZ = (float)walls->at(i)[1];
		if ((wallX-1.5 < x) && (x < wallX+1.5) && (wallZ-1.5 < z) && (z < wallZ+1.5)) return true; //TODO: make bounds variables!!!
	}
	return false;
}

//random items in maze
int** generateItems(Cell level[][SIZE], int numItems, int scale){
	srand((unsigned)time(NULL));
	int** locations = new int*[numItems];
	for (int i = 0; i < numItems; i++){
		bool foundLocation = false;
		int* coords = new int[2];
		while (!foundLocation){
			coords[0] = rand() % SIZE;
			coords[1] = rand() % SIZE;
			if (level[coords[0]][coords[1]].vacant){
				coords[0] *= scale;
				coords[1] *= scale;
				foundLocation = true;
			}
		}
		locations[i] = coords;
	}
	return locations;
}

bool* itemIntersection(int** items, int numItems,bool* pickedUp, float x, float z){
	for (int i = 0; i < numItems; i++){
		if (!pickedUp[i]){
			float itemX = (float)items[i][0];
			float itemZ = (float)items[i][1];
			if ((itemX-1 < x) && (x < itemX+1) && (itemZ-1 < z) && (z < itemZ+1)) pickedUp[i]=true;
			else pickedUp[i]=false;
		} //TODO: make bounds variables!!!
	}
	return pickedUp;
}

int itemsPickedUp(bool* items, int numItems){
	int pickedUp = 0;
	for (int i = 0; i < numItems; i++){
		if (items[i]) pickedUp++;
	}
	return pickedUp;
}