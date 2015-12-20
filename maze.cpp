#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <gl/glut.h>
#include <gl/gl.h>
#include <string>
#include <vector>
#include "mazeGenerator.cpp"

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
float headRot[] = {0, 0, 0};
int frame = 0;
int holdKey = 0;
bool animate = false;

//screens
//GLubyte*

/////////////////OLD STUFF///////////////
/* TEXTURE */
/*GLubyte* image;
GLubyte* img_data;
int width, height, MAX;*/
///////////////END OF OLD STUFF/////////////

/////////////////NEW STUFF///////////////
GLubyte* wall_tex;
GLubyte* floor_tex;
int width, height, MAX;
GLuint textures[2];
///////////////END OF NEW STUFF//////////////

GLubyte* LoadPPM(const char* file, int* width, int* height, int* MAX)
{
	GLubyte* img;
	FILE *fd;
	int n, m;
	int  k, nm;
	char c;
	int i;
	char b[100];
	float s;
	int red, green, blue;
	
	/* first open file and check if it's an ASCII PPM (indicated by P3 at the start) */
	fd = fopen(file, "r");
	fscanf(fd,"%[^\n] ",b);
	if(b[0]!='P'|| b[1] != '3')
	{
		//printf("%s is not a PPM file!\n",file); 
		exit(0);
	}
	//printf("%s is a PPM file\n", file);
	fscanf(fd, "%c",&c);

	/* next, skip past the comments - any line starting with #*/
	while(c == '#') 
	{
		fscanf(fd, "%[^\n] ", b);
		//printf("%s\n",b);
		fscanf(fd, "%c",&c);
	}
	ungetc(c,fd); 

	/* now get the dimensions and MAX colour value from the image */
	fscanf(fd, "%d %d %d", &n, &m, &k);

	//printf("%d rows  %d columns  MAX value= %d\n",n,m,k);

	/* calculate number of pixels and allocate storage for this */
	nm = n*m;
	img = (GLubyte*)malloc(3*sizeof(GLuint)*nm);
	s=255.0/k;

	/* for every pixel, grab the read green and blue values, storing them in the image data array */
	for(i=0;i<nm;i++) 
	{
		fscanf(fd,"%d %d %d",&red, &green, &blue );
		img[3*nm-3*i-3]=red*s;
		img[3*nm-3*i-2]=green*s;
		img[3*nm-3*i-1]=blue*s;
	}

	/* finally, set the "return parameters" (width, height, MAX) and return the image array */
	*width = n;
	*height = m;
	*MAX = k;

	return img;
}

void drawCube() {
	glBegin(GL_QUADS);
	
	//front
    glNormal3f(0.0, 0.0, -1.0);
    //glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexCoord2f(0, 0);
    glVertex3f( 1.0f/2,  1.0f/2, 1.0f/2);

    glNormal3f(0.0, 0.0, -1.0);
    glTexCoord2f(0, 1);
    glVertex3f(-1.0f/2,  1.0f/2, 1.0f/2);

    glNormal3f(0.0, 0.0, -1.0);
    glTexCoord2f(1, 0);
    glVertex3f(-1.0f/2, -1.0f/2, 1.0f/2);

    glNormal3f(0.0, 0.0, -1.0);
    glTexCoord2f(1, 1);
    glVertex3f( 1.0f/2, -1.0f/2, 1.0f/2);
    
	//top
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0, 0);
	glVertex3f( 1.0f/2, 1.0f/2, -1.0f/2);

	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0, 1);
    glVertex3f(-1.0f/2, 1.0f/2, -1.0f/2);

    glNormal3f(0.0, 1.0, 0.0);
    glTexCoord2f(1, 0);
    glVertex3f(-1.0f/2, 1.0f/2,  1.0f/2);

    glNormal3f(0.0, 1.0, 0.0);
    glTexCoord2f(1, 1);
    glVertex3f( 1.0f/2, 1.0f/2,  1.0f/2);

	//bottom
	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(0, 0);
	glVertex3f( 1.0f/2, -1.0f/2,  1.0f/2);

	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(0, 1);
    glVertex3f(-1.0f/2, -1.0f/2,  1.0f/2);

    glNormal3f(0.0, -1.0, 0.0);
    glTexCoord2f(1, 0);
    glVertex3f(-1.0f/2, -1.0f/2, -1.0f/2);

    glNormal3f(0.0, -1.0, 0.0);
    glTexCoord2f(1, 1);
    glVertex3f( 1.0f/2, -1.0f/2, -1.0f/2);

	//left side
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(0, 0);
	glVertex3f(-1.0f/2,  1.0f/2,  1.0f/2);

	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(0, 1);
    glVertex3f(-1.0f/2,  1.0f/2, -1.0f/2);

    glNormal3f(-1.0, 0.0, 0.0);
    glTexCoord2f(1, 0);
    glVertex3f(-1.0f/2, -1.0f/2, -1.0f/2);

    glNormal3f(-1.0, 0.0, 0.0);
    glTexCoord2f(1, 1);
    glVertex3f(-1.0f/2, -1.0f/2,  1.0f/2);

	//right side
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(0, 0);
	glVertex3f(1.0f/2,  1.0f/2, -1.0f/2);

	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(0, 1);
    glVertex3f(1.0f/2,  1.0f/2,  1.0f/2);

    glNormal3f(1.0, 0.0, 0.0);
    glTexCoord2f(1, 0);
    glVertex3f(1.0f/2, -1.0f/2,  1.0f/2);

    glNormal3f(1.0, 0.0, 0.0);
    glTexCoord2f(1, 1);
    glVertex3f(1.0f/2, -1.0f/2, -1.0f/2);

	//back side
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0, 0);
	glVertex3f( 1.0f/2, -1.0f/2, -1.0f/2);

	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0, 1);
    glVertex3f(-1.0f/2, -1.0f/2, -1.0f/2);

    glNormal3f(0.0, 0.0, 1.0);
    glTexCoord2f(1, 0);
    glVertex3f(-1.0f/2,  1.0f/2, -1.0f/2);

    glNormal3f(0.0, 0.0, 1.0);
    glTexCoord2f(1, 1);
    glVertex3f( 1.0f/2,  1.0f/2, -1.0f/2);

	glEnd();
}


