//WINTER MAZE
//most of the functions used by display() to draw to the screen

#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <gl/glut.h>
#include <gl/gl.h>
#include <string>
#include <vector>

using namespace std;
#define SIZE 21

//texturing
GLubyte* wall_tex;
GLubyte* floor_tex;
GLubyte* sky;
int width, height, MAX;
GLuint textures[3];

//texture loader, from tutorial!
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

void drawCube(float s) {
	glBegin(GL_QUADS);
	
	//fro1t
    glNormal3f(0.0, 0.0, -1);
    //glBi1dTexture(GL_TEXTURE_2D, textures[0]);
    glTexCoord2f(0, 0);
    glVertex3f( s/2,  s/2, s/2);

    glNormal3f(0.0, 0.0, -1);
    glTexCoord2f(0,1);
    glVertex3f(-s/2,  s/2, s/2);

    glNormal3f(0.0, 0.0, -1);
    glTexCoord2f(1,0);
    glVertex3f(-s/2, -s/2, s/2);

    glNormal3f(0.0, 0.0, -1);
    glTexCoord2f(1,1);
    glVertex3f( s/2, -s/2, s/2);
    
	//top
	glNormal3f(0.0, 1, 0.0);
	glTexCoord2f(0, 0);
	glVertex3f( s/2, s/2, -s/2);

	glNormal3f(0.0, 1, 0.0);
	glTexCoord2f(0,1);
    glVertex3f(-s/2, s/2, -s/2);

    glNormal3f(0.0, 1, 0.0);
    glTexCoord2f(1,0);
    glVertex3f(-s/2, s/2,  s/2);

    glNormal3f(0.0, 1, 0.0);
    glTexCoord2f(1,1);
    glVertex3f( s/2, s/2,  s/2);

	//bottom
	glNormal3f(0.0, -1, 0.0);
	glTexCoord2f(0, 0);
	glVertex3f( s/2, -s/2,  s/2);

	glNormal3f(0.0, -1, 0.0);
	glTexCoord2f(0,1);
    glVertex3f(-s/2, -s/2,  s/2);

    glNormal3f(0.0, -1, 0.0);
    glTexCoord2f(1,0);
    glVertex3f(-s/2, -s/2, -s/2);

    glNormal3f(0.0, -1, 0.0);
    glTexCoord2f(1,1);
    glVertex3f( s/2, -s/2, -s/2);

	//left side
	glNormal3f(-1, 0.0, 0.0);
	glTexCoord2f(0, 0);
	glVertex3f(-s/2,  s/2,  s/2);

	glNormal3f(-1, 0.0, 0.0);
	glTexCoord2f(0,1);
    glVertex3f(-s/2,  s/2, -s/2);

    glNormal3f(-1, 0.0, 0.0);
    glTexCoord2f(1,0);
    glVertex3f(-s/2, -s/2, -s/2);

    glNormal3f(-1, 0.0, 0.0);
    glTexCoord2f(1,1);
    glVertex3f(-s/2, -s/2,  s/2);

	//right side
	glNormal3f(1, 0.0, 0.0);
	glTexCoord2f(0, 0);
	glVertex3f(s/2,  s/2, -s/2);

	glNormal3f(1, 0.0, 0.0);
	glTexCoord2f(0,1);
    glVertex3f(s/2,  s/2,  s/2);

    glNormal3f(1, 0.0, 0.0);
    glTexCoord2f(1,0);
    glVertex3f(s/2, -s/2,  s/2);

    glNormal3f(1, 0.0, 0.0);
    glTexCoord2f(1,1);
    glVertex3f(s/2, -s/2, -s/2);

	//back side
	glNormal3f(0.0, 0.0, 1);
	glTexCoord2f(0, 0);
	glVertex3f( s/2, -s/2, -s/2);

	glNormal3f(0.0, 0.0, 1);
	glTexCoord2f(0,1);
    glVertex3f(-s/2, -s/2, -s/2);

    glNormal3f(0.0, 0.0, 1);
    glTexCoord2f(1,0);
    glVertex3f(-s/2,  s/2, -s/2);

    glNormal3f(0.0, 0.0, 1);
    glTexCoord2f(1,1);
    glVertex3f( s/2,  s/2, -s/2);

	glEnd();
}

//draw the plane!
void drawXZPlane(float y_intercept, float size){
	glColor3f(1,1,1);
	glLineWidth(1);
	glBegin(GL_QUADS);
		for (int x = 0; x < size-1; x++) { 
      		for (int z = 0; z < size-1; z++) {
		    	//draw quad vertices CCW
		    	//assigning normals as well
		    	//it's a flat x-z plane so the normal is always 1 in the y direction
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

//draw walls!
void drawWalls(Cell path[][SIZE]){
	for (int x = 0; x < SIZE; x++){
		for (int z= 0; z < SIZE; z++){
			if (!path[x][z].vacant){
				glColor3f(1,1,1);
				glPushMatrix();
				glTranslatef(x, 0, z);
				glScalef(1, 1, 1);
				drawCube(1);
				glPopMatrix();
			}
		}
	}

}

//draw items!
void drawItems(int** items, bool* pickedUp, int numItems){
	glDisable(GL_LIGHTING);
	for (int i = 0; i < numItems; i++){
		if(!pickedUp[i]){
			glPushMatrix();
			glTranslatef(items[i][0], 0.4, items[i][1]);
			if (i%2) glColor3f(1,0,0);//red
			else glColor3f(0,0.7,0.1);//green
			glutSolidSphere(0.2, 30, 30);
			glRotatef(30,0,0,1);
			glTranslatef(0,0.2,0);
			glColor3f(1,1,0);
			glutSolidCube(0.1);
			glPopMatrix();
		}
	}
	glEnable(GL_LIGHTING);
}

//penguin character!
void drawPenguin(float* pos, float* rot, int frame, bool animate){
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

	glColor3f(0.15,0.15,0.5);
	glPushMatrix();
	glRotatef(90,1,0,0);
	glTranslatef(0,0,-1);
	GLUquadric* qobj = gluNewQuadric();
	gluCylinder(qobj, 0.75, 0.75, 2, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1, 0);
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

//snowman character! modified from tutorial
void drawSnowman(float* pos, float* rot, int frame, bool animate)
{	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glRotatef(rot[1], 0, 1, 0);	

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
	glutSolidSphere(1, 30,30);

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
	glutSolidSphere(0.5, 30,30);
	
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

	//hat!
	glPushMatrix();
	glRotatef(-30,1,0,0); //rotate around head
	glTranslatef(0,0.3,0);

	glPushMatrix();
	glRotatef(-90,1,0,0); //rotate upright
	glColor3f(1,0,0);
	glutSolidCone(0.3,0.8,30,30);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0,0.8,0);
	glColor3f(1,1,1);
	glutSolidSphere(0.1,10,10);
	glPopMatrix();
	glPopMatrix();//hat

	glPopMatrix();//body

	glPopMatrix();//snowman
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

//draw text at given position with given font, size, and color using stroke font
void drawText(char* text, void* font, int posX, int posY, float scale, float r, float g, float b){
	glPushMatrix();
	glColor3f(r,g,b);
	glTranslatef(posX,posY,0);
	glScalef(scale,scale,1);
		for (int i = 0; i < strlen(text); i++){
			glutStrokeCharacter(font, text[i]);
		}
	glPopMatrix();
	glFlush();
}

//draw the current time using bitmap font
void drawTime(void* font, int x, int y, float r, float g, float b, int seconds){
	int digits[] = {48,48,48};
	if (seconds<10) digits[2] += seconds;
	else if (seconds<60){
		digits[2] += seconds % 10;
		digits[1] += seconds / 10;
	}
	else{
		digits[2] += seconds % 60 % 10;
		digits[1] += seconds / 10 % 6;
		digits[0] += seconds / 60;
	}

	glColor3f(r,g,b);
	glRasterPos2i(x, y);
		for (int i = 0; i < 3; i++){
			glutBitmapCharacter(font, digits[i]);
			if (i == 0) glutBitmapCharacter(font, ':');
		}
}

//sets up matrices for 2D drawing
//calls 2D drawing functions (didn't end up putting in more than just fonts)
void draw2D(bool win, int seconds, int pickedUp){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 600, 0, 600);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_LIGHTING);


	drawTime(GLUT_BITMAP_HELVETICA_18, 20, 20, 1, 0 , 0, seconds);
	if (win){
		drawText("you win!!!",GLUT_STROKE_ROMAN, 40,100,1,1,0,0);
		drawText("you win!!!",GLUT_STROKE_ROMAN, 45,105,1,0,0,1);
		drawText("you win!!!",GLUT_STROKE_ROMAN, 35,95,1,0,1,0);
		drawText("press enter to play again :^)",GLUT_STROKE_ROMAN, 30, 40,0.25,1,0,0);
	}

	else {
		char* count = new char[3];
		count[0] = pickedUp+48;
		count[1] = '/';
		count[2] = 57;
		count[3] = '\0';
		drawText(count,GLUT_STROKE_ROMAN, 60, 20, 0.1, 1, 0, 0);
	}

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glLoadIdentity();
	gluPerspective(45,1,0.1,500);
	
}

//initialize random locations for snow
//within x y z bounds
void initializeSnow(float** snows, int numSnows, int x, int y, int z){
	for (int i = 0; i < numSnows; i++){
		float* coords = new float[3];
		coords[0] = rand() % x;
		coords[1] = rand() % y;
		coords[2] = rand() % z;
		snows[i] = coords;
	}
}

//update snow that's fallen to the ground to start at the top again
//according to x y z bounds
void updateSnow(float** snows, int numSnows, int x, int y, int z){
	for (int i = 0; i < numSnows; i++){
		snows[i][1] = snows[i][1] - 0.1;
		if (snows[i][1] < -1){
			snows[i][0] = rand() % x;
			snows[i][1] = y;
			snows[i][2] = rand() % z;
		}
	}
}

//draw all the snow!
void drawSnow(float** snows, int numSnows){
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glColor3f(1,1,1);
	for (int i = 0; i < numSnows; i++){
		glPushMatrix();
		glTranslatef(snows[i][0]-5,snows[i][1],snows[i][2]-5);
		glutSolidSphere(0.1,5,5);
		glPopMatrix();
	}
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}

//draw the skybox
void drawSkyBox(float s, float n){
	
	//front
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glBegin(GL_QUADS);

    glNormal3f(0.0, 0.0, -n);
    glTexCoord2f(1,1);
    glVertex3f( s,  s, s);

    glNormal3f(0.0, 0.0, -n);
    glTexCoord2f(1,0);
    glVertex3f( s, -s, s);

    glNormal3f(0.0, 0.0, -n);
    glTexCoord2f(0,1);
    glVertex3f(-s, -s, s);

    glNormal3f(0.0, 0.0, -n);
    glTexCoord2f(0,0);
    glVertex3f(-s,  s, s);

    
	//top

    glNormal3f(0.0, n, 0.0);
    glTexCoord2f(1,1);
    glVertex3f( s, s,  s);

	glNormal3f(0.0, n, 0.0);
	glTexCoord2f(0, 0);
	glVertex3f( s, s, -s);

	glNormal3f(0.0, n, 0.0);
	glTexCoord2f(0,1);
    glVertex3f(-s, s, -s);

    glNormal3f(0.0, n, 0.0);
    glTexCoord2f(1,0);
    glVertex3f(-s, s,  s);


	//bottom

	glNormal3f(0.0, -n, 0.0);
	glTexCoord2f(0, 0);
	glVertex3f( s, -s,  s);

    glNormal3f(0.0, -n, 0.0);
    glTexCoord2f(1,1);
    glVertex3f( s, -s, -s);

    glNormal3f(0.0, -n, 0.0);
    glTexCoord2f(1,0);
    glVertex3f(-s, -s, -s);

	glNormal3f(0.0, -n, 0.0);
	glTexCoord2f(0,1);
    glVertex3f(-s, -s,  s);


	//left side
	glNormal3f(-n, 0.0, 0.0);
	glTexCoord2f(0, 0);
	glVertex3f(-s,  s,  s);

    glNormal3f(-n, 0.0, 0.0);
    glTexCoord2f(1,1);
    glVertex3f(-s, -s,  s);

    glNormal3f(-n, 0.0, 0.0);
    glTexCoord2f(1,0);
    glVertex3f(-s, -s, -s);

	glNormal3f(-n, 0.0, 0.0);
	glTexCoord2f(0,1);
    glVertex3f(-s,  s, -s);


	//right side
	glNormal3f(n, 0.0, 0.0);
	glTexCoord2f(0, 0);
	glVertex3f(s,  s, -s);

	glNormal3f(n, 0.0, 0.0);
    glTexCoord2f(1,1);
    glVertex3f(s, -s, -s);

    glNormal3f(n, 0.0, 0.0);
    glTexCoord2f(1,0);
    glVertex3f(s, -s,  s);

	glNormal3f(n, 0.0, 0.0);
	glTexCoord2f(0,1);
    glVertex3f(s,  s,  s);


	//back side
	glNormal3f(0.0, 0.0, n);
	glTexCoord2f(0, 0);
	glVertex3f( s, -s, -s);

	glNormal3f(0.0, 0.0, n);
    glTexCoord2f(1,1);
    glVertex3f( s,  s, -s);

    glNormal3f(0.0, 0.0, n);
    glTexCoord2f(1,0);
    glVertex3f(-s,  s, -s);

	glNormal3f(0.0, 0.0, n);
	glTexCoord2f(0,1);
    glVertex3f(-s, -s, -s);
	glEnd();
}