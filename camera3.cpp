#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <gl/glut.h>
#include <gl/gl.h>
#include <vector>
#include <math.h>

struct Vector3
{
    float x, y, z;
};

struct ThirdPersonCamera_t
{
    struct Vector3 vecPos;
    struct Vector3 vecRot;
    float fRadius;          // Distance between the camera and the object.
    float fLastX;
    float fLastY;
};

struct ThirdPersonCamera_t camera;
GLuint floor_tex = 0;

//render floor so I can tell if I'm moving
void floor_render(float size, float y) {
    glTranslatef(0.0f, y, 0.0f);

    //weird things happen if you enable lighting. So don't
    glDisable(GL_LIGHTING);
    
    //draw the floor
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f( 0.0f, 0.0f );     glVertex3f(-size, y, -size);
    glTexCoord2f( 0.0f, 1.0f );     glVertex3f(-size, y, +size);
    glTexCoord2f( 1.0f, 1.0f );     glVertex3f(+size, y, +size);
    glTexCoord2f( 1.0f, 0.0f );     glVertex3f(+size, y, -size);
    glEnd();
}


int InitScene(void) {
    //depth test. you know, so the cube's actually on the plane 
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearDepth( 1.0f );

   memset(&camera, 0, sizeof( struct ThirdPersonCamera_t));
    camera.fRadius = 10.0f;
}

void DisplayFunction(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();

    //position camera, draw cube
    glTranslatef(0.0f, -2.0f, -camera.fRadius);
    glRotatef(camera.vecRot.x, 1.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidCube(3.0f);

    //rotate camera
    glRotatef(camera.vecRot.y, 0.0f, 1.0f, 0.0f);
    glTranslatef(-camera.vecPos.x, 0.0f, -camera.vecPos.z);

    floor_render(50.0f, -0.75f);

    glutSwapBuffers();
}

void IdleFunction(void) {
    //show stuff even when its not doing anything
    glutPostRedisplay();
}

void KeyboardFunction(GLubyte k, int x, int y) {
    static float fRotSpeed = 1.0f;

    if (k=='q') {
        camera.vecRot.y += fRotSpeed;
        if (camera.vecRot.y > 360) camera.vecRot.y -= 360;
    }
    if (k=='z') {
        camera.vecRot.y -= 1;
        if (camera.vecRot.y < -360) camera.vecRot.y += 360;
    }
    if (k=='w') {
        float xrotrad, yrotrad;
        yrotrad = (camera.vecRot.y / 180.0f * 3.141592654f);
        xrotrad = (camera.vecRot.x / 180.0f * 3.141592654f); 
        camera.vecPos.x += (float)(sin(yrotrad));
        camera.vecPos.z -= (float)(cos(yrotrad));
        camera.vecPos.y -= (float)(sin(xrotrad));
    }
    if (k=='s') {
        float xrotrad, yrotrad;
        yrotrad = (camera.vecRot.y / 180.0f * 3.141592654f);
        xrotrad = (camera.vecRot.x / 180.0f * 3.141592654f); 
        camera.vecPos.x -= (float)(sin(yrotrad));
        camera.vecPos.z += (float)(cos(yrotrad));
        camera.vecPos.y += (float)(sin(xrotrad));
    }
    if (k=='d') {
        float yrotrad;
        yrotrad = (camera.vecRot.y / 180.0f * 3.141592654f);
        camera.vecPos.x += (float)(cos(yrotrad)) * 0.5f;
        camera.vecPos.z += (float)(sin(yrotrad)) * 0.5f;
    }
    if (k =='a') {
        float yrotrad;
        yrotrad = (camera.vecRot.y / 180.0f * 3.141592654f);
        camera.vecPos.x -= (float)(cos(yrotrad)) * 0.5f;
        camera.vecPos.z -= (float)(sin(yrotrad)) * 0.5f;
    }

    //quit when you press escape
    if(k == 27) {
        exit(0);
    }
    glutPostRedisplay();
}

void MouseFunction(int x, int y) {
    int diffx = x - camera.fLastX;
    int diffy = y - camera.fLastY;

    camera.fLastX = x;
    camera.fLastY = y;

    camera.vecRot.x += (float) diffy;
    camera.vecRot.y += (float) diffx;

    if(camera.vecRot.x < -30.0f)
        camera.vecRot.x = -30.0f;
    if(camera.vecRot.x > 90.0f)
        camera.vecRot.x = 90.0f;
}

void ReshapeFunction(GLsizei width, GLsizei height) {
    glViewport( 0, 0, width, height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    gluPerspective( 45.0f, (GLdouble) width/height, 0.1f, 500.0f );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

int main(int argc, char* argv[]) {
    glutInit( &argc, argv );
    glutInitWindowSize( 640, 480 );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA );
    glutCreateWindow( "Camera" );

    InitScene();
    glutIdleFunc(IdleFunction);
    glutDisplayFunc(DisplayFunction);
    glutKeyboardFunc(KeyboardFunction);
    glutReshapeFunc(ReshapeFunction);
    glutPassiveMotionFunc(MouseFunction);
    glutMainLoop();

    return 0;
}