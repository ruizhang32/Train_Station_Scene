#include <cmath>
#include <GL/freeglut.h>
#include <fstream>
#include <string>
#include <iostream>
#include <math.h>
#include "loadBMP.h"

//# define M_PI 3.14159265358979323846  /* pi */
using namespace std;
//
//float cam_hgt = 30.0;
//float x_pos =22.0;
//float z_pos = 90.0; 

float cam_hgt = 600.0;
float x_pos = 22.0;
float z_pos = 90.0;

void special(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) z_pos++;
	else if (key == GLUT_KEY_DOWN) z_pos--;
	else if (key == GLUT_KEY_LEFT) x_pos++;
	else if (key == GLUT_KEY_RIGHT) x_pos--;
	glutPostRedisplay();
}

void keyBoardSpecail(unsigned char key, int x, int y)
{
	if (key == 'J') cam_hgt++;
	else if (key == 'K') cam_hgt--;
	glutPostRedisplay();
}

void floor(float width, float height)
{
	float white[4] = { 1., 1., 1., 1. };
	float black[4] = { 0 };
	glColor4f(0.7, 0.7, 0.7, 1.0);  //The floor is gray in colour
	glNormal3f(0.0, 1.0, 0.0);

	//The floor is made up of several tiny squares on a 400 x 400 grid. Each square has a unit size.
	glBegin(GL_QUADS);
	/*for (int i = -400; i < 400; i++)
	{
		for (int j = -400; j < 400; j++)
		{*/
			glVertex3f(-width/2, 0, -height/2);
			glVertex3f(width / 2, 0, -height / 2);
			glVertex3f(width / 2, 0, height / 2);
			glVertex3f(-width / 2, 0, height / 2);
	/*	}
	}*/
	glEnd();
}

void drawOval() {
	glColor3f(0., 0.5, 0.);			//Floor colour
	std::ifstream file("oval.txt");
	std::string str;
	std::getline(file, str); // skip the first line of txt. file
	glBegin(GL_LINE_LOOP);
	while (std::getline(file, str))
	{
		float p1[2] = {};
		std::string s = str;
		std::string delimiter = " ";
		std::string token1 = s.substr(0, s.find(delimiter));
		std::string token2 = s.substr(s.find(delimiter) + 1, s.length());
		p1[0] = std::stof(token1);
		p1[1] = std::stof(token2);
		glVertex3f(p1[0], 1.0, p1[1]);
	}
	glEnd();
}

void drawSleeper(float p1[2], float p2[2], float s1, float s2) {
	float u1[2] = { p1[0] - p2[0],p1[1] - p2[1] }; //p1-p2
	float v1[2] = { p1[1] - p2[1],-(p1[0] - p2[0]) };//u1y, -u1x
	float e1[2] = { p1[0] + (v1[0] * s1),(p1[1] + (v1[1]) * s1) };//p1+v1*s1
	float e2[2] = { e1[0] - (u1[0] * s2),(e1[1] - (u1[1]) * s2) };//e1-u1*s2
	float f1[2] = { p1[0] - (v1[0] * s1),(p1[1] - (v1[1]) * s1) };//p1-v1*s1
	float f2[2] = { f1[0] - (u1[0] * s2),(f1[1] - (u1[1]) * s2) };//f1-u1*s2
	glBegin(GL_QUADS);
	glNormal3f(0., 1., 0.);
	glVertex3f(e1[0], 0.0, e1[1]);  
	glVertex3f(e2[0], 0.0, e2[1]);
	glVertex3f(f2[0], 0.0, f2[1]);
	glVertex3f(f1[0], 0.0, f1[1]);
	glEnd();
}

void sleepers(float s1, float s2) {
	glColor4f(1.0, 0.0, 0.0, 1.0);			//track colour
	std::ifstream file("oval.txt");
	std::string str;
	std::getline(file, str); // skip the first line of txt. file
	float p0[2] = {}; // use for loop connection with last two points
	float p1[2] = {};
	float p2[2] = {};
	int cnt = 1;
	while (std::getline(file, str))
	{
		std::string s = str;
		std::string delimiter = " ";
		std::string token1 = s.substr(0, s.find(delimiter));
		std::string token2 = s.substr(s.find(delimiter) + 1, s.length());
		/*calculate pi*/
		if (cnt == 1) {
			p0[0] = std::stof(token1);
			p0[1] = std::stof(token2);
			p1[0] = std::stof(token1);
			p1[1] = std::stof(token2);
		}
		/*calculate pi*/
		else if (cnt == 2) {
			p2[0] = std::stof(token1);
			p2[1] = std::stof(token2);
			drawSleeper(p1, p2, s1, s2);
		}
		else {
			p1[0] = p2[0];
			p1[1] = p2[1];
			p2[0] = std::stof(token1);
			p2[1] = std::stof(token2);
			drawSleeper(p1, p2, s1, s2);
		}
		cnt += 1;
	}
	p1[0] = p2[0];
	p1[1] = p2[1];
	drawSleeper(p1, p0, s1, s2);
}

void drawTrackSegment(float p1[2], float p2[2], float p3[2], float w1, float w2) {
	float u1[2] = { p1[0] - p2[0],p1[1] - p2[1] }; //p1-p2
	float v1[2] = { p1[1] - p2[1],-(p1[0] - p2[0]) };//u1y, -u1x
	float u2[2] = { p2[0] - p3[0],p2[1] - p3[1] }; //p2-p3
	float v2[2] = { p2[1] - p3[1],-(p2[0] - p3[0]) };//u2y, -u2x
	// outer track quad points a1\a2\b1\b2
	float a1[2] = { p1[0] + (v1[0] * w1),(p1[1] + (v1[1]) * w1) };//p1+v1*w1
	float a2[2] = { p1[0] + (v1[0] * w2),(p1[1] + (v1[1]) * w2) };//p1+v1*w2
	float b1[2] = { p2[0] + (v2[0] * w1),(p2[1] + (v2[1]) * w1) }; //p2+v2*w1
	float b2[2] = { p2[0] + (v2[0] * w2),(p2[1] + (v2[1]) * w2) }; //p2+v2*w2
	// outer track quad points c1\c2\d1\d2
	float c1[2] = { p1[0] - (v1[0] * w1),(p1[1] - (v1[1]) * w1) };//p1-v1*w1
	float c2[2] = { p1[0] - (v1[0] * w2),(p1[1] - (v1[1]) * w2) };//p1-v1*w2
	float d1[2] = { p2[0] - (v2[0] * w1),(p2[1] - (v2[1]) * w1) }; //p2-v2*w1
	float d2[2] = { p2[0] - (v2[0] * w2),(p2[1] - (v2[1]) * w2) }; //p2-v2*w2
	glBegin(GL_QUADS); 
	// draw outer track
	glNormal3f(0., 1., 0.);
	glVertex3f(a1[0], 1.0, a1[1]);  //Qaud 1 facing up
	glVertex3f(a2[0], 1.0, a2[1]);
	glVertex3f(b2[0], 1.0, b2[1]);
	glVertex3f(b1[0], 1.0, b1[1]);

	glNormal3f(v1[0], 0.0, v1[1]);  //Quad 2 facing inward
	glVertex3f(a1[0], 1.0, a1[1]);
	glVertex3f(a1[0], 0.0, a1[1]);
	glNormal3f(v1[0], 0.0, v1[1]);
	glVertex3f(b1[0], 0.0, b1[1]);
	glVertex3f(b1[0], 1.0, b1[1]);

	glNormal3f(v1[0], 0.0, v1[1]);   //Quad 3 facing outward
	glVertex3f(a2[0], 1.0, a2[1]);
	glVertex3f(a2[0], 0.0, a2[1]);
	glNormal3f(v1[0], 0.0, v1[1]);
	glVertex3f(b2[0], 0.0, b2[1]);
	glVertex3f(b2[0], 1.0, b2[1]);

	// draw inner track
	glNormal3f(0., 1., 0.);
	glVertex3f(c1[0], 1.0, c1[1]);  //Qaud 1 facing up
	glVertex3f(c2[0], 1.0, c2[1]);
	glVertex3f(d2[0], 1.0, d2[1]);
	glVertex3f(d1[0], 1.0, d1[1]);

	glNormal3f(v1[0], 0.0, v1[1]);  //Quad 2 facing inward
	glVertex3f(c1[0], 1.0, c1[1]);
	glVertex3f(c1[0], 0.0, c1[1]);
	glNormal3f(v1[0], 0.0, v1[1]);
	glVertex3f(d1[0], 0.0, d1[1]);
	glVertex3f(d1[0], 1.0, d1[1]);

	glNormal3f(v1[0], 0.0, v1[1]);   //Quad 3 facing outward
	glVertex3f(c2[0], 1.0, c2[1]);
	glVertex3f(c2[0], 0.0, c2[1]);
	glNormal3f(v1[0], 0.0, v1[1]);
	glVertex3f(d2[0], 0.0, d2[1]);
	glVertex3f(d2[0], 1.0, d2[1]);

	glEnd();
}

void track(float w1, float w2) {
	glColor4f(0.0, 0.0, 0.3, 1.0);			//track colour
	std::ifstream file("oval.txt");
	std::string str;
	std::getline(file, str); // skip the first line of txt. file
	float p0[2] = {}; // use for loop connection with last two points
	float p00[2] = {};
	float p1[2] = {};
	float p2[2] = {};
	float p3[2] = {};
	int cnt = 1;
	while (std::getline(file, str))
	{
		std::string s = str;
		std::string delimiter = " ";
		std::string token1 = s.substr(0, s.find(delimiter));
		std::string token2 = s.substr(s.find(delimiter) + 1, s.length());
		/*calculate pi*/
		if (cnt == 1) {
			p0[0] = std::stof(token1); p0[1] = std::stof(token2);
			p1[0] = std::stof(token1); p1[1] = std::stof(token2);
		}
		/*calculate pi*/
		else if (cnt == 2) {
			p2[0] = std::stof(token1); p2[1] = std::stof(token2);
			p00[0] = std::stof(token1); p00[1] = std::stof(token2);
		}
		else if (cnt == 3) {
			p3[0] = std::stof(token1); p3[1] = std::stof(token2);
			drawTrackSegment(p1, p2, p3, w1, w2);
		}
		else {
			p1[0] = p2[0]; p1[1] = p2[1];
			p2[0] = p3[0]; p2[1] = p3[1];
			p3[0] = std::stof(token1);p3[1] = std::stof(token2);
			drawTrackSegment(p1, p2, p3, w1, w2);
		}
		cnt += 1;
	}
	p1[0] = p2[0]; p1[1] = p2[1];
	p2[0] = p3[0]; p2[1] = p3[1];
	drawTrackSegment(p1, p2, p0, w1, w2);
	drawTrackSegment(p2, p0, p00, w1, w2);
}

void base(float w1, float w2)
{
	glColor4f(0.1f, 0.1f, 0.1f, 0.25);//Dark grey

	glPushMatrix();
	glTranslatef(0.0, 4.0, 0.0); // translate from origin
	float baseWidthScaleSize = ((w2 - w1) / 2 + w1 ) * 2; // base width related with inner/outer track ((3-2)/2 + 2) * 2 = 5
	float baseLengthScaleSize = 8.0; float baseHeightScaleSize = 2.0; // set base length and height
	glScalef(baseLengthScaleSize, baseHeightScaleSize, baseWidthScaleSize);  //Size 8x5 units, thickness 2 units.
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();					//Connector between wagons
	glTranslatef(4.1, 4.0, 0.0); 
	glutSolidCube(1.5); 
	glPopMatrix();

	//4 Wheels (radius = 1 units)
	//x, z positions of wheels:
	float wx[4] = { 3,   3,   -3,   -3 };
	float wz[4] = { -2.6, 2.6, 2.6, -2.6 }; 
	glColor4f(0.5, 0., 0., 1.0);    //Wheel color
	GLUquadric* q = gluNewQuadric();   //Disc

	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		glTranslatef(wx[i], 2.2, wz[i]);
		gluDisk(q, 0.0, 1.0, 20, 2); // outer r = 1.0 unit
		glPopMatrix();
	}
}

void engine(float w1, float w2)
{
	base(w1, w2);

	//Cab
	glColor4f(0.8, 0.8, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(3., 7.5, 0.0);
	glScalef(2.0, 5.0, 5.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3., 11.5, 0.0);
	glScalef(2.0, 3.0, 3.0);
	glutSolidCube(1.0);
	glPopMatrix();

	//Boiler
	GLUquadric* q = gluNewQuadric();   //Cylinder
	glPushMatrix();

	glColor4f(0.5, 0., 0., 1.0);
	glTranslatef(2., 8., 0.0); // y height = 8.0 = 3+1+4 , x moves to 2.0
	glRotatef(-90.0, 0., 1., 0.); 
	gluCylinder(q, 3., 3., 6.0, 20, 5); // radius = 3.0, height = 6.0 
	glTranslatef(0.0, 0.0, 6.0);
	gluDisk(q, 0.0, 3.0, 20, 3);

	glColor4f(1.0, 1.0, 0.0, 1.0);
	glTranslatef(0.0, 2.5, 0.1); 
	gluDisk(q, 0.0, 1.0, 20, 2);  //head light r = 1.0
	glPopMatrix();

}

//--------------- WAGON ----------------------------------
// This simple model of a rail wagon consists of the base,
// and a cube(!)
//--------------------------------------------------------
void wagon(float w1, float w2)
{
	base(w1, w2);

	glColor4f(0.0, 1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(0.0, 8.0, 0.0);
	glScalef(8.0, 6.0, 5.0);
	glutSolidCube(1.0);
	glPopMatrix();
}

GLuint txId;
//------Function to load a texture in bmp format  ------------------------
void loadTexture()
{
	glGenTextures(1, &txId); 				// Create a Texture object
	glBindTexture(GL_TEXTURE_2D, txId);		//Use this texture
	loadBMP("WagonTexture.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

float convertToOneDecimalFloat(float value) {
	return static_cast<float>((value * 10.)) / 10.;
}

void wagon1(float w1, float w2)
{
	base(w1, w2);

	glColor4f(0.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId);

	glPushMatrix();
	glTranslatef(0.0, 8.0, 0.0);
	//3 large polygons (front, back, top)
	glBegin(GL_QUADS);
	
	float y1 = convertToOneDecimalFloat(static_cast<float>(87)/256);
	float y2 = convertToOneDecimalFloat(static_cast<float>(115) / 256);
	float y3 = convertToOneDecimalFloat(static_cast<float>(212) / 256);
	float x = convertToOneDecimalFloat(static_cast<float>(103) / 256);
	glNormal3f(0.0, 0.0, 1.0);   //Facing +z (Front side)
	glTexCoord2f(0.148, 0.); glVertex3f(-4.0, -3.0, 2.5);
	glTexCoord2f(0.499, 0.); glVertex3f(4.0, -3.0, 2.5);
	glTexCoord2f(0.499, y1); glVertex3f(4.0, 3.0, 2.5);
	glTexCoord2f(0.148, y1); glVertex3f(-4.0, 3.0, 2.5);

	glNormal3f(0.0, 0.0, -1.0);   //Facing -z (Back side)
	glTexCoord2f(0., 0.); glVertex3f(-4.0, -3.0, -2.5);
	glTexCoord2f(1., 0.); glVertex3f(4.0, -3.0, -2.5);
	glTexCoord2f(1., y1); glVertex3f(4.0, 3.0, -2.5);
	glTexCoord2f(0., y1); glVertex3f(-4.0, 3.0, -2.5);

	glNormal3f(0.0, 1.0, 0.0);   //Facing +y (Top side)
	glTexCoord2f(0., y2); glVertex3f(-4.0, 3.0, 2.5);
	glTexCoord2f(1., y2); glVertex3f(4.0, 3.0, 2.5);
	glTexCoord2f(1., y3); glVertex3f(4.0, 3.0, -2.5);
	glTexCoord2f(0., y3); glVertex3f(-4.0, 3.0, -2.5);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId);
	//2 small side polygons (left, right)
	glColor4f(0.1f, 0.1f, 0.1f, 0.25);//Dark grey
	glBegin(GL_QUADS);
	glNormal3f(-1.0, 0.0, 0.0);   //Facing -x (Left side)
	glTexCoord2f(0., y2); glVertex3f(-4.0, -3.0, 2.5);
	glTexCoord2f(x, y2); glVertex3f(-4.0, 3.0, 2.5);
	glTexCoord2f(x, y3); glVertex3f(-4.0, 3.0, -2.5);
	glTexCoord2f(0., y3); glVertex3f(-4.0, -3.0, -2.5);

	glNormal3f(1.0, 0.0, 0.0);   //Facing +x (Right side)
	glVertex3f(4.0, 3.0, 2.5);
	glVertex3f(4.0, 3.0, -2.5);
	glVertex3f(4.0, -3.0, -2.5);
	glVertex3f(4.0, -3.0, 2.5);
	glEnd();
	glPopMatrix();
}

void stationBase() {
	glColor3f(0.1f, 0.0f, 0.0f);//Brown
	glBegin(GL_QUADS);

	glNormal3f(0.0, 0.0, 1.0);   //Facing +z (Front side) 
	glVertex3f(-25.0, 0.0, 30);
	glVertex3f(25.0, 0.0, 30);
	glVertex3f(25.0, 5.0, 30);
	glVertex3f(-25.0, 5.0, 30);

	glNormal3f(0.0, 0.0, -1.0);   //Facing -z (Back side)
	glVertex3f(-25.0, 0.0, -12);
	glVertex3f(25.0, 0.0, -12);
	glVertex3f(25.0, 5.0, -12);
	glVertex3f(-25.0, 5.0, -12);

	glNormal3f(0.0, 1.0, 0.0);   //has to change normal vector(Top side)
	glVertex3f(-25.0, 5.0, 30);
	glVertex3f(25.0, 5.0, 30);
	glVertex3f(25.0, 5.0, -12);
	glVertex3f(-25.0, 5.0, -12);

	glNormal3f(-1.0, 0.0, 0.0);   //has to change (Left side) 
	glVertex3f(-25.0, 0.0, 30);
	glVertex3f(-25.0, 0.0, -12);
	glVertex3f(-25.0, 5.0, -12);
	glVertex3f(-25.0, 5.0, 30);

	glNormal3f(1.0, 0.0, 0.0);   //has to change (Right side) 
	glVertex3f(25.0, 0.0, 30);
	glVertex3f(25.0, 0.0, -12);
	glVertex3f(-25.0, 5.0, -12);
	glVertex3f(-25.0, 5.0, 30);

	glEnd();
}

void stationSupportBeam(float x, float y, float z) {
	glColor4f(1.0f, 1.0f, 0.0f, 0.0f); //yellow 
	glPushMatrix();
	glScalef(x, y, z);
	glutSolidCube(1.0);
	glPopMatrix();
}

void stationSign() {
	glColor4f(1.0f, 1.0f, 0.0f, 0.0f); //yellow 
	
	glPushMatrix();
	glTranslatef(-5., 1., 0.);
	stationSupportBeam(0.5, 5., 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5., 1., 0.);
	stationSupportBeam(0.5, 5., 0.5);
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId);
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(-5.0, 1.0, 0.);
	glVertex3f(5.0, 1.0, 0.);
	glVertex3f(5.0, 5.0, 0.);
	glVertex3f(-5.0, 5.0, 0.);
	glEnd();
	glPopMatrix();
}

void station()
{
	// draw station base
	stationBase();

	// draw station sign
	glPushMatrix();
	glTranslatef(0., 24., 21.5);
	stationSign();
	glPopMatrix();

	// draw support beam for station
	glPushMatrix();
	glTranslatef(-19.5, 11., 21.5);
	stationSupportBeam(1., 23., 1.);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(19.5, 11., 21.5);
	stationSupportBeam(1., 23., 1.);
	glPopMatrix();

	// draw station building
	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId);*/

	glPushMatrix();
	glTranslatef(0., 12.5, 0.);
	glBegin(GL_QUADS);

	/*float y1 = convertToOneDecimalFloat(static_cast<float>(87) / 256);
	float y2 = convertToOneDecimalFloat(static_cast<float>(115) / 256);
	float y3 = convertToOneDecimalFloat(static_cast<float>(212) / 256);
	float x = convertToOneDecimalFloat(static_cast<float>(103) / 256);*/
	glColor4f(1.0f, 1.0f, 0.0f, 0.0f); //yellow 
	glNormal3f(0.0, 0.0, 1.0);   //Facing ??? (shelter board)
	//glTexCoord2f(0.148, 0.); 
	glVertex3f(-20.0, 15.0, 10);
	//glTexCoord2f(0.499, 0.); 
	glVertex3f(-20.0, 10.0, 22);
	//glTexCoord2f(0.499, y1); 
	glVertex3f(20.0, 10.0, 22);
	//glTexCoord2f(0.148, y1); 
	glVertex3f(20.0, 15.0, 10);

	glColor4f(0.0, 1.0, 1.0, 1.0);
	glNormal3f(0.0, 0.0, 1.0);   //Facing +z (Front side)
	//glTexCoord2f(0.148, 0.); 
	glVertex3f(-20.0, -15.0, 10);
	//glTexCoord2f(0.499, 0.); 
	glVertex3f(20.0, -15.0, 10);
	//glTexCoord2f(0.499, y1); 
	glVertex3f(20.0, 15.0, 10);
	//glTexCoord2f(0.148, y1); 
	glVertex3f(-20.0, 15.0, 10);

	glNormal3f(0.0, 0.0, -1.0);   //Facing -z (Back side)
	//glTexCoord2f(0., 0.); 
	glVertex3f(-20.0, -15.0, -10);
	//glTexCoord2f(1., 0.); 
	glVertex3f(20.0, -15.0, -10);
	//glTexCoord2f(1., y1); 
	glVertex3f(20.0, 15.0, -10);
	//glTexCoord2f(0., y1); 
	glVertex3f(-20.0, 15.0, -10);

	glNormal3f(0.0, 1.0, 0.0);   //has to change (Top Front side)
	//glTexCoord2f(0., y2); 
	glVertex3f(-20.0, 15.0, 10);
	//glTexCoord2f(1., y2); 
	glVertex3f(20.0, 15.0, 10);
	//glTexCoord2f(1., y3); 
	glVertex3f(10.0, 30., 0);
	//glTexCoord2f(0., y3); 
	glVertex3f(-10.0, 30.0, 0);

	glNormal3f(0.0, 1.0, 0.0);   //has to change (Top Back side)
	//glTexCoord2f(0., y2); 
	glVertex3f(20.0, 15.0, -10);
	//glTexCoord2f(1., y2); 
	glVertex3f(20.0, 15.0, -10);
	//glTexCoord2f(1., y3); 
	glVertex3f(10.0, 30., 0);
	//glTexCoord2f(0., y3); 
	glVertex3f(-10.0, 30.0, 0);

	glNormal3f(0.0, 1.0, 0.0);   //has to change (Left side)
	//glTexCoord2f(0., y2); 
	glVertex3f(-20.0, -15.0, 10);
	//glTexCoord2f(1., y2); 
	glVertex3f(-20.0, -15.0, -10);
	//glTexCoord2f(1., y3); 
	glVertex3f(-20.0, 15.0, -10);
	//glTexCoord2f(0., y3); 
	glVertex3f(-20.0, 15.0, 10);

	glNormal3f(0.0, 1.0, 0.0);   //has to change (Right side)
	//glTexCoord2f(0., y2); 
	glVertex3f(20.0, -15.0, 10);
	//glTexCoord2f(1., y2); 
	glVertex3f(20.0, -15.0, -10);
	//glTexCoord2f(1., y3); 
	glVertex3f(20.0, 15.0, -10);
	//glTexCoord2f(0., y3); 
	glVertex3f(20.0, 15.0, 10);

	glEnd();

	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId);
	//2 small side polygons (left, right)
	glColor4f(1.0f, 1.0f, 0.0f, 0.0f); //yellow 
	glBegin(GL_TRIANGLES);
	glNormal3f(-1.0, 0.0, 0.0);   //has to change (Top Left side)
	//glTexCoord2f(x, y2); 
	glVertex3f(-20.0, 15.0, 10);
	//glTexCoord2f(x, y3); 
	glVertex3f(-20.0, 15.0, -10);
	//glTexCoord2f(0., y3); 
	glVertex3f(-10.0, 30.0, 0);

	glNormal3f(1.0, 0.0, 0.0);   //has to change (Top Right side)
	glVertex3f(20.0, 15.0, 10);
	glVertex3f(20.0, 15.0, -10);
	glVertex3f(10.0, 30., 0);
	glEnd();

	glPopMatrix();
}

float trackPoints[492][2] = {};
void convertTxtToArray() {
	std::ifstream file("oval.txt");
	std::string str;
	std::getline(file, str); // skip first line of txt. file
	int i = 0;
	while (std::getline(file, str))
	{
		std::string s = str;
		std::string delimiter = " ";
		std::string token1 = s.substr(0, s.find(delimiter));
		std::string token2 = s.substr(s.find(delimiter) + 1, s.length());
		trackPoints[i][0] = std::stof(token1); // pxi
		trackPoints[i][1] = std::stof(token2); // pzi
		i++;
	}
}

int icurr = 0;int icurrk1 = 0; int icurrk2 = 0;
int icurrm = 0;int icurrmk1 = 0; int icurrmk2 = 0;
int icurr2m = 0;int icurr2mk1 = 0; int icurr2mk2 = 0;
int icurr3m = 0;int icurr3mk1 = 0; int icurr3mk2 = 0;

void myTimer(int value)
{
	glutPostRedisplay();
	if (icurr < 492) {
		icurr += 1;
		if (icurr == 492) {
			icurr = 0;
		}
	}
	cout << "icurr"; cout << icurr; cout << ",";
	glutTimerFunc(50, myTimer, 0);	
}

//--Display: ---------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.
void display(void) 
{
	float w1 = 2.0; float w2 = 3.0;
	float lgt_pos[] = { 0.0f, 20.0f, 0.0f, 1.0f };  //light0 position (directly above the origin) Omni-directional point sources
	glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   //Set light position

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(x_pos, cam_hgt, z_pos, 0, 0, -40, 0, 1, 0);  //Camera position and orientation
	
	drawOval();
	track(w1, w2);
	sleepers(4.5,0.5);
	floor(300.0,250.0);

	glPushMatrix();
	glTranslatef(0., 0., -75.);
	station();
	glPopMatrix();
	
	convertTxtToArray();

	int m = 9;
	int rToA = 180 / 3.1415926;
	int halfLocomotiveLength = 4; 
	icurrk1 = icurr + halfLocomotiveLength; //icurr + k
	icurrk2 = icurr - halfLocomotiveLength; //icurr - k
	cout << "icurrk1: "; cout << icurrk1;  cout << " ";
	cout << "icurrk2: "; cout << icurrk2;  cout << " ";

	float ux = trackPoints[icurrk2][0] - trackPoints[icurrk1][0];
	float uz = trackPoints[icurrk2][1] - trackPoints[icurrk1][1];
	cout << "ux "; cout << ux;
	cout << "trackPoints[icurrk1][0]: "; cout << trackPoints[icurrk1][0];  cout << " ";
	cout << "trackPoints[icurrk2][0]: "; cout << trackPoints[icurrk2][0];  cout << " ";
	cout << " "; cout << "uz "; cout << uz; cout << " ";
	cout << "trackPoints[icurrk1][1]: "; cout << trackPoints[icurrk1][1];  cout << " ";
	cout << "trackPoints[icurrk2][1]: "; cout << trackPoints[icurrk2][1];  cout << " ";
	float theta = atan2(uz, -ux) * rToA;
	cout << "theta "; cout << theta; cout << " ";

	icurrm = icurr - m;
	icurrmk1 = icurrm + halfLocomotiveLength; //icurr - m + k
	icurrmk2 = icurrm - halfLocomotiveLength; //icurr - m - k
	float ux1 = trackPoints[icurrmk2][0] - trackPoints[icurrmk1][0];
	float uz1 = trackPoints[icurrmk2][1] - trackPoints[icurrmk1][1];
	float theta1 = atan2(uz1, -ux1) * rToA;
	cout << "theta1 "; cout << theta1; cout << " ";

	icurr2m = icurr - 2*m;
	icurr2mk1 = icurr2m + halfLocomotiveLength;
	icurr2mk2 = icurr2m - halfLocomotiveLength;
	float ux2 = trackPoints[icurr2mk2][0] - trackPoints[icurr2mk1][0];
	float uz2 = trackPoints[icurr2mk2][1] - trackPoints[icurr2mk1][1];
	float theta2 = atan2(uz2, -ux2) * rToA;
	cout << "theta2 "; cout << theta2; cout << " ";

	icurr3m = icurr - 3 * m;
	icurr3mk1 = icurr3m + halfLocomotiveLength;
	icurr3mk2 = icurr3m - halfLocomotiveLength;
	float ux3 = trackPoints[icurr3mk2][0] - trackPoints[icurr3mk1][0];
	float uz3 = trackPoints[icurr3mk2][1] - trackPoints[icurr3mk1][1];
	float theta3 = atan2(uz3, -ux3) * rToA;
	cout << "theta3 "; cout << theta3; cout << " ";

	glPushMatrix();
	glTranslatef(trackPoints[icurr][0], 0., trackPoints[icurr][1]);
	glRotatef(theta, 0., 1., 0.); // rotate using ui
	engine(w1, w2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(trackPoints[icurrm][0], 0., trackPoints[icurrm][1]);
	cout << "trackPoints[icurrm][0]: "; cout << trackPoints[icurrm][0];  cout << " ";
	cout << "trackPoints[icurrm][1]: "; cout << trackPoints[icurrm][1];  cout << " ";
	glRotatef(theta1, 0., 1., 0.); // rotate certain angle first 
	wagon1(w1, w2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(trackPoints[icurr2m][0], 0., trackPoints[icurr2m][1]);
	cout << "trackPoints[icurr2m][0]: "; cout << trackPoints[icurr2m][0];  cout << " ";
	cout << "trackPoints[icurr2m][1]: "; cout << trackPoints[icurr2m][1];  cout << " ";
	glRotatef(theta2, 0., 1., 0.); // rotate certain angle first 
	wagon1(w1, w2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(trackPoints[icurr3m][0], 0., trackPoints[icurr3m][1]);
	cout << "trackPoints[icurr3m][0]: "; cout << trackPoints[icurr3m][0];  cout << " ";
	cout << "trackPoints[icurr3m][1]: "; cout << trackPoints[icurr3m][1];  cout << " ";
	glRotatef(theta3, 0., 1., 0.); // rotate certain angle first 
	wagon1(w1, w2);
	glPopMatrix();

	glutSwapBuffers();   //Useful for animation
}

//----------------------------------------------------------------------
//Parameter/state initialization
void initialize(void) 
{
	glClearColor(0.0, 0.0, 0.0, 0.0);  //Background colour
	loadTexture();
	float grey[4] = { 0.2, 0.2, 0.2, 1.0 };
	float white[4] = { 1.0, 1.0, 1.0, 1.0 };
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//	Define light's ambient, diffuse, specular properties
	glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30., 1.0, 10.0, 1000.0);   //Perspective projection
}


//  ------- Main: Initialize glut window and register call backs -------
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1200, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Railway Station");
	initialize();
	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyBoardSpecail);
	glutTimerFunc(50, myTimer, 0);
	glutMainLoop();
	return 0;
}
