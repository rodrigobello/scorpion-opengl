#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "RgbImage.h"

#define PI 3.141592654

using namespace std;

char* fileNameTexScorpion = "./textures/dragonSkin.bmp";
char* filenameTexTop = "./textures/woodTextureTop.bmp";
char* filenameTexFront = "./textures/woodTextureFront.bmp";
char* filenameTexSide = "./textures/woodTextureSide.bmp";

GLuint _textureIdScorpion;
GLuint _textureIdSphere;
GLuint _textureIdCylinder;
GLuint _textureIdPlatformTop;
GLuint _textureIdPlatformFront;
GLuint _textureIdPlatformSide;

bool textureOn = true;

GLUquadric *quadSphere;
GLUquadric *quadCylinder;

float bodyDiameter = 2.2;

float xScaleBody = 0.6;
float yScaleBody = 0.8;
float zScaleBody = 2;

float diameterCylinder = 0.3;
float diameterSphere = 0.4;
float diameterTailCylinder = 0.5;
float diameterTailSphere = 0.7;
float sizeArm = 4.5;
float sizeLeg = 3.2;
float sizeTail = 1.5;
float sizeForearm = 3.0;
float sizeForeleg = 2.0;
float sizeHand = 2.0;
float sizeFoot = 3.3;
float sizeClampPart = 1.0;
float lenghtPlatform = 10.0;
float heightPlatform = 0.4;
float diameterBase = 2.0;
float heightBase = 0.5;

float eyeDistance = 30.0;
float eyeX;
float eyeY;
float eyeZ;
float viewAngleX = 0.0;
float viewAngleZ = 15.0;


float angleBum = -40.0;
float angleTail = 30.0;
float angleClaw = 30.0;
float angleArm = 90.0;
float angleLeg = 65.0;
float angleForeleg = 25.0;
float angleForearm = 110.0;
float angleFoot = 90.0;
float angleHand = 0.0;
float angleClampZ = 0.0;
float angleClampY = 0.0;

GLfloat lightposition[4] = { 0.0f, 30.0f, 0.0f, 0.0 };
GLfloat luzAmbiente[4] = { 0.19, 0.19, 0.19, 0.0 };
GLfloat luzDifusa[4] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat luzEspecular[4] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat especularidade[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat semEspecularidade[4] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat especMaterialTexture = 50;
GLfloat especMaterialNoTexture = 60;

//defines light source (LIGHT0)
void initLighting(void)
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    // Especifica que a cor de fundo da janela ser� preta
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Habilita o modelo de coloriza��o de Gouraud
    glShadeModel(GL_SMOOTH);

    // Ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
    //glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

    // Define os par�metros da luz de n�mero 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

    // Materials will follow current color
    glEnable(GL_COLOR_MATERIAL);
}

//makes the image into a texture, and returns the id of the texture
GLuint loadTexture(char *filename) {
    GLuint textureId;

    RgbImage theTexMap(filename); //Image with texture

    glGenTextures(1, &textureId); //Make room for our texture
    glBindTexture(GL_TEXTURE_2D, textureId);	//Tell OpenGL which texture to edit
                                                //Map the image to the texture
    glTexImage2D(GL_TEXTURE_2D,	//Always GL_TEXTURE_2D
        0,						//0 for now
        GL_RGB,					//Format OpenGL uses for image
        theTexMap.GetNumCols(),	//Width
        theTexMap.GetNumRows(),	//Height
        0,						//The border of the image
        GL_RGB,					//GL_RGB, because pixels are stored in RGB format
        GL_UNSIGNED_BYTE,		//GL_UNSIGNED_BYTE, because pixels are stored as unsigned numbers
        theTexMap.ImageData());	//The actual pixel data
    return textureId; //Returns the id of the texture
}

void initRendering(void) {

    quadSphere = gluNewQuadric();
    quadCylinder = gluNewQuadric();
    _textureIdScorpion = loadTexture(fileNameTexScorpion);
    _textureIdCylinder = _textureIdScorpion;
    _textureIdSphere = _textureIdScorpion;
    _textureIdPlatformTop = loadTexture(filenameTexTop);
    _textureIdPlatformFront = loadTexture(filenameTexFront);
    _textureIdPlatformSide = loadTexture(filenameTexSide);
}

void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
    case 27: //Escape key
        exit(0);
    case 'w': //Increase view angle z axis
        if (viewAngleZ < 180) viewAngleZ += 3;
        glutPostRedisplay();
        break;
    case 's': //Decrease view angle z axis
        if (viewAngleZ > 0) viewAngleZ -= 3;
        glutPostRedisplay();
        break;
    case 'a': //Decrease view angle x axis
        if (viewAngleX > 0) viewAngleX -= 3;
        glutPostRedisplay();
        break;
    case 'd': //Increase view angle x axis
        if (viewAngleX < 180) viewAngleX += 3;
        glutPostRedisplay();
        break;
    case '1': //Increase forearm angle
        if (angleForearm < 120) angleForearm += 3;
        glutPostRedisplay();
        break;
    case '2': //Decrease forearm angle
        if (angleForearm > 60) angleForearm -= 3;
        glutPostRedisplay();
        break;
    case '3': //Increase hand angle
        if (angleClaw < 90) angleClaw += 3;
        glutPostRedisplay();
        break;
    case '4': //Decrease hand angle
        if (angleClaw > 30) angleClaw -= 3;
        glutPostRedisplay();
        break;
    case '5':
        if (angleTail < 50) angleTail += 3;
        glutPostRedisplay();
        break;
    case '6':
        if (angleTail > 10) angleTail -= 3;
        glutPostRedisplay();
        break;
    case '7':
        if(angleBum < -30) angleBum +=3;
        glutPostRedisplay();
        break;
    case '8':
        if(angleBum > -60) angleBum -=3;
        glutPostRedisplay();
        break;
    case '9':
        if(angleFoot < 90) angleFoot += 3;
        glutPostRedisplay();
        break;
    case '0':
        if(angleFoot > 75) angleFoot -= 3;
        glutPostRedisplay();
        break;
    case '-':
        if(angleForeleg < 30) angleForeleg += 3;
        glutPostRedisplay();
        break;
    case '=':
        if(angleForeleg > 10) angleForeleg -= 3;
        glutPostRedisplay();
        break;
    }
}

void GerenciaMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
        if (state == GLUT_DOWN) {  // Zoom-out
            if (eyeDistance < 30) eyeDistance += 1;
        }
    if (button == GLUT_RIGHT_BUTTON)
        if (state == GLUT_DOWN) {  // Zoom-out
            if (eyeDistance > 0) eyeDistance -= 1;
        }
    glutPostRedisplay();
}

void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / (float)h, 1.0, 50.0);
}

void drawCube(float lenghtX, float lenghtY, float height)
{
    glBindTexture(GL_TEXTURE_2D, _textureIdPlatformFront);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);			// Face posterior
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-lenghtX, -lenghtY, -height);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-lenghtX, lenghtY, -height);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(lenghtX, lenghtY, -height);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(lenghtX, -lenghtY, -height);
    glEnd();
    glBegin(GL_QUADS);			// Face frontal
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-lenghtX, -lenghtY, height);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(lenghtX, -lenghtY, height);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(lenghtX, lenghtY, height);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-lenghtX, lenghtY, height);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, _textureIdPlatformSide);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);			// Face lateral esquerda
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-lenghtX, -lenghtY, -height);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-lenghtX, -lenghtY, height);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-lenghtX, lenghtY, height);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-lenghtX, lenghtY, -height);
    glEnd();
    glBegin(GL_QUADS);			// Face lateral direita
    glTexCoord2f(1.0f, 0.0f); glVertex3f(lenghtX, -lenghtY, -height);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(lenghtX, lenghtY, -height);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(lenghtX, lenghtY, height);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(lenghtX, -lenghtY, height);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, _textureIdPlatformTop);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);			// Face superior
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-lenghtX, lenghtY, -height);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-lenghtX, lenghtY, height);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(lenghtX, lenghtY, height);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(lenghtX, lenghtY, -height);
    glEnd();
    glBegin(GL_QUADS);			// Face inferior
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-lenghtX, -lenghtY, -height);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(lenghtX, -lenghtY, -height);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(lenghtX, -lenghtY, height);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-lenghtX, -lenghtY, height);
    glEnd();
}

void drawCylinder(float diameter, float lenght) {
    if (textureOn) {
        glBindTexture(GL_TEXTURE_2D, _textureIdCylinder);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        gluQuadricTexture(quadCylinder, 1);
    }
    else
        gluQuadricTexture(quadCylinder, 0);
    gluCylinder(quadCylinder, diameter, diameter, lenght, 40.0, lenght*30.0);
}

void drawCone(float diameter, float lenght) {
    if (textureOn) {
        glBindTexture(GL_TEXTURE_2D, _textureIdCylinder);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        gluQuadricTexture(quadCylinder, 1);
    } else
        gluQuadricTexture(quadCylinder, 0);
    gluCylinder(quadCylinder, diameter, 0, lenght, 40.0, lenght*30.0);
}