void drawXZPlane(float y_intercept, float size){
	glColor3f(0.1,0.1,0.1);
	glLineWidth(1);
	glBegin(GL_QUADS);
		for (int x = 0; x < size-1; x++) { 
      		for (int z = 0; z < size-1; z++) {
		    	//draw quad vertices CCW
		    	//assigning normals as well
		    	//it's a flat x-z plane so the normal is always 1 in the y direction
		       	//glBindTexture(GL_TEXTURE_2D, textures[1]);
		       	glNormal3f(0,1,0);
		       	glTexCoord2f(0, 0);
		        glVertex3f(x, y_intercept, z);

		        glNormal3f(0,1,0);
		        glTexCoord2f(0, 1);
		        glVertex3f(x+1, y_intercept, z);

		        glNormal3f(0,1,0);
		        glTexCoord2f(1, 0);
		        glVertex3f(x+1, y_intercept, z+1);

		        glNormal3f(0,1,0);
		        glTexCoord2f(1, 1);
		        glVertex3f(x, y_intercept, z+1);
      }
    }
    glEnd();
}

void drawWalls(Cell path[][SIZE]){
	for (int x = 0; x < SIZE; x++){
		for (int z= 0; z < SIZE; z++){
			if (!path[x][z].vacant){
				/*glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //TEXTURE_MIN_FILTER*/
				
				glColor3f(1,0,0);
				glPushMatrix();
				glTranslatef(x, 0, z);
				glScalef(1, 1, 1);
				drawCube();
				//glutSolidCube(1);
				glPopMatrix();
			}
		}
	}
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //TEXTURE_MIN_FILTER
	//glutSolidTeapot(1);

}

