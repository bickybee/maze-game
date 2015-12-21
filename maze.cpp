// WINTER MAZE 
// Final project for COMP SCI 3GC3
// Vicky Bilbily, Lucas Bongers, Daniel Cefaratti

#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <gl/glut.h>
#include <gl/gl.h>
#include <string>
#include <vector>
#include "mazeGenerator.cpp"
#include "drawing.cpp"

using namespace std;
#define SIZE 21

//maze and game vars
int mazeScale = 2;
vector<int*>* walls = new vector<int*>;
Cell maze[SIZE][SIZE];
float eye[3] = {SIZE*mazeScale,10,SIZE*mazeScale};
int numItems = 9;
int** items = new int*[2];
bool* pickedUp = new bool[numItems];
int numPickedUp = 0;
bool win = false;
int seconds = 0;

//lighting
float light_pos0[] = {SIZE,30,SIZE,1.0};
float amb0[4] = {1,1,1,1};
float diff0[4] = {1,1,1, 1};
float spec0[4] = {1, 1, 1, 1};

//character
float pos[] = {5,1,5};
float rot[] = {0, 0, 0};
int frame = 0;
int holdKey = 0;
bool animate = false;
int numSnows = 500;
float** snows = new float*[numSnows];
int character = 0;

void display()
{
	//clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//camera
	gluLookAt(eye[0],eye[1],eye[2],pos[0],pos[1],pos[2],0,1,0);

	//skybox
	glColor3f(1,1,1);
	glPushMatrix();
	glTranslatef(SIZE,25,SIZE);
	glRotatef(90,1,0,0);
	drawSkyBox(50,1);
	glPopMatrix();

	//lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_POSITION,light_pos0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);

	//plane
	glPushMatrix();
	glScalef(mazeScale, 1, mazeScale);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	drawXZPlane(0, SIZE);

	//maze walls
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	drawWalls(maze);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//items
	drawItems(items, pickedUp, numItems);

	//character
	if (character%2==1) drawSnowman(pos, rot, frame, animate);
	else drawPenguin(pos, rot, frame, animate);

	//snow
	updateSnow(snows, numSnows, SIZE*2, SIZE*2, SIZE*2);
	drawSnow(snows, numSnows);

	//bitmaps
	draw2D(win, seconds, numPickedUp);

	//display!
	glutSwapBuffers();
	glutPostRedisplay();
}

//constantly check for wins
void idle(){
	numPickedUp = itemsPickedUp(pickedUp, numItems);
	win = numPickedUp==numItems;
}

//update the timer
void timer(int value){
	if (!win) seconds++;
	 glutTimerFunc(1000, timer, 0);
}

//load textures
void initTextures() {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(3, textures);
	//wall texture
	wall_tex = LoadPPM("ice5.ppm", &width, &height, &MAX);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, wall_tex);
	//plane texture
	floor_tex = LoadPPM("snow3.ppm", &width, &height, &MAX);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, floor_tex);
	//skybox texture
	sky = LoadPPM("sky2.ppm", &width, &height, &MAX);
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, sky);

}

//set up maze using back-end algorithm
//initialize character and camera accordingly
//also initialize snow here
void initMaze(){
	Initialize(maze);
	DrawMaze(maze);
	items = generateItems(maze,numItems,mazeScale);
	for (int i=0; i <numItems;i++){
		pickedUp[i] = false;
	}
	int* startEnd = getStartAndGoalCoords(maze);
	pos[0] = (float)startEnd[0]*mazeScale;
	pos[2] = (float)startEnd[1]*mazeScale;
	eye[0] = pos[0];
	eye[1] = 8;
	eye[2] = pos[2] + 15;
	initializeSnow(snows, numSnows, SIZE*2, SIZE*2, SIZE*2);
	walls = getWalls(maze, mazeScale);
}

void kbd(unsigned char key, int x, int y){
	if (holdKey>1) animate = true;

	switch (key){
		//exit
		case 27:
			exit (0);
			break;

		//MOVEMENT
		//if not about to intersect with wall, move character
		//check if picking up item, and if so, update variables accordingly

		//left
		case 'a':
		case 'A':
			if(!wallIntersection(walls, pos[0]-0.25, pos[2]))
				{pos[0] -= 0.25; eye[0] -= 0.25;}
			rot[1] = -90;
			holdKey++;
			if (animate) frame++;
			pickedUp = itemIntersection(items, numItems, pickedUp, pos[0], pos[2]);
			break;

		//up
		case 'w':
		case 'W':
			if(!wallIntersection(walls, pos[0], pos[2]-0.25))
				{pos[2] -= 0.25; eye[2] -=0.25;}
			rot[1] = 180;
			holdKey++;
			if (animate) frame++;
			pickedUp = itemIntersection(items, numItems, pickedUp, pos[0], pos[2]);
			break;

		//right
		case 'd':
		case 'D':
			if(!wallIntersection(walls, pos[0]+0.25, pos[2]))
				{pos[0]+=0.25; eye[0] += 0.25;}
			rot[1] = 90;
			holdKey++;
			if (animate) frame++;
			pickedUp = itemIntersection(items, numItems, pickedUp, pos[0], pos[2]);
			break;

		//down
		case 's':
		case 'S':
			if(!wallIntersection(walls, pos[0], pos[2]+0.25))
				{pos[2] += 0.25; eye[2] += 0.25;}
			rot[1] = 0;
			holdKey++;
			if (animate) frame++;
			pickedUp = itemIntersection(items, numItems, pickedUp, pos[0], pos[2]);
			break;

		//toggle between characters
		case 'c':
		case 'C':
			character++;
			break;

		//if you've own, hit enter to restart
		case 13:
			if (win)
				{initMaze(); seconds = 0;}
			break;

		default:
			break;
			
	}
	glutPostRedisplay();
}

//for walk animation
//keyUp = no more animating
void keyUp(unsigned char key, int x, int y){
	switch(key)
	{
		case 'a':
		case 's':
		case 'd':
		case 'w':
			holdKey = 0;
			animate = false;
			break;
		default:
			break;

	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{	
	//glut inits
	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Winter Maze");
	glEnable(GL_DEPTH_TEST);

	//viewing volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,1,0.1,500);

	//clear color
	glClearColor(0, 0, 0, 0);

	//callbacks
	glutKeyboardFunc(kbd);
	glutDisplayFunc(display);
	glutKeyboardUpFunc(keyUp);
	glutIdleFunc(idle);
	glutTimerFunc(1000, timer, 0);

	//game inits
	initTextures();
	initMaze();

	//start the program!
	glutMainLoop();

	return 0;
}