#include <cmath>
#include <GL/freeglut.h>
#include <fstream>
#include <string>
#include <iostream>
#include <math.h>
#include "loadBMP.h"
#include "loadTGA.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
const int NPTS = 492;
int halfLocomotiveLength = 4;
float armAngleLeft = 0;
float armAngleRight = 0;
float redlight;
float greenlight;
float wheelRotateAngle = 0;
float x_pos = -150.0; 
float y_pos = 28.0;
float z_pos = 0.0; 
float y_angle = 10.0;	//Rotation angles about x, z axes
float look_x = 100.;
float look_y = 0.;
float look_z = 0.;

int icurr = 1; int icurrk1; int icurrk2;
int icurrm; int icurrmk1; int icurrmk2;
int icurr2m; int icurr2mk1; int icurr2mk2;
int icurr3m; int icurr3mk1; int icurr3mk2;
float ux;
float uz;
int viewFlag = 0;
int refreshFlag = 0;

const int trainStayPeriod = 100; // stay time = trainStayPeriod * 50ms
int timeClock = trainStayPeriod;


float trackPoints[NPTS][2] = {};
void storeTrackMedianLinePointsToArray() {
	std::ifstream file("Oval.txt");
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

int modify_value(int value) {
	if (value >= NPTS)
		return value - NPTS;
	if (value < 0)
		return value + NPTS;
	return value;
}

GLuint texId[31];
void loadTexture()
{	
	glGenTextures(31, texId); 				

	// textures for skybox
	// *** left ***
	glBindTexture(GL_TEXTURE_2D, texId[0]);
	loadTGA("L5.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// *** back ***
	glBindTexture(GL_TEXTURE_2D, texId[1]);
	loadTGA("B5.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// *** right ***
	glBindTexture(GL_TEXTURE_2D, texId[2]);
	loadTGA("R5.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// *** front***
	glBindTexture(GL_TEXTURE_2D, texId[3]);
	loadTGA("F5.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// *** top ***
	glBindTexture(GL_TEXTURE_2D, texId[4]);
	loadTGA("U5.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// *** down ***
	glBindTexture(GL_TEXTURE_2D, texId[5]);
	loadTGA("D5.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// road 
	glBindTexture(GL_TEXTURE_2D, texId[6]);		
	loadBMP("Road.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	// wagon
	glBindTexture(GL_TEXTURE_2D, texId[7]);		
	loadBMP("WagonTexture.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// station
	glBindTexture(GL_TEXTURE_2D, texId[8]);		
	loadBMP("WoodenWall1.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, texId[9]);
	loadBMP("StationSign.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texId[10]);
	loadBMP("CHCH.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texId[11]);
	loadBMP("gallery3.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texId[12]);
	loadBMP("gallery5.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texId[13]);
	loadBMP("gallery2.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texId[14]);
	loadBMP("gallery4.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texId[15]);
	loadBMP("tunnelWall.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void myTimer(int value)
{
	glutPostRedisplay();
	if (timeClock < trainStayPeriod) {
		timeClock++;
	}
	else {
		if (icurr < NPTS) {
			icurr += 1;
			if (icurr == NPTS) {
				icurr = 1;
			}
		}

		wheelRotateAngle+=60;
		if (wheelRotateAngle > 360) wheelRotateAngle = 0;

		// angles control left barrier arm
		if (icurr >= 0 && icurr < 75) {
			armAngleLeft += 1.2;
		}
		else if (icurr >= 76 && icurr < 200) {
			armAngleLeft = 90.;
		}
		else if (icurr >= 200 and icurr <= 275) {
			armAngleLeft -= 1.2;
		}
		// angles control right barrier arm
		if (icurr >= 265 && icurr < 340) {
			armAngleRight += 1.2;
		}
		else if (icurr >= 340 && icurr <= 415) {
			armAngleRight = 90.;
		}
		else if (icurr > 415 or icurr <= 0) {
			armAngleRight -= 1.2;
		}
		
		// control signal light
		if (icurr >= 65 && icurr < 200) {
			redlight = 1.0f;
			greenlight = 0.15f;
		}
		else{
			redlight = 0.15f;
			greenlight = 1.0f;
		}

		if(icurr == 1){
			timeClock = 0;
		}

		cout << "icurr"; cout << icurr; cout << ",";
	}
	glutTimerFunc(50, myTimer, 0);
}

float convertToOneDecimalFloat(float value) {
	return static_cast<float>((value * 10.)) / 10.;
}

//Up arrow : Move camera forward in the current direction
//Down arrow : Move camera backward in the current direction
//Left arrow : Change the current direction towards left by a certain angle
//Right arrow : Change the current direction towards right by a certain angle
//Page - up : Increase camera height
//Page - down : Decrease camera height
void special(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) {
		x_pos++;
		if (x_pos > 150) x_pos = 150;
	}
	else if (key == GLUT_KEY_DOWN) {
		x_pos--;
		if (x_pos < -150) x_pos = -150;
	}
	else if (key == GLUT_KEY_LEFT) {
		y_angle--;
	}
	else if (key == GLUT_KEY_RIGHT) {
		y_angle++;
	}
	else if (key == GLUT_KEY_PAGE_UP) {
		y_pos++;
		if (y_pos > 300) y_pos = 300;
	}
	else if (key == GLUT_KEY_PAGE_DOWN) {
		y_pos--;
		if (y_pos < 0) y_pos = 0;
	}
	glutPostRedisplay();
}

void keyBoardSpecail(unsigned char key, int x, int y)
{
	if (key == 'C') { 
		// if view is from train station view, shift view to view from signal light
		if (viewFlag == 2) {
			viewFlag = 0;
		}
		// else shift view to driver's view
		else if (viewFlag == 0) {
			viewFlag = 1;			
		}
		// if view is not from train station view, shift view to train station view
		else if (viewFlag == 1) {
			viewFlag = 2;
		}
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) y_pos += 5;
	else if (button == GLUT_RIGHT_BUTTON) y_pos -= 5;
	if (y_pos > 300) y_pos = 300;
	else if (y_pos < 0) y_pos = 0;
	glutPostRedisplay();

}
//--------------------------------------------------------------------------------------------------------------------------------
// skybox
void skybox() {
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);

	////////////////////// LEFT WALL ///////////////////////
	glBindTexture(GL_TEXTURE_2D, texId[0]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-150, 0, 150);
	glTexCoord2f(1.0, 0.0); glVertex3f(-150, 0., -150);
	glTexCoord2f(1.0, 1.0); glVertex3f(-150, 300., -150);
	glTexCoord2f(0.0, 1.0); glVertex3f(-150, 300, 150);
	glEnd();

	////////////////////// FRONT WALL ///////////////////////
	glBindTexture(GL_TEXTURE_2D, texId[1]);
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-150, 0, -150);
	glTexCoord2f(1.0, 0.0); glVertex3f(150, 0., -150);
	glTexCoord2f(1.0, 1.0); glVertex3f(150, 300, -150);
	glTexCoord2f(0.0, 1.0); glVertex3f(-150, 300, -150);
	glEnd();

	////////////////////// RIGHT WALL ///////////////////////
	glBindTexture(GL_TEXTURE_2D, texId[2]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, -1.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(150, 0, -150);
	glTexCoord2f(1.0, 0.0); glVertex3f(150, 0, 150);
	glTexCoord2f(1.0, 1.0); glVertex3f(150, 300, 150);
	glTexCoord2f(0.0, 1.0); glVertex3f(150, 300, -150);
	glEnd();


	////////////////////// REAR WALL ////////////////////////
	glBindTexture(GL_TEXTURE_2D, texId[3]);
	glBegin(GL_QUADS);
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(150, 0, 150);
	glTexCoord2f(1.0, 0.0); glVertex3f(-150, 0, 150);
	glTexCoord2f(1.0, 1.0); glVertex3f(-150, 300, 150);
	glTexCoord2f(0.0, 1.0); glVertex3f(150, 300, 150);
	glEnd();

	/////////////////////// TOP //////////////////////////
	glBindTexture(GL_TEXTURE_2D, texId[4]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-150, 300, -150);
	glTexCoord2f(1.0, 0.0); glVertex3f(150, 300, -150);
	glTexCoord2f(1.0, 1.0); glVertex3f(150, 300, 150);
	glTexCoord2f(0.0, 1.0); glVertex3f(-150, 300, 150);
	glEnd();

	/////////////////////// FLOOR //////////////////////////
	glBindTexture(GL_TEXTURE_2D, texId[5]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-150, 0., 150);
	glTexCoord2f(1.0, 0.0); glVertex3f(150, 0., 150);
	glTexCoord2f(1.0, 1.0); glVertex3f(150, 0., -150);
	glTexCoord2f(0.0, 1.0); glVertex3f(-150, 0., -150);
	glEnd();

}

//--------------------------------------------------------------------------------------------------------------------------------
// road
void roadName() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId[6]);
}

void road() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId[6]);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glNormal3f(0., 1., 0.);
	glTexCoord2f(0., 0.); glVertex3f(-200., 0.2, -10.);
	glTexCoord2f(7., 0.); glVertex3f(200., 0.2, -10.);
	glTexCoord2f(7., 1.); glVertex3f(200., 0.2, 10.);
	glTexCoord2f(0, 1.); glVertex3f(-200., 0.2, 10.);
	glEnd();
}

//--------------------------------------------------------------------------------------------------------------------------------
// tracks and sleepers
void trackMedianLine() {
	glColor3f(0., 0.5, 0.);			
	glBegin(GL_LINE_LOOP);
	for (int i=0; i < NPTS; i++){
		glVertex3f(trackPoints[i][0], 1.0, trackPoints[i][1]);
	}
	glEnd();
}

void aSleeper(float p1[2], float p2[2], float s1, float s2) {
	float u1[2] = { p1[0] - p2[0],p1[1] - p2[1] }; //p1-p2
	float v1[2] = { p1[1] - p2[1],-(p1[0] - p2[0]) };//u1y, -u1x
	float e1[2] = { p1[0] + (v1[0] * s1),(p1[1] + (v1[1]) * s1) };//p1+v1*s1
	float e2[2] = { e1[0] - (u1[0] * s2),(e1[1] - (u1[1]) * s2) };//e1-u1*s2
	float f1[2] = { p1[0] - (v1[0] * s1),(p1[1] - (v1[1]) * s1) };//p1-v1*s1
	float f2[2] = { f1[0] - (u1[0] * s2),(f1[1] - (u1[1]) * s2) };//f1-u1*s2
	glBegin(GL_QUADS);
	glNormal3f(0., 1., 0.);
	glVertex3f(e1[0], 0.2, e1[1]);  
	glVertex3f(e2[0], 0.2, e2[1]);
	glVertex3f(f2[0], 0.2, f2[1]);
	glVertex3f(f1[0], 0.2, f1[1]);
	glEnd();
}

void sleepers(float s1, float s2) {
	glColor4f(0.2, 0.2, 0.2, 1.0);	//track colour: grey
	for (int i = 0; i < 491; i++) {
		aSleeper(trackPoints[i], trackPoints[i+1], s1, s2);
	}
	aSleeper(trackPoints[491], trackPoints[0], s1, s2);
}

void trackSegment(float p1[2], float p2[2], float p3[2], float w1, float w2) {
	float v1[2] = { p1[1] - p2[1],-(p1[0] - p2[0]) };//u1y, -u1x
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
	glColor4f(0.1f, 0.0f, 0.0f, 0.6);			//track colour: orange/brown
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
	for (int i = 0; i < 490; i++) {
		trackSegment(trackPoints[i], trackPoints[i+1], trackPoints[i+2], w1, w2);
	}
	trackSegment(trackPoints[490], trackPoints[491], trackPoints[0], w1, w2);
	trackSegment(trackPoints[491], trackPoints[0], trackPoints[1], w1, w2);
}

//--------------------------------------------------------------------------------------------------------------------------------
// train
void trainBase(float w1, float w2)
{
	
	glPushMatrix();
	glColor4f(0.1f, 0.1f, 0.1f, 0.25);//Dark grey
	glTranslatef(0.0, 4.0, 0.0); // translate from origin point
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
	//x, z positions of outer wheels:
	float wx[4] = { 3,   3,   -3,   -3 };
	float wz[4] = { -2.6, 2.6, 2.6, -2.6 }; 
	//z positions of inner wheels:
	float wz1[4] = { -0.1, 0.1, 0.1, -0.1 };
	//z positions of black decoration on wheel:
	float wz2[4] = { -0.2, 0.2, 0.2, -0.2 };

	GLUquadric* q = gluNewQuadric();   //Disc

	glPushMatrix();
	for (int i = 0; i < 4; i++){
		glPushMatrix();
		glTranslatef(wx[i], 2.2, wz[i]);
		glRotatef(wheelRotateAngle, 0., 0., 1.);
		
		glPushMatrix();
		glColor4f(0.5, 0., 0., 1.0);    //Wheel color
		gluDisk(q, 0.0, 1.0, 20, 2); // outer r = 1.0 unit
		glPopMatrix();
		
		glPushMatrix();
		glColor4f(1., 0.5, 0., 1.0); // yellow
		glTranslatef(0., 0., wz1[i]);
		gluDisk(q, 0.0, 0.7, 20, 2); // inner r = 0.7 unit
		glPopMatrix();

		glPushMatrix();
		glColor4f(0., 0., 0., 1.0); // black
		glTranslatef(0., 0.3, wz2[i]);
		gluDisk(q, 0.0, 0.2, 20, 2); 
		glPopMatrix();

		glPopMatrix();
	}
	glPopMatrix();
}

void engine(float w1, float w2)
{
	//Base
	trainBase(w1, w2);

	//Cab
	glColor4f(0.5, 0.5, 0.5, 1.0); // gery
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
	glColor4f(1.0f, 0.f, 0.0f, 0.5f);//red
	glPushMatrix();
	glTranslatef(2., 8., 0.0); // y height = 8.0 = 3+1+4 , x moves to 2.0
	glRotatef(-90.0, 0., 1., 0.); 
	gluCylinder(q, 3., 3., 6.0, 20, 5); // radius = 3.0, height = 6.0 
	//Disk
	glTranslatef(0.0, 0.0, 6.0);
	gluDisk(q, 0.0, 3.0, 20, 3);

	glColor4f(1.0, 1.0, 0.0, 1.0); // yellow
	glTranslatef(0.0, 2.5, 0.1); 
	gluDisk(q, 0.0, 1.0, 20, 2);  //head light r = 1.0
	glPopMatrix();
}

void wagon(float w1, float w2)
{
	trainBase(w1, w2);

	glColor4f(1.0, 1.0, 1.0, 1.0); // white color
	float y1 = convertToOneDecimalFloat(static_cast<float>(87) / 256);
	float y2 = convertToOneDecimalFloat(static_cast<float>(115) / 256);
	float y3 = convertToOneDecimalFloat(static_cast<float>(212) / 256);
	float x = convertToOneDecimalFloat(static_cast<float>(103) / 256);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId[7]);
	
	glPushMatrix();
	glTranslatef(0.0, 8.0, 0.0);
	
	glBegin(GL_QUADS);
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

void trainAnimation(float w1, float w2) {
	int m = 9;
	double rToA = 180 / 3.1415926;
	icurrk1 = modify_value(icurr + halfLocomotiveLength); //icurr + k
	icurrk2 = modify_value(icurr - halfLocomotiveLength); //icurr - k

	ux = trackPoints[icurrk1][0] - trackPoints[icurrk2][0];
	uz = trackPoints[icurrk1][1] - trackPoints[icurrk2][1];
	float theta = atan2(uz, -ux) * rToA;

	icurrm = modify_value(icurr - m);
	icurrmk1 = modify_value(icurrm + halfLocomotiveLength); //icurr - m + k
	icurrmk2 = modify_value(icurrm - halfLocomotiveLength); //icurr - m - k
	float ux1 = trackPoints[icurrmk2][0] - trackPoints[icurrmk1][0];
	float uz1 = trackPoints[icurrmk2][1] - trackPoints[icurrmk1][1];
	float theta1 = atan2(uz1, -ux1) * rToA;
	
	icurr2m = modify_value(icurr - 2 * m);
	icurr2mk1 = modify_value(icurr2m + halfLocomotiveLength);
	icurr2mk2 = modify_value(icurr2m - halfLocomotiveLength);
	float ux2 = trackPoints[icurr2mk2][0] - trackPoints[icurr2mk1][0];
	float uz2 = trackPoints[icurr2mk2][1] - trackPoints[icurr2mk1][1];
	float theta2 = atan2(uz2, -ux2) * rToA;
	
	icurr3m = modify_value(icurr - 3 * m);
	icurr3mk1 = modify_value(icurr3m + halfLocomotiveLength);
	icurr3mk2 = modify_value(icurr3m - halfLocomotiveLength);
	float ux3 = trackPoints[icurr3mk2][0] - trackPoints[icurr3mk1][0];
	float uz3 = trackPoints[icurr3mk2][1] - trackPoints[icurr3mk1][1];
	float theta3 = atan2(uz3, -ux3) * rToA;
	float spotDir[] = { -1.,-1.,0.,0. };
	float spotPosn[] = { -4.,9.,0.1,1. };

	// movement
	glPushMatrix();
	glTranslatef(trackPoints[icurr][0], 0., trackPoints[icurr][1]);
	glRotatef(theta, 0., 1., 0.); // rotate using ui
	engine(w1, w2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(trackPoints[icurr][0], 0., trackPoints[icurr][1]);
	glRotatef(theta, 0., 1., 0.); // rotate using ui
	glLightfv(GL_LIGHT0, GL_POSITION, spotPosn);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(trackPoints[icurrm][0], 0., trackPoints[icurrm][1]);
	glRotatef(theta1, 0., 1., 0.); 
	wagon(w1, w2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(trackPoints[icurr2m][0], 0., trackPoints[icurr2m][1]);
	glRotatef(theta2, 0., 1., 0.);  
	wagon(w1, w2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(trackPoints[icurr3m][0], 0., trackPoints[icurr3m][1]);
	glRotatef(theta3, 0., 1., 0.);
	wagon(w1, w2);
	glPopMatrix();
}

//--------------------------------------------------------------------------------------------------------------------------------
// train station

void stationBase() {
	
	glBegin(GL_QUADS);
	glColor3f(0.1f, 0.0f, 0.0f);//Brown
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

	glNormal3f(0.0, 1.0, 0.0);   //Facing +y (Top side)
	glVertex3f(-25.0, 5.0, 30);
	glVertex3f(25.0, 5.0, 30);
	glVertex3f(25.0, 5.0, -12);
	glVertex3f(-25.0, 5.0, -12);

	glNormal3f(-1.0, 0.0, 0.0);   //Facing -x  (Left side) 
	glVertex3f(-25.0, 0.0, 30);
	glVertex3f(-25.0, 0.0, -12);
	glVertex3f(-25.0, 5.0, -12);
	glVertex3f(-25.0, 5.0, 30);

	glNormal3f(1.0, 0.0, 0.0);   //Facing +x  (Right side) 
	glVertex3f(25.0, 0.0, 30);
	glVertex3f(25.0, 0.0, -12);
	glVertex3f(25.0, 5.0, -12);
	glVertex3f(25.0, 5.0, 30);

	glEnd();
}

void stationSupportBeam(float x, float y, float z) {
	glPushMatrix();
	glColor4f(0.1f, 0.0f, 0.0f, 0.6);//Brown
	glScalef(x, y, z);
	glutSolidCube(1.0);
	glPopMatrix();
}

void stationSign() {

	glPushMatrix();
	glTranslatef(-40., 0., -55.);
	glRotatef(45,0,1,0);

	glPushMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 0.0f); //white
	glTranslatef(0.,20.,0.);
	glBindTexture(GL_TEXTURE_2D, texId[9]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.04, 0.); glVertex3f(-11.0, 0.0, 0.);
	glTexCoord2f(1.05, 0.); glVertex3f(11.0, 0.0, 0.);
	glTexCoord2f(1.05, 1.); glVertex3f(11.0, 3.0, 0.);
	glTexCoord2f(0.04, 1.); glVertex3f(-11.0, 3.0, 0.);
	glEnd();
	glPopMatrix();

	GLUquadric* q = gluNewQuadric();   // sign pole
	glPushMatrix();
	glColor4f(1.0f, 1.f, 1.0f, 1.f);//black
	glTranslatef(0., 20., 0.);
	glRotatef(90,1.,0.,0.);
	gluCylinder(q, 0.4, 0.4, 20.0, 20, 5);
	glPopMatrix();

	glPopMatrix();
}

void stationCeilingSign() {
	glPushMatrix();
	glTranslatef(0., 26., -55);

	glPushMatrix();
	glColor3f(0.1f, 0.0f, 0.0f); //brown
	glTranslatef(-5.1, 0., 0.);
	stationSupportBeam(0.5, 4., 0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.1f, 0.0f, 0.0f); //brown
	glTranslatef(5.1, 0., 0.);
	stationSupportBeam(0.5, 4., 0.5);
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId[10]);
	glPushMatrix();
	glColor4f(1.0f, 1.0f, 0.0f, 0.0f); //yellow
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0., 0.); glVertex3f(-5.0, 0.0, 0.);
	glTexCoord2f(1., 0.); glVertex3f(5.0, 0.0, 0.);
	glTexCoord2f(1., 1.); glVertex3f(5.0, 4.4, 0.);
	glTexCoord2f(0., 1.); glVertex3f(-5.0, 4.4, 0.);
	glEnd();
	glPopMatrix();

	glPopMatrix();
}

void station()
{
	glPushMatrix();
	glTranslatef(0., 0., -75.);

	glEnable(GL_TEXTURE_2D);

	// draw station base
	stationBase();

	// draw support beam for station
	glPushMatrix();
	glTranslatef(-19.5, 11., 21.5);
	stationSupportBeam(1., 25., 1.);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(19.5, 11., 21.5);
	stationSupportBeam(1., 25., 1.);
	glPopMatrix();

	// draw station building
	glPushMatrix();
	glTranslatef(0., 13.5, 0.);
	
	glColor3f(1.0f, 1.0f, 1.0f); //white

	glBindTexture(GL_TEXTURE_2D, texId[8]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);   //Facing ??? (shelter board)
	glTexCoord2f(0., 0.); glVertex3f(-20.0, 15.0, 10);
	glTexCoord2f(1., 0.); glVertex3f(-20.0, 10.0, 22);
	glTexCoord2f(1., 1.); glVertex3f(20.0, 10.0, 22);
	glTexCoord2f(0., 1.); glVertex3f(20.0, 15.0, 10);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texId[8]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, -1.0);   //Facing -z (Back side)
	glTexCoord2f(0., 0.); glVertex3f(-20.0, -15.0, -10);
	glTexCoord2f(4., 0.); glVertex3f(20.0, -15.0, -10);
	glTexCoord2f(4., 2); glVertex3f(20.0, 15.0, -10);
	glTexCoord2f(0., 2); glVertex3f(-20.0, 15.0, -10);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, texId[8]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);   //Left side
	glTexCoord2f(0, 0); glVertex3f(-20.0, -15.0, 10);
	glTexCoord2f(2, 0); glVertex3f(-20.0, -15.0, -10);
	glTexCoord2f(2, 2); glVertex3f(-20.0, 15.0, -10);
	glTexCoord2f(0, 2); glVertex3f(-20.0, 15.0, 10);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texId[8]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);   //Right side
	glTexCoord2f(0, 0); glVertex3f(20.0, -15.0, 10);
	glTexCoord2f(2, 0); glVertex3f(20.0, -15.0, -10);
	glTexCoord2f(2, 2); glVertex3f(20.0, 15.0, -10);
	glTexCoord2f(0, 2); glVertex3f(20.0, 15.0, 10);
	glEnd();

	glm::vec3 p (-20.0, 15.0, 10);
	glm::vec3 q (20.0, 15.0, 10);
	glm::vec3 r (10.0, 30., 0);
	glm::vec3 v1(40, 0, 0); glm::vec3 v2(-10, 15, -10);
	glm::vec3 unit_v1 = normalize(v1);
	glm::vec3 unit_v2 = normalize(v2);
	glm::vec3 cprod = glm::cross(unit_v1, unit_v2);
	glBindTexture(GL_TEXTURE_2D, texId[8]);
	glBegin(GL_QUADS);
	glNormal3f(cprod[0], cprod[1], cprod[2]);   //Ceiling Front side
	glTexCoord2f(0., 0.); glVertex3f(-20.0, 15.0, 10);
	glTexCoord2f(1., 0.); glVertex3f(20.0, 15.0, 10);
	glTexCoord2f(0.75, 1.); glVertex3f(10.0, 30., 0);
	glTexCoord2f(0.25, 1.); glVertex3f(-10.0, 30.0, 0);
	glEnd();

	glm::vec3 p1(-20.0, 15.0, -10);
	glm::vec3 q1(20.0, 15.0, -10);
	glm::vec3 r1(10.0, 30., 0);
	glm::vec3 v3(40, 0, 0); glm::vec3 v4(-10, 15, 10);
	glm::vec3 unit_v3 = normalize(v3);
	glm::vec3 unit_v4 = normalize(v4);
	glm::vec3 cprod1 = glm::cross(unit_v3, unit_v4);
	glBindTexture(GL_TEXTURE_2D, texId[8]);
	glBegin(GL_QUADS);
	glNormal3f(cprod1[0], cprod1[1], cprod1[2]);   //Ceiling Back side
	glTexCoord2f(0., 0.); glVertex3f(-20.0, 15.0, -10);
	glTexCoord2f(1., 0.); glVertex3f(20.0, 15.0, -10);
	glTexCoord2f(0.75, 1.); glVertex3f(10.0, 30., 0);
	glTexCoord2f(0.25, 1.); glVertex3f(-10.0, 30.0, 0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texId[8]);
	glBegin(GL_TRIANGLES);
	glNormal3f(-1.0, 0.0, 0.0);   //Ceiling Left side
	glTexCoord2f(0.0, 0.5); glVertex3f(-20.0, 15.0, 10);
	glTexCoord2f(1.0, 0.0); glVertex3f(-20.0, 15.0, -10);
	glTexCoord2f(0.5, 1.0); glVertex3f(-10.0, 30.0, 0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texId[8]);
	glBegin(GL_TRIANGLES);
	glNormal3f(1.0, 0.0, 0.0);   //Ceiling Right side
	glTexCoord2f(0.0, 0.5); glVertex3f(20.0, 15.0, 10);
	glTexCoord2f(1.0, 0.0); glVertex3f(20.0, 15.0, -10);
	glTexCoord2f(0.5, 1.0); glVertex3f(10.0, 30., 0);
	glEnd();

	glPopMatrix();
	glPopMatrix();
}

void stationGallery() {
	
	glPushMatrix();
	glTranslatef(-5.,16.,-84.);
	glColor3f(1.0f, 1.0f, 1.0f); //white
	glBindTexture(GL_TEXTURE_2D, texId[11]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0., 0.); glVertex3f(-3.275, 0.0, 0.);
	glTexCoord2f(1., 0.); glVertex3f(3.275, 0.0, 0.);
	glTexCoord2f(1., 1.); glVertex3f(3.275, 5.78, 0.);
	glTexCoord2f(0., 1.); glVertex3f(-3.275, 5.78, 0.);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5., 16., -84.);
	glColor3f(1.0f, 1.0f, 1.0f); //white
	glBindTexture(GL_TEXTURE_2D, texId[12]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0., 0.); glVertex3f(-3.275, 0.0, 0.);
	glTexCoord2f(1., 0.); glVertex3f(3.275, 0.0, 0.);
	glTexCoord2f(1., 1.); glVertex3f(3.275, 5.78, 0.);
	glTexCoord2f(0., 1.); glVertex3f(-3.275, 5.78, 0.);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-15., 16., -84.);
	glColor3f(1.0f, 1.0f, 1.0f); //white
	glBindTexture(GL_TEXTURE_2D, texId[13]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0., 0.); glVertex3f(-3.275, 0.0, 0.);
	glTexCoord2f(1., 0.); glVertex3f(3.275, 0.0, 0.);
	glTexCoord2f(1., 1.); glVertex3f(3.275, 5.78, 0.);
	glTexCoord2f(0., 1.); glVertex3f(-3.275, 5.78, 0.);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15., 16., -84.);
	glColor3f(1.0f, 1.0f, 1.0f); //white
	glBindTexture(GL_TEXTURE_2D, texId[14]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0., 0.); glVertex3f(-3.275, 0.0, 0.);
	glTexCoord2f(1., 0.); glVertex3f(3.275, 0.0, 0.);
	glTexCoord2f(1., 1.); glVertex3f(3.275, 5.78, 0.);
	glTexCoord2f(0., 1.); glVertex3f(-3.275, 5.78, 0.);
	glEnd();
	glPopMatrix();
}

//--------------------------------------------------------------------------------------------------------------------------------
// tunnel 
void normal(float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3)
{
	float nx, ny, nz;
	nx = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
	ny = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
	nz = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);

	glNormal3f(nx, ny, nz);
}

void tunnel() {
	const int N = 40;  // Total number of vertices on the base curve
	float vx_init[N] = { 0 };
	float vy_init[N] = { 0., 2.5, 5., 7.5, 10., 12.5, 15.,17.5, 20., 22.5, 23., 23.,23., 23.,23.,23.,23.,23.,23.,23.,23.,23.,23.,23.,23.,23.,23.,23.,23.,23.,22.5,20.,17.5,15.,12.5,10.,7.5,5.,2.5, 0. };
	float vz_init[N] = { 30.,30., 30., 30., 30., 30., 30., 30., 30., 30., 31.,32.,33.,34.,35.,36.,37.,38.,39.,40.,41.,42.,43.,44.,45.,46.,47.,48.,49.,50.,50., 50., 50., 50., 50., 50., 50., 50., 50., 50. };
	const int M = 35;
	float vx_init1[M] = { 0 };
	float vy_init1[M] = { 0., 2.5, 5., 7.5, 10., 12.5, 15.,16.5, 17.5, 18.5, 19., 19.,19., 19., 19., 19., 19., 19., 19., 19., 19., 19., 19., 19.,19., 18.5,17.5,16.5, 15., 12.5,10.,7.5, 5.,   2.5, 0. };
	float vz_init1[M] = { 33.,33., 33.,33., 33., 33., 33., 33.,  33.,  33., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 44., 45., 46.,47. ,47., 47., 47.,  47., 47., 47., 47., 47., 47., 47.};

	float vx[N], vy[N], vz[N];   //vertex positions
	float wx[N], wy[N], wz[N];
	float vx1[M], vy1[M], vz1[M];   //vertex positions
	float wx1[M], wy1[M], wz1[M];
	int d = 2; 

	// outside wall of tunnel
	for (int i = 0; i < N; i++)     //Initialize data everytime the frame is refreshed
	{
		vx[i] = vx_init[i];
		vy[i] = vy_init[i];
		vz[i] = vz_init[i];
	}

	int nslices = 20;
	for (int slice = 0; slice < nslices; slice++) {

		for (int i = 0; i < N; i++)
		{
			wx[i] = vx[i] + d;
			wy[i] = vy[i];
			wz[i] = vz[i];
		}

		glNormal3f(0, 1, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_QUAD_STRIP);
		glColor4f(1.0, 1.0, 1.0, 1.0);
		for (int i = 1; i < N; i++)
		{
			normal(vx[i - 1], vy[i - 1], vz[i - 1],
				vx[i], vy[i], vz[i],
				wx[i], wy[i], wz[i]);
			if (i == 1) {
				//glTexCoord2f((float)i / (float)(NSIDE - 1), 0);
				glVertex3f(vx[0], vy[0], vz[0]);
				//glTexCoord2f((float)i / (float)(NSIDE - 1), 1);
				glVertex3f(wx[0], wy[0], wz[0]);
			}
			glVertex3f(vx[i], vy[i], vz[i]);
			glVertex3f(wx[i], wy[i], wz[i]);
		}
		glEnd();

		for (int i = 0; i < N; i++) {
			vx[i] = wx[i];
			vy[i] = wy[i];
			vz[i] = wz[i];
		}
	}

	// inner wall of tunnel
	for (int i = 0; i < M; i++)     //Initialize data everytime the frame is refreshed
	{
		vx1[i] = vx_init1[i];
		vy1[i] = vy_init1[i];
		vz1[i] = vz_init1[i];
	}

	for (int slice = 0; slice < nslices; slice++) {

		for (int i = 0; i < M; i++)
		{
			wx1[i] = vx1[i] + d;
			wy1[i] = vy1[i];
			wz1[i] = vz1[i];
		}

		glNormal3f(0, 1, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_QUAD_STRIP);
		glColor4f(0.0, 0.0, 0.0, 0.0); // black color so when using driver's view, can see light that placed on engine head clearly
		for (int i = 1; i < M; i++)
		{
			normal(vx1[i - 1], vy1[i - 1], vz1[i - 1],
				vx1[i], vy1[i], vz1[i],
				wx1[i], wy1[i], wz1[i]);
			if (i == 1) {
				//glTexCoord2f((float)i / (float)(NSIDE - 1), 0);
				glVertex3f(vx1[0], vy1[0], vz1[0]);
				//glTexCoord2f((float)i / (float)(NSIDE - 1), 1);
				glVertex3f(wx1[0], wy1[0], wz1[0]);
			}
			glVertex3f(vx1[i], vy1[i], vz1[i]);
			glVertex3f(wx1[i], wy1[i], wz1[i]);
		}
		glEnd();

		for (int i = 0; i < M; i++) {
			vx1[i] = wx1[i];
			vy1[i] = wy1[i];
			vz1[i] = wz1[i];
		}

		// conceal +x tunnel 
		glBindTexture(GL_TEXTURE_2D, texId[15]);
		glBegin(GL_QUADS);
		glColor4f(0.4, 0.4, 0.4, 1.0);
		glNormal3f(1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0, 0, 29.9);
		glTexCoord2f(1.0, 0.0); glVertex3f(0, 0., 33.1);
		glTexCoord2f(1.0, 5.0); glVertex3f(0, 19.1, 33.1);
		glTexCoord2f(0.0, 5.0); glVertex3f(0, 19.1, 29.9);
		glEnd();

		glBegin(GL_QUADS);
		glBindTexture(GL_TEXTURE_2D, texId[15]);
		glColor4f(0.4, 0.4, 0.4, 1.0);
		glNormal3f(1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0, 0, 47.1);
		glTexCoord2f(1.0, 0.0); glVertex3f(0, 0., 50.1);
		glTexCoord2f(1.0, 5.0); glVertex3f(0, 19.1, 50.1);
		glTexCoord2f(0.0, 5.0); glVertex3f(0, 19.1, 47.1);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texId[15]);
		glBegin(GL_QUADS);
		glColor4f(0.4, 0.4, 0.4, 1.0);
		glNormal3f(1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0, 19.1, 29.9);
		glTexCoord2f(5.0, 0.0); glVertex3f(0, 19., 50.1);
		glTexCoord2f(5.0, 1.0); glVertex3f(0, 23.1, 50.1);
		glTexCoord2f(0.0, 1.0); glVertex3f(0, 23.1, 29.9);
		glEnd();

		// conceal -x tunnel
		glBindTexture(GL_TEXTURE_2D, texId[15]);
		glBegin(GL_QUADS);
		glColor4f(0.4, 0.4, 0.4, 1.0);
		glNormal3f(-1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(40.1, 0, 29.9);
		glTexCoord2f(1.0, 0.0); glVertex3f(40.1, 0., 33.1);
		glTexCoord2f(1.0, 5.0); glVertex3f(40.1, 19, 33.1);
		glTexCoord2f(0.0, 5.0); glVertex3f(40.1, 19, 29.9);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texId[15]);
		glBegin(GL_QUADS);
		glColor4f(0.4, 0.4, 0.4, 1.0);
		glNormal3f(-1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(40.1, 0, 47.1);
		glTexCoord2f(1.0, 0.0); glVertex3f(40.1, 0., 50.1);
		glTexCoord2f(1.0, 5.0); glVertex3f(40.1, 19.1, 50.1);
		glTexCoord2f(0.0, 5.0); glVertex3f(40.1, 19.1, 47.1);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texId[15]);
		glBegin(GL_QUADS);
		glColor4f(0.4, 0.4, 0.4, 1.0);
		glNormal3f(-1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(40.1, 19.1, 29.9);
		glTexCoord2f(5.0, 0.0); glVertex3f(40.1, 19.1, 50.1);
		glTexCoord2f(5.0, 1.0); glVertex3f(40.1, 23.1, 50.1);
		glTexCoord2f(0.0, 1.0); glVertex3f(40.1, 23.1, 29.9);
		glEnd();

		// blocks for texture
		glBindTexture(GL_TEXTURE_2D, texId[15]);
		glBegin(GL_QUADS);
		glColor4f(0.4, 0.4, 0.4, 1.0);
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0, 0, 50.1);
		glTexCoord2f(5.0, 0.0); glVertex3f(40.1, 0., 50.1);
		glTexCoord2f(5.0, 5.0); glVertex3f(40.1, 23.1, 50.1);
		glTexCoord2f(0.0, 5.0); glVertex3f(0, 23.1, 50.1);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texId[15]);
		glBegin(GL_QUADS);
		glColor4f(0.4, 0.4, 0.4, 1.0);
		glNormal3f(0.0, 0.0, -1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0., 0., 29.9);
		glTexCoord2f(5.0, 0.0); glVertex3f(40.1, 0., 29.9);
		glTexCoord2f(5.0, 5.0); glVertex3f(40.1, 23.1, 29.9);
		glTexCoord2f(0.0, 5.0); glVertex3f(0., 23.1, 29.9);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texId[15]);
		glBegin(GL_QUADS);
		glColor4f(0.4, 0.4, 0.4, 1.0);
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0, 23.1, 29.9);
		glTexCoord2f(6.0, 0.0); glVertex3f(0, 23.1, 50.1);
		glTexCoord2f(6.0, 5.0); glVertex3f(40.1, 23.1, 50.1);
		glTexCoord2f(0.0, 5.0); glVertex3f(40.1, 23.1, 29.9);
		glEnd();
	}
}

//--------------------------------------------------------------------------------------------------------------------------------
// barriers 
void barrierBase() {
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);//white
	glPushMatrix();
	glTranslatef(0.,5.,0.);
	glScalef(1, 10, 1);
	glutSolidCube(1.0);
	glPopMatrix();
}

