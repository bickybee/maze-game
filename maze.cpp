#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <gl/glut.h>
#include <gl/gl.h>
#include <string>
#include <vector>
#include "mazeGenerator.cpp"
#include "drawing.cpp"

using namespace std;

#define SIZE 21

//maze stuff
int mazeScale = 2;
vector<int*>* walls = new vector<int*>;
Cell maze[SIZE][SIZE];
float eye[3] = {SIZE*mazeScale,10,SIZE*mazeScale};
int numItems = 9;
int** items = new int*[2];
bool* pickedUp = new bool[numItems];
bool win = false;

//lighting
float light_pos0[] = {SIZE,30,SIZE,1.0};
float amb0[4] = {0.5,0.5,0.5,1};
float diff0[4] = {1,1,1, 1};
float spec0[4] = {1, 1, 1, 1};

//animation
float pos[] = {5,1,5};
float rot[] = {0, 0, 0};
int frame = 0;
int holdKey = 0;
bool animate = false;
int numSnows = 500;
float** snows = new float*[numSnows];

//screens
//GLubyte*

/////////////////OLD STUFF///////////////
/* TEXTURE */
/*GLubyte* image;
GLubyte* img_data;
int width, height, MAX;*/
///////////////END OF OLD STUFF/////////////

void display()
{
	//clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0],eye[1],eye[2],pos[0],pos[1],pos[2],0,1,0);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0,GL_POSITION,light_pos0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);

	glPushMatrix();
	glScalef(mazeScale, 1, mazeScale);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	drawXZPlane(0, SIZE);
	//glBindTexture(GL_TEXTURE_2D, textures[1]);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //TEXTURE_MIN_FILTER
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	drawWalls(maze);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	drawItems(items, pickedUp, numItems);
	drawSnowman(pos, rot, frame, animate);
	//drawPenguin(pos, rot, frame);
	updateSnow(snows, numSnows, SIZE*2, SIZE*2, SIZE*2);
	drawSnow(snows, numSnows);
	draw2D(win);
	
	//swap buffers - rendering is done to the back buffer, bring it forward to display
	glutSwapBuffers();

	//force a redisplay, to keep the animation running
	glutPostRedisplay();
}

void idle(){
	win = checkWin(pickedUp, numItems);
}

void init() {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(2, textures);
	//load wall texture
	wall_tex = LoadPPM("marble.ppm", &width, &height, &MAX);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, wall_tex);

	floor_tex = LoadPPM("snail_a.ppm", &width, &height, &MAX);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, floor_tex);
}

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
	drawSnowman(pos, rot, frame, animate);
	initializeSnow(snows, numSnows, SIZE*2, SIZE*2, SIZE*2);
	walls = getWalls(maze, mazeScale);
}

void kbd(unsigned char key, int x, int y)
{
	if (holdKey>1) animate = true;

	switch (key)
	{
		case 'q':
		case 'Q':
		case 27:
			exit (0);
			break;

		case 'a':
		case 'A':
			if(!wallIntersection(walls, pos[0]-0.25, pos[2]))
				{pos[0] -= 0.25; eye[0] -= 0.25;}
			rot[1] = -90;
			holdKey++;
			if (animate) frame++;
			pickedUp = itemIntersection(items, numItems, pickedUp, pos[0], pos[2]);
			break;

		case 'w':
		case 'W':
			if(!wallIntersection(walls, pos[0], pos[2]-0.25))
				{pos[2] -= 0.25; eye[2] -=0.25;}
			rot[1] = 180;
			holdKey++;
			if (animate) frame++;
			pickedUp = itemIntersection(items, numItems, pickedUp, pos[0], pos[2]);
			break;

		case 'd':
		case 'D':
			if(!wallIntersection(walls, pos[0]+0.25, pos[2]))
				{pos[0]+=0.25; eye[0] += 0.25;}
			rot[1] = 90;
			holdKey++;
			if (animate) frame++;
			pickedUp = itemIntersection(items, numItems, pickedUp, pos[0], pos[2]);
			break;

		case 's':
		case 'S':
			if(!wallIntersection(walls, pos[0], pos[2]+0.25))
				{pos[2] += 0.25; eye[2] += 0.25;}
			rot[1] = 0;
			holdKey++;
			if (animate) frame++;
			pickedUp = itemIntersection(items, numItems, pickedUp, pos[0], pos[2]);
			break;

		case 13: //enter
			if (win) initMaze();
			break;
			
	}
	glutPostRedisplay();
}

//resets the vars used to check if a key is being held down (for walk-animation)
void keyUp(unsigned char key,int x, int y){
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

void special(int key, int x, int y){
	switch(key){
		case GLUT_KEY_LEFT:
		eye[0]-=0.2;
		break;

		case GLUT_KEY_RIGHT:
		eye[0]+=0.2;
		break;

		case GLUT_KEY_UP:
		eye[2]+=0.2;
		break;

		case GLUT_KEY_DOWN:
		eye[2]-=0.2;
		break;

		default:
		break;
	}
}

int main(int argc, char** argv)
{
	//glut initialization stuff:
	// set the window size, display mode, and create the window
	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Maze Game");

	//enable Z buffer test, otherwise things appear in the order they're drawn
	glEnable(GL_DEPTH_TEST);

	//setup the initial view
	// change to projection matrix mode, set the extents of our viewing volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,1,1,100);

	//set clear colour to white
	glClearColor(0, 0, 0, 0);

	glMatrixMode(GL_MODELVIEW);
	//look down from a 45 deg. angle
	glRotatef(45, 1, 0, 0);

	//register glut callbacks for keyboard and display function
	glutKeyboardFunc(kbd);
	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutKeyboardUpFunc(keyUp);
	glutIdleFunc(idle);
	init();

	/*glEnable(GL_TEXTURE_2D);
	img_data = LoadPPM((char*)"marble.ppm", &width, &height, &MAX);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
	GL_UNSIGNED_BYTE, img_data); */


	//win_img = LoadPPM((char*)"ribbon.ppm", &winW, &winH, &winMAX);
	init();
	initMaze();


	//start the program!
	glutMainLoop();

	return 0;
}