void drawDisk(float diameterInner, float diameterOuter) {
    if (textureOn) {
        glBindTexture(GL_TEXTURE_2D, _textureIdCylinder);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        gluQuadricTexture(quadCylinder, 1);
    }
    else
        gluQuadricTexture(quadCylinder, 0);
    gluDisk(quadCylinder, diameterInner, diameterOuter, 40.0, 30.0);
}

void drawSphere(float diameter) {
    if (textureOn) {
        glBindTexture(GL_TEXTURE_2D, _textureIdSphere);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        gluQuadricTexture(quadSphere, 1);
    } else
        gluQuadricTexture(quadSphere, 0);
    gluSphere(quadSphere, diameter, 40.0, 40.0);
}


void drawArm(char p) {
    glPushMatrix();

    if (p == 'r')
        glRotatef(angleArm, 1.0f, 0.0f, 0.0f);
    else
        glRotatef(angleArm*-1, 1.0f, 0.0f, 0.0f);

    //draws the arm
    drawCylinder(diameterCylinder, sizeArm);

    // move to forearm referential
    glTranslatef(0.0f, 0.0f, sizeArm + diameterSphere / 5);

    glRotatef(angleForearm, 0.0f, 1.0f, 0.0f);

    //draws the forearm
    drawSphere(diameterSphere);
    glTranslatef(0.0f, 0.0f, diameterSphere / 5);
    drawCylinder(diameterCylinder, sizeForearm);

    //move to hand referential
    glTranslatef(0.0f, 0.0f,  sizeForearm + diameterSphere / 5);
    glRotatef(angleHand, 0.0f, 1.0f, 0.0f);

    //draws the clamp sphere
    drawSphere(diameterSphere);

    glPushMatrix();

    //draws top part of clamp
    glRotatef(-1*angleClaw, 0.0f, 1.0f, 0.0f);

    drawCylinder(diameterCylinder / 3, sizeClampPart);
    glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
    drawSphere(diameterSphere / 3);

    glTranslatef(0.0f, 0.0f, diameterSphere / 15);
    glRotatef(60.0, 0.0f, 1.0f, 0.0f);

    drawCone(diameterCylinder / 3, sizeClampPart);

    glPopMatrix();

    //draws bottom part of clamp
    glRotatef(angleClaw, 0.0f, 1.0f, 0.0f);

    drawCylinder(diameterCylinder / 3, sizeClampPart);
    glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
    drawSphere(diameterSphere / 3);

    glTranslatef(0.0f, 0.0f, diameterSphere / 15);
    glRotatef(-60.0, 0.0f, 1.0f, 0.0f);

    drawCone(diameterCylinder / 3, sizeClampPart);

    glPopMatrix();

}


void drawLeg(char p) {

    glPushMatrix();

    if (p == 'r')
        glRotatef(angleLeg, 1.0f, 0.0f, 0.0f);
    else
        glRotatef(angleLeg*-1, 1.0f, 0.0f, 0.0f);

    drawCylinder(diameterCylinder*0.75, sizeLeg);

    glTranslatef(0.0f, 0.0f, sizeLeg + (diameterSphere*0.75) / 5);

    if (p == 'r')
        glRotatef(angleForeleg, 1.0f, 0.0f, 0.0f);
    else
        glRotatef(angleForeleg*-1, 1.0f, 0.0f, 0.0f);

    drawSphere(diameterSphere*0.75);
    glTranslatef(0.0f, 0.0f, (diameterSphere*0.75) / 5);
    drawCylinder(diameterCylinder*0.75, sizeForeleg);

    glTranslatef(0.0f, 0.0f,  sizeForeleg + (diameterSphere*0.75) / 5);

    if (p == 'r')
        glRotatef(angleFoot, 1.0f, 0.0f, 0.0f);
    else
        glRotatef(angleFoot*-1, 1.0f, 0.0f, 0.0f);

    drawSphere(diameterSphere*0.75);
    glTranslatef(0.0f, 0.0f, (diameterSphere*0.75) / 5);
    drawCone(diameterCylinder*0.75, sizeFoot);

    glPopMatrix();

}

void drawTail() {
    glPushMatrix();
        drawSphere(diameterTailSphere);

        glRotatef(angleBum, 0.0f, 1.0f, 0.0f);

        glTranslatef(0.0f, 0.0f, diameterSphere / 2);

        drawCylinder(diameterTailCylinder, sizeTail);

        glTranslatef(0.0f, 0.0f, sizeTail);

        drawSphere(diameterTailSphere);

        for (int i = 0;i<3;i++) {
            glRotatef(angleTail, 0.0f, 1.0f, 0.0f);

            glTranslatef(0.0f, 0.0f, diameterSphere / 2);

            drawCylinder(diameterTailCylinder, sizeTail);

            glTranslatef(0.0f, 0.0f, sizeTail);

            drawSphere(diameterTailSphere);
        }

        glRotatef(angleTail, 0.0f, 1.0f, 0.0f);

        glTranslatef(0.0f, 0.0f, diameterTailSphere / 2);

        drawCone(diameterTailCylinder, sizeTail);


    glPopMatrix();
}

void drawScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    eyeX = eyeDistance * cos(viewAngleZ*PI / 180) * cos(viewAngleX*PI / 180);
    eyeY = eyeDistance * cos(viewAngleZ*PI / 180) * sin(viewAngleX*PI / 180);
    eyeZ = eyeDistance * sin(viewAngleZ*PI / 180);
    if (viewAngleZ<90)
        gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    else
        gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);

    glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

    // clears color and material parameters before drawing platform
    glColor3f(1.0f, 1.0f, 1.0f);
    glMaterialfv(GL_FRONT, GL_SPECULAR, semEspecularidade);
    glMateriali(GL_FRONT, GL_SHININESS, 0);

    // draws the platform
    //glPushMatrix();
    //glScalef(lenghtPlatform, lenghtPlatform, heightPlatform);
    drawCube(lenghtPlatform, lenghtPlatform, heightPlatform);
    //glPopMatrix();

    if (textureOn) {
        glColor3f(0.9f, 0.9f, 0.9f);
        glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
        glMateriali(GL_FRONT, GL_SHININESS, especMaterialTexture);
    }
    else {
        glColor3f(0.8f, 0.9f, 1.0f);
        glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
        glMateriali(GL_FRONT, GL_SHININESS, especMaterialNoTexture);
    }

    // move to base referential
    glTranslatef(0.0f, 0.0f, heightPlatform + bodyDiameter/2);


    // draw body
    glPushMatrix();

        glRotatef(90.0, 0.0f, 1.0f, 0.0f);
        glScalef(xScaleBody, yScaleBody, zScaleBody);
        drawSphere(bodyDiameter);

    glPopMatrix();


    // draw arms
    glPushMatrix();

        glTranslatef(bodyDiameter*(xScaleBody+1.0), 0.0f, 0.0f);

        drawArm('r');
        drawArm('l');

    glPopMatrix();

    // draw legs
    glPushMatrix();

        glPushMatrix();
        glTranslatef(3*(bodyDiameter*(xScaleBody+1.0))/4, 0.0f, 0.0f);
        drawLeg('r');
        glPopMatrix();

        glPushMatrix();
        glTranslatef((bodyDiameter*(xScaleBody+1.0))/4, 0.0f, 0.0f);
        drawLeg('r');
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-1*(bodyDiameter*(xScaleBody+1.0))/4, 0.0f, 0.0f);
        drawLeg('r');
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-3*(bodyDiameter*(xScaleBody+1.0))/4, 0.0f, 0.0f);
        drawLeg('r');
        glPopMatrix();

        glPushMatrix();
        glTranslatef(3*(bodyDiameter*(xScaleBody+1.0))/4, 0.0f, 0.0f);
        drawLeg('l');
        glPopMatrix();

        glPushMatrix();
        glTranslatef((bodyDiameter*(xScaleBody+1.0))/4, 0.0f, 0.0f);
        drawLeg('l');
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-1*(bodyDiameter*(xScaleBody+1.0))/4, 0.0f, 0.0f);
        drawLeg('l');
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-3*(bodyDiameter*(xScaleBody+1.0))/4, 0.0f, 0.0f);
        drawLeg('l');
        glPopMatrix();

    glPopMatrix();

    glTranslatef(0.0f, 0.0f, bodyDiameter/3);

    glTranslatef(-1*bodyDiameter*(xScaleBody+1.0), 0.0f, 0.0f);

    drawTail();


    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Clamp");

    initLighting();
    initRendering();
    glutDisplayFunc(drawScene);
    glutKeyboardFunc(handleKeypress);
    glutMouseFunc(GerenciaMouse);
    glutReshapeFunc(handleResize);

    glutMainLoop();
    return 0;
}