void drawItems(){
	glDisable(GL_TEXTURE_2D);
	for (int i = 0; i < numItems; i++){
		if(!pickedUp[i]){
			if (i%2) glColor3f(1,0,0);
			else glColor3f(0,1,0);
			glPushMatrix();
			glTranslatef(items[i][0], 0.4, items[i][1]);
			glutSolidSphere(0.2, 10, 10);
			glPopMatrix();
		}
	}
	glEnable(GL_TEXTURE_2D);
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

void drawPenguin(float* pos, float* rot, int frame){
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glRotatef(rot[1], 0, 1, 0);	

	if (animate)
	{	if ((frame)%8==7||(frame)%8==5){
			//glTranslatef(0,0.025,0);
			glRotatef(2,0,0,1);
		}
		if ((frame)%8==6){
			//glTranslatef(0,0.025,0);
			glRotatef(3,0,0,1);
		}
		if ((frame)%8==3||(frame)%8==1){
			//glTranslatef(0,-0.025,0);
			glRotatef(-2,0,0,1);
		}
		if ((frame)%8==2){
			//glTranslatef(0,-0.025,0);
			glRotatef(-2,0,0,1);
		}
	}

	glColor3f(0,0,0.5);
	glPushMatrix();
	glRotatef(90,1,0,0);
	glTranslatef(0,0,-0.5);
	GLUquadric* qobj = gluNewQuadric();
	gluCylinder(qobj, 0.75, 0.75, 1.5, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.5, 0);
	glutSolidSphere(0.75,20,20);

	//translate and draw right eye
	glPushMatrix();
	glColor3f(0,0,0);
	glTranslatef(0.25, 0.2, 0.6);
	glColor3f(0,0,0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	//translate and draw left eye
	glPushMatrix();
	glTranslatef(-0.25, 0.2, 0.6);
	glColor3f(0,0,0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	//translate and draw nose
	glPushMatrix();
	glTranslatef(0, 0.1, 0.7);
	glColor3f(1,0.4,0);
	glutSolidCone(0.1, 0.2, 10, 10);
	glPopMatrix();

	glPopMatrix();//head

	//feet!
	glColor3f(1,0.4,0);
	glPushMatrix();
	glTranslatef(0,-1,0.4);
	if(animate&&(frame%8)<4) glRotatef(-45,1,0,0);
	glTranslatef(0.4,0,0.4);
	glScalef(2,0.1,3);
	glutSolidCube(0.25);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0,-1,0.4);
	if(animate&&(frame%8)>4) glRotatef(-45,1,0,0);
	glTranslatef(-0.4,0,0.4);
	glScalef(2,0.1,3);
	glutSolidCube(0.25);
	glPopMatrix();


	glPopMatrix();//body

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

}

void drawSnowman(float* pos, float* rot, int frame)
{	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glRotatef(rot[1], 0, 1, 0);	
	//gluLookAt(-5,10,10,0,0,0,0,1,0);

	//bouncing animation while the character is moving (movement key held down)
	if (animate)
	{	if ((frame)%8==7||(frame)%8==5){
			glTranslatef(0,0.025,0);
			glScalef(1,1.025,1);
		}
		if ((frame)%8==6){
			glTranslatef(0,0.025,0);
			glScalef(1,1.025,1);
		}
		if ((frame)%8==3||(frame)%8==1){
			glTranslatef(0,-0.025,0);
			glScalef(1,0.925,1);
		}
		if ((frame)%8==2){
			glTranslatef(0,-0.025,0);
			glScalef(1,0.925,1);
		}
	}

	//draw body
	glColor3f(1,1,1);
	glutSolidSphere(1, 16, 16);

	//draw buttons
	glPushMatrix();
	glTranslatef(0, 0.35, 0.9);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.15, 0.95);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.05, 0.95);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();


	glPushMatrix();
	//translate relative to body, and draw head
	glColor3f(1,1,1);
	glTranslatef(0, 1.25, 0);
	glRotatef(headRot[1], 0, 1, 0); //turn the head relative to the body
	glutSolidSphere(0.5, 16, 16);
	
	//translate and draw right eye
	glPushMatrix();
	glColor3f(0,0,0);
	glTranslatef(0.2, 0.15, 0.45);
	glColor3f(0,0,0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	//translate and draw left eye
	glPushMatrix();
	glTranslatef(-0.2, 0.15, 0.45);
	glColor3f(0,0,0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	//translate and draw nose
	glPushMatrix();
	glTranslatef(0, 0, 0.5);
	glColor3f(1,0.4,0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	glPopMatrix();//body

	glPopMatrix();//snowman
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

void drawText(char* text, void* font, int posX, int posY, float scale, float r, float g, float b){
	glPushMatrix();
	glColor3f(r,g,b);
	glTranslatef(posX,posY,0);
	glScalef(scale,scale,0);
		for (int i = 0; i < strlen(text); i++){
			glutStrokeCharacter(font, text[i]);
		}
	glPopMatrix();
	glFlush();
}

// void drawHUD(GLubyte* image, int x, int y, float w, float h){
// 	glRasterPos2i(x,y); 
// 	glPixelZoom(-1, 1); 
// 	glDrawPixels(w,h,GL_RGB, GL_UNSIGNED_BYTE, image); 
// 	glFlush(); 
// }

void drawTime(void* font, int x, int y, float r, float g, float b){
	int currentTime = glutGet(GLUT_ELAPSED_TIME)/1000;
	//string stringTime = to_string(currentTime);

	glColor3f(r,g,b);
	glRasterPos2i(x, y);
	glutBitmapCharacter(font, currentTime);
/*		for (int i = 0; i < 3; i++){
			glutBitmapCharacter(font, minSec[i]);
			if i = 1,
				glutBitmapCharacter(font, ':');
		}*/
}

void draw2D(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 600, 0, 600);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_LIGHTING);


	//drawHUD(img_data, 400, 400, width, height);
	if (win){
		drawText("you win!!!",GLUT_STROKE_ROMAN, 40,100,1,1,0,0);
		drawText("press enter to play again :^)",GLUT_STROKE_ROMAN, 30, 40,0.25,1,0,0);
	}

	drawTime(GLUT_BITMAP_HELVETICA_18, 100, 100, 1, 0 , 0);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glLoadIdentity();
	gluPerspective(45, 1, 1, 100);
	
}

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

	drawItems();
	drawSnowman(pos, rot, frame);
	//drawPenguin(pos, rot, frame);
	draw2D();
	
	//swap buffers - rendering is done to the back buffer, bring it forward to display
	glutSwapBuffers();

	//force a redisplay, to keep the animation running
	glutPostRedisplay();
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
	eye[2] = pos[2] + 10;
	drawSnowman(pos, rot, frame);
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

		case 'y':
		case 'Y':
			if(headRot[1] < 85)
				headRot[1] += 1;
			break;

		case 'u':
		case 'U':
			if(headRot[1] > -85)
				headRot[1] -= 1;
			break;
		case 13: //enter
			if (win) initMaze();
			break;
			
	}
	glutPostRedisplay();
}

void idle(){
	win = checkWin(pickedUp, numItems);
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