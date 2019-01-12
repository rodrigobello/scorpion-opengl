#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "RgbImage.h"

#define PI 3.141592654

using namespace std;

char *skinTexture = "textures/dragonSkin.bmp";

GLuint _textureId = NULL;

GLUquadric *quadSphere;
GLUquadric *quadCylinder;

// float diameterCylinder = 0.3;
// float diameterSphere = 0.4;
float rightPaws[4] = {0.0, 0.0, 0.0, 0.0};
float leftPaws[4] = {0.0, 0.0, 0.0, 0.0};
float leftArm = 0.0;
float leftClaw = 30.0;
float rightArm = 0.0;
float rightClaw = 30.0;
float tail = 30.0;

float eyeDistance = 30.0;
float eyeX;
float eyeY;
float eyeZ;
float viewAngleX = 0.0;
float viewAngleZ = 15.0;

GLfloat lightposition[4] = {50.0f, 0.0f, 0.0f, 0.0};
GLfloat luzAmbiente[4] = {0.19, 0.19, 0.19, 0.0};
GLfloat luzDifusa[4] = {0.5, 0.5, 0.5, 1.0};
GLfloat luzEspecular[4] = {0.5, 0.5, 0.5, 1.0};
GLfloat especularidade[4] = {1.0, 1.0, 1.0, 1.0};
GLfloat semEspecularidade[4] = {0.0, 0.0, 0.0, 1.0};
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

	// Especifica que a cor de fundo da janela será preta
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Ativa o uso da luz ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

	// Materiais irão seguir a cor atual
	glEnable(GL_COLOR_MATERIAL);
}

//makes the image into a texture, and returns the id of the texture
GLuint loadTexture(char *filename)
{
	GLuint textureId;

	RgbImage theTexMap(filename); //Image with texture

	glGenTextures(1, &textureId);			 //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
											 //Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,				 //Always GL_TEXTURE_2D
				 0,							 //0 for now
				 GL_RGB,					 //Format OpenGL uses for image
				 theTexMap.GetNumCols(),	 //Width
				 theTexMap.GetNumRows(),	 //Height
				 0,							 //The border of the image
				 GL_RGB,					 //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE,			 //GL_UNSIGNED_BYTE, because pixels are stored as unsigned numbers
				 theTexMap.ImageData());	 //The actual pixel data

	return textureId; //Returns the id of the texture
}

void initRendering(void)
{
	quadSphere = gluNewQuadric();
	quadCylinder = gluNewQuadric();
	_textureId = loadTexture(skinTexture);
}

// void handleKeypress(unsigned char key, int x, int y) {
// 	switch (key) {
// 	case 27: //Escape key
// 		exit(0);
// 	case 'w': //Increase view angle z axis
// 		if (viewAngleZ < 180) viewAngleZ += 3;
// 		glutPostRedisplay();
// 		break;
// 	case 'z': //Decrease view angle z axis
// 		if (viewAngleZ > 0) viewAngleZ -= 3;
// 		glutPostRedisplay();
// 		break;
// 	case 'a': //Decrease view angle x axis
// 		if (viewAngleX > 0) viewAngleX -= 3;
// 		glutPostRedisplay();
// 		break;
// 	case 's': //Increase view angle x axis
// 		if (viewAngleX < 180) viewAngleX += 3;
// 		glutPostRedisplay();
// 		break;
// 	case 't': //Change metal texture
// 		texturePresent = true;
// 		if (_textureIdSphere == _textureIdMetal1) {
// 			_textureIdSphere = _textureIdMetal2;
// 			_textureIdCylinder = _textureIdMetal2;
// 		} else if (_textureIdSphere == _textureIdMetal2) {
// 			_textureIdSphere = _textureIdMetal3;
// 			_textureIdCylinder = _textureIdMetal3;
// 		} else if (_textureIdSphere == _textureIdMetal3) {
// 			_textureIdSphere = _textureIdMetal4;
// 			_textureIdCylinder = _textureIdMetal4;
// 		} else if (_textureIdSphere == _textureIdMetal4) {
// 			_textureIdSphere = _textureIdMetal1;
// 			_textureIdCylinder = _textureIdMetal1;
// 		}
// 		glutPostRedisplay();
// 		break;
// 	case 'n': //no texture
// 		texturePresent = false;
// 		glutPostRedisplay();
// 		break;
// 	case '1': //Increase arm angle
// 		angleArm += 3;
// 		if (angleArm >= 360) angleArm = 0;
// 		glutPostRedisplay();
// 		break;
// 	case '2': //Decrease arm angle
// 		angleArm -= 3;
// 		if (angleArm <= 0) angleArm = 360;
// 		glutPostRedisplay();
// 		break;
// 	case '3': //Increase forearm angle
// 		if (angleForearm < 90) angleForearm += 3;
// 		glutPostRedisplay();
// 		break;
// 	case '4': //Decrease forearm angle
// 		if (angleForearm > -90) angleForearm -= 3;
// 		glutPostRedisplay();
// 		break;
// 	case '5': //Increase hand angle
// 		if (angleHand < 90) angleHand += 3;
// 		glutPostRedisplay();
// 		break;
// 	case '6': //Decrease hand angle
// 		if (angleHand > -90) angleHand -= 3;
// 		glutPostRedisplay();
// 		break;
// 	case '7': //Increase clamp angle y axis
// 		if (angleClampY < 60) angleClampY += 3;
// 		glutPostRedisplay();
// 		break;
// 	case '8': //Decrease clamp angle y axis
// 		if (angleClampY > 0) angleClampY -= 3;
// 		glutPostRedisplay();
// 		break;
// 	case '9': //Increase clamp angle z axis
// 		angleClampZ += 3;
// 		if (angleClampZ >= 360) angleClampZ = 0;
// 		glutPostRedisplay();
// 		break;
// 	case '0': //Decrease clamp angle z axis
// 		angleClampZ -= 3;
// 		if (angleClampZ <= 0) angleClampZ = 360;
// 		glutPostRedisplay();
// 		break;
// 	}
// 	//cout << "Arm (" << angleArm << "); Forearm (" << angleForearm << "); Hand (" << angleHand << "); ";
// 	//cout << "Clamp Y (" << angleClampY << "); Clamp Z (" << angleClampZ << ") \n";
// 	//cout << "Eye X (" << eyeX << "); Eye Y (" << eyeY << "); Eye Z (" << eyeZ << ") \n";
// }

// // Fun��o callback chamada para gerenciar eventos do mouse
// void GerenciaMouse(int button, int state, int x, int y)
// {
// 	if (button == GLUT_LEFT_BUTTON)
// 		if (state == GLUT_DOWN) {  // Zoom-out
// 			if (eyeDistance < 30) eyeDistance += 1;
// 		}
// 	if (button == GLUT_RIGHT_BUTTON)
// 		if (state == GLUT_DOWN) {  // Zoom-out
// 			if (eyeDistance > 0) eyeDistance -= 1;
// 		}
// 	glutPostRedisplay();
// }

void handleResize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)w / (float)h, 1.0, 50.0);
}

void drawCylinder(float diameter, float lenght, GLuint _textureId)
{
	if (_textureId)
	{
		glBindTexture(GL_TEXTURE_2D, _textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
	{
		gluQuadricTexture(quadCylinder, 0);
	}
	gluCylinder(quadCylinder, diameter, diameter, lenght, 40.0, lenght * 30.0);
}

void drawSphere(float diameter, GLuint _textureId)
{
	if (_textureId)
	{
		glBindTexture(GL_TEXTURE_2D, _textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadSphere, 1);
	}
	else
	{
		gluQuadricTexture(quadSphere, 0);
	}
	gluSphere(quadSphere, diameter, 40.0, 40.0);
}

void drawBody(GLuint _textureId)
{
	glColor3f(0.4f, 0.3f, 0.0f);

	glPushMatrix();
	glScalef(0.6, 0.8, 2);
	drawSphere(4.0, _textureId);
	glPopMatrix();

	//eyes
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(2, -1, -4.2);
	gluSphere(quadSphere, 0.25, 10.0, 10.0);
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(2, 1, -4.2);
	gluSphere(quadSphere, 0.25, 10.0, 10.0);

	//teeths
	glColor3f(0.8, 0.45, 0.1);
	glTranslatef(-1.5, 0, -3);
	drawSphere(0.7, _textureId);
	glColor3f(0.8, 0.45, 0.1);
	glTranslatef(-1.5, 0, -3);
	drawSphere(0.7, _textureId);

	glTranslatef(0.0, 0.0, -0.5);
	glRotatef(200, 1, 0, 0);
	drawCylinder(0.5, 1.0, _textureId);
	glPopMatrix();

	glTranslatef(0.0, 0.0, -0.5);
	glRotatef(160, 1, 0, 0);
	drawCylinder(0.5, 1.0, _textureId);
	glPopMatrix();
}

void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	eyeX = eyeDistance * cos(viewAngleZ * PI / 180) * cos(viewAngleX * PI / 180);
	eyeY = eyeDistance * cos(viewAngleZ * PI / 180) * sin(viewAngleX * PI / 180);
	eyeZ = eyeDistance * sin(viewAngleZ * PI / 180);

	if (viewAngleZ < 90)
	{
		gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	}
	else
	{
		gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
	}

	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

	// clears color and material parameters before drawing platform
	glColor3f(1.0f, 1.0f, 1.0f);
	glMaterialfv(GL_FRONT, GL_SPECULAR, semEspecularidade);
	glMateriali(GL_FRONT, GL_SHININESS, 0);

	if (_textureId)
	{
		//glColor3f(0.9f, 0.9f, 0.9f);
		glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
		glMateriali(GL_FRONT, GL_SHININESS, especMaterialTexture);
	}
	else
	{
		//glColor3f(0.8f, 0.9f, 1.0f);
		glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
		glMateriali(GL_FRONT, GL_SHININESS, especMaterialNoTexture);
	}

	drawBody(_textureId);

	// move to base referential
	// glTranslatef(0.0f, 0.0f, heightPlatform);
	// 	// move to base referential
	// 	glTranslatef(0.0f, 0.0f, heightPlatform);

	//draws the base
	// drawCylinder(diameterBase, heightBase);
	// glTranslatef(0.0f, 0.0f, heightBase);

	// //draws the base
	// drawCylinder(diameterBase, heightBase);
	// glTranslatef(0.0f, 0.0f, heightBase);
	// drawDisk(diameterCylinder, diameterBase);

	// 	// move to arm referential
	// 	glRotatef(angleArm, 0.0f, 0.0f, 1.0f);

	// 	//draws the arm
	// 	drawCylinder(diameterCylinder, sizeArm);

	// 	// move to forearm referential
	// 	glTranslatef(0.0f, 0.0f, sizeArm + diameterSphere / 5);
	// 	glRotatef(angleForearm, 0.0f, 1.0f, 0.0f);

	// 	//draws the forearm
	// 	drawSphere(diameterSphere);
	// 	glTranslatef(0.0f, 0.0f, diameterSphere / 5);
	// 	drawCylinder(diameterCylinder, sizeForearm);

	// 	//move to hand referential
	// 	glTranslatef(0.0f, 0.0f,  sizeForearm + diameterSphere / 5);
	// 	glRotatef(angleHand, 0.0f, 1.0f, 0.0f);

	// 	//draws the hand
	// 	drawSphere(diameterSphere);
	// 	glTranslatef(0.0f, 0.0f, diameterSphere / 5);
	// 	drawCylinder(diameterCylinder, sizeHand);

	// 	// move to clamp referential
	// 	glTranslatef(0.0f, 0.0f, sizeHand + diameterSphere / 5);
	// 	glRotatef(angleClampZ, 0.0f, 0.0f, 1.0f);

	// 	//draws the clamp sphere
	// 	drawSphere(diameterSphere);
	// 	glTranslatef(0.0f, 0.0f, diameterSphere / 2);

	// 	glPushMatrix();

	// 	//draws top part of clamp
	// 	glRotatef(angleClampY+60, 0.0f, 1.0f, 0.0f);

	// 	drawCylinder(diameterCylinder / 3, sizeClampPart);
	// 	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	// 	drawSphere(diameterSphere / 3);

	// 	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	// 	glRotatef(-60, 0.0f, 1.0f, 0.0f);

	// 	drawCylinder(diameterCylinder / 3, sizeClampPart);
	// 	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere/ 15);
	// 	drawSphere(diameterSphere / 3);

	// 	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	// 	glRotatef(-60, 0.0f, 1.0f, 0.0f);
	// 	drawCone(diameterCylinder / 3, sizeClampPart);

	// 	glPopMatrix();
	// 	glPushMatrix();

	// 	//draws bottom part of clamp
	// 	glRotatef(-angleClampY - 60, 0.0f, 1.0f, 0.0f);

	// 	drawCylinder(diameterCylinder / 3, sizeClampPart);
	// 	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	// 	drawSphere(diameterSphere / 3);

	// 	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	// 	glRotatef(60, 0.0f, 1.0f, 0.0f);

	// 	drawCylinder(diameterCylinder / 3, sizeClampPart);
	// 	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	// 	drawSphere(diameterSphere / 3);

	// 	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	// 	glRotatef(60, 0.0f, 1.0f, 0.0f);
	// 	drawCone(diameterCylinder / 3, sizeClampPart);

	// 	glPopMatrix();

	// 	// rotate to draw other two parts
	// 	glRotatef(90, 0.0f, 0.0f, 1.0f);

	// 	glPushMatrix();

	// 	//draws top part of clamp
	// 	glRotatef(angleClampY + 60, 0.0f, 1.0f, 0.0f);

	// 	drawCylinder(diameterCylinder / 3, sizeClampPart);
	// 	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	// 	drawSphere(diameterSphere / 3);

	// 	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	// 	glRotatef(-60, 0.0f, 1.0f, 0.0f);

	// 	drawCylinder(diameterCylinder / 3, sizeClampPart);
	// 	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	// 	drawSphere(diameterSphere / 3);

	// 	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	// 	glRotatef(-60, 0.0f, 1.0f, 0.0f);
	// 	drawCone(diameterCylinder / 3, sizeClampPart);

	// 	glPopMatrix();

	// 	//draws bottom part of clamp
	// 	glRotatef(-angleClampY - 60, 0.0f, 1.0f, 0.0f);

	// 	drawCylinder(diameterCylinder / 3, sizeClampPart);
	// 	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	// 	drawSphere(diameterSphere / 3);

	// 	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	// 	glRotatef(60, 0.0f, 1.0f, 0.0f);

	// 	drawCylinder(diameterCylinder / 3, sizeClampPart);
	// 	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	// 	drawSphere(diameterSphere / 3);

	// 	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	// 	glRotatef(60, 0.0f, 1.0f, 0.0f);
	// 	drawCone(diameterCylinder / 3, sizeClampPart);

	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Escorpiao");

	initLighting();
	initRendering();
	glutDisplayFunc(drawScene);
	// glutKeyboardFunc(handleKeypress);
	// glutMouseFunc(GerenciaMouse);
	glutReshapeFunc(handleResize);

	glutMainLoop();
	return 0;
}