void barrierArm() {
	glColor4f(1.0f, 0.0f, 0.0f, 0.5f);//red
	glPushMatrix();
	glTranslatef(0., 20., 0.);
	// the whole barrier arm, red color
	glPushMatrix();
	glScalef(0.5,20, 1.);
	glutSolidCube(1.0);
	glPopMatrix();
	// white segments on arm
	glColor4f(1.0f, 1.0f, 1.0f, 1.f); //white
	glPushMatrix();
	glTranslatef(0., 1, 0.);
	glScalef(0.51, 2, 1.);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0., 5, 0.);
	glScalef(0.51, 2, 1.);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0., 9, 0.);
	glScalef(0.51, 2, 1.);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0., -3, 0.);
	glScalef(0.51, 2, 1.);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0., -7, 0.);
	glScalef(0.51, 2, 1.);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
}

void leftBarrierMovement() {

	//left barrier near station side
	if (icurr >= 0 && icurr <= 275) {
		glPushMatrix();
		glTranslatef(-85., 0., -10.);

		glPushMatrix();
		glTranslatef(0, 10, 0);
		glRotatef(armAngleLeft, 1, 0, 0); // positive x towards tunnel
		glTranslatef(0, -10, 0);
		barrierArm();
		glPopMatrix();

		glPushMatrix();
		barrierBase();
		glPopMatrix();
		glPopMatrix();

		//left barrier near tunnel side
		glPushMatrix();
		glTranslatef(-115., 0., 10.);

		glPushMatrix();
		glTranslatef(0, 10, 0);
		glRotatef(armAngleLeft, -1, 0, 0); // positive x towards tunnel
		glTranslatef(0, -10, 0);
		barrierArm();
		glPopMatrix();

		glPushMatrix();
		barrierBase();
		glPopMatrix();
		glPopMatrix();
	}
	else {
		glPushMatrix();
		glTranslatef(-85., 0., -10.);
		barrierArm();
		barrierBase();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-115., 0., 10.);
		barrierArm();
		barrierBase();
		glPopMatrix();
	}
}

void rightBarrierMovement() {
	//right barrier near station side
	if (icurr <= 16 || icurr >= 240) {
		glPushMatrix();
		glTranslatef(85., 0., -10.);

		glPushMatrix();
		glTranslatef(0, 10, 0);
		glRotatef(armAngleRight, 1, 0, 0); // positive x towards tunnel
		glTranslatef(0, -10, 0);
		barrierArm();
		glPopMatrix();

		glPushMatrix();
		barrierBase();
		glPopMatrix();
		glPopMatrix();

		// right side near tunnel side
		glPushMatrix();
		glTranslatef(115., 0., 10.);

		glPushMatrix();
		glTranslatef(0, 10, 0);
		glRotatef(armAngleRight, -1, 0, 0); // positive x towards tunnel
		glTranslatef(0, -10, 0);
		barrierArm();
		glPopMatrix();

		glPushMatrix();
		barrierBase();
		glPopMatrix();
		glPopMatrix();
	}
	else {
		glPushMatrix();
		glTranslatef(85., 0., -10.);
		barrierArm();
		barrierBase();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(115., 0., 10.);
		barrierArm();
		barrierBase();
		glPopMatrix();
	}
}

//--------------------------------------------------------------------------------------------------------------------------------
// signalling light

void signalLight() {
	glColor3f(0.0f, 0.0f, 0.0f);//black

	glPushMatrix();
	glTranslatef(-85, 0., -12.5); // move signal light from origin to place next to railway
	glRotatef(-90, 0.,1.0, 0.);
	glTranslatef(0., 7.5, 0.);

	//base pole
	glPushMatrix(); 
	glScalef(0.5, 15, 0.5);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0., 7.5, 0.);
	//top cube
	glPushMatrix();
	glScalef(2, 8, 2);
	glutSolidCube(1.0);
	glPopMatrix();

	glColor3f(redlight, 0.0f, 0.0f);
	GLUquadric* q1 = gluNewQuadric();   //Disc
	glPushMatrix();
	glTranslatef(0., 2., 1.1);
	gluDisk(q1, 0.0, 0.8, 20, 2); // outer r = 1.5 unit
	glPopMatrix();
	
	glColor3f(0.0f, greenlight, 0.0f);
	GLUquadric* q2 = gluNewQuadric();   //Disc
	glPushMatrix();
	glTranslatef(0., -2., 1.1);
	gluDisk(q2, 0.0, 0.8, 20, 2); // outer r = 1.5 unit
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();

}

void display(void)
{
	const float w1 = 2.0; const float w2 = 3.0;
	float lgt1_pos[] = { 0.0f, 40.0f, 0.0f, 0.0f };  //light0 position (directly above the origin) Omni-directional point sources
	glLightfv(GL_LIGHT1, GL_POSITION, lgt1_pos);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	int viewPoint = modify_value(icurrk1 + 15);
	if (viewFlag == 1) {
		x_pos = trackPoints[icurrk1][0]; y_pos = 8., z_pos = trackPoints[icurrk1][1];
		look_x = trackPoints[viewPoint][0]; look_y = 10.; look_z = trackPoints[viewPoint][1];
		y_angle = 10;
	}
	else if (viewFlag == 2 && refreshFlag == 0) {
		x_pos = -150.0; y_pos = 28., z_pos = 0.;
		look_x = 100.; look_y = 0.; look_z = 0.;
		y_angle = 10;
		refreshFlag = 1;
	}
	else if (viewFlag == 0 && refreshFlag == 1) {
		x_pos = -40; y_pos = 18., z_pos = -53.;
		look_x = 100.; look_y = 0.; look_z = 0.;
		y_angle = 10;
		refreshFlag = 0;
	}
	
	gluLookAt(x_pos, y_pos, z_pos, look_x, look_y, look_z, 0, 1, 0);
	glRotatef(y_angle, 0.0, 1.0, 0.0);	//rotate the object about the y-axis

	road();
	skybox();
	tunnel();
	stationGallery();
	stationSign();
	stationCeilingSign();
	station();
	glDisable(GL_TEXTURE_2D);

	track(w1, w2);
	sleepers(4.5,0.5);
	trainAnimation(w1, w2);
	leftBarrierMovement();
	rightBarrierMovement();
	signalLight();

	glutSwapBuffers();   //Useful for animation
}

//--------------------------------------------------------------------------------------------------------------------------------
//Parameter/state initialization
void initialize(void) 
{
	glClearColor(0.0, 0.0, 0.0, 0.0);  //Background colour
	loadTexture();
	float grey[4] = { 0.2, 0.2, 0.2, 1.0 };
	float white[4] = { 1.0, 1.0, 1.0, 1.0 };
	//float yellow[4] = { 1.0f, 1.0f, 0.0f, 0.0f };
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); // light on train engine
	glEnable(GL_LIGHT1); // scene light 
	//	Define light's ambient, diffuse, specular properties
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1.);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightfv(GL_LIGHT1, GL_AMBIENT, white);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, grey);
	glLightfv(GL_LIGHT1, GL_SPECULAR, grey);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	//glClearColor(0.0, 0.0, 0.0, 0.0);  //Background colour
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70., 1.0, 10.0, 1000.0);   //Perspective projection

	// transfer txt file that contains track median line points to a buffer
	storeTrackMedianLinePointsToArray();
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
	glutMouseFunc(mouse);
	glutTimerFunc(50, myTimer, 0);
	glutMainLoop();
	return 0;
}
