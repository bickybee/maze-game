#include <stdio.h>
#include <stdlib.h>

#include <GL/glut.h>

int width = 0;
int height = 0;


GLubyte *image;


void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

	/* set raster position, and flip coordinates to account for data "orientation" in image data array*/
	glRasterPos2i(width,0);
	glPixelZoom(-1, 1);
    
	/* draw the pixels from the image data array */
	glDrawPixels(width,height,GL_RGB, GL_UNSIGNED_BYTE, image);
	glFlush();
}


void myreshape(int h, int w)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLfloat) width, 0.0, (GLfloat) height);
	glMatrixMode(GL_MODELVIEW);
}

GLubyte* LoadPPM(char* file, int* width, int* height, int* max)
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
		printf("%s is not a PPM file!\n",file); 
		exit(0);
	}
	printf("%s is a PPM file\n", file);
	fscanf(fd, "%c",&c);

	/* next, skip past the comments - any line starting with #*/
	while(c == '#') 
	{
		fscanf(fd, "%[^\n] ", b);
		printf("%s\n",b);
		fscanf(fd, "%c",&c);
	}
	ungetc(c,fd); 

	/* now get the dimensions and max colour value from the image */
	fscanf(fd, "%d %d %d", &n, &m, &k);

	printf("%d rows  %d columns  max value= %d\n",n,m,k);

	/* calculate number of pixels and allocate storage for this */
	nm = n*m;
	img = malloc(3*sizeof(GLuint)*nm);
	s=255.0/k;

	/* for every pixel, grab the read green and blue values, storing them in the image data array */
	for(i=0;i<nm;i++) 
	{
		fscanf(fd,"%d %d %d",&red, &green, &blue );
		img[3*nm-3*i-3]=red*s;
		img[3*nm-3*i-2]=green*s;
		img[3*nm-3*i-1]=blue*s;
	}

	/* finally, set the "return parameters" (width, height, max) and return the image array */
	*width = n;
	*height = m;
	*max = k;

	return img;
}

void main(int argc, char *argv[])
{
	int k;

	/* use PPM load function to load the snail - other PPMs also provided so you can test them. 
	 * You can also create your own using the process outlined in the slides 
	 */
	image = LoadPPM("snail_a.ppm", &width, &height, &k);

	/* nothing special down here */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(width, height);
    glutCreateWindow("image");
    glClearColor (1.0, 1.0, 1.0, 1.0);
	glutReshapeFunc(myreshape);
    glutDisplayFunc(display);
    glutMainLoop();

}