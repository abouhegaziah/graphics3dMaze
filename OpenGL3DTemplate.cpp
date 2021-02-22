#include <time.h>
#include<iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <random>
#include<Windows.h>
#include<MMSystem.h>



#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)
using namespace std;

bool done = false;

double goalX;
double goalZ;
bool goalDrawn = false;

int timee = -20;

double playerX = 0.5;
double playerZ = 0.8;

double playerMelt = 0;


bool is_e_pressed = false;
bool is_r_pressed = false;
bool is_t_pressed = false;
bool is_y_pressed = false;
bool is_u_pressed = false;
bool is_i_pressed = false;


float rotangl = 0;
bool rotanglB = true;

float rotangl2 = 0;
bool rotangl2B = true;


float rotangl3 = 0;
bool rotangl3B = true;

float rotangl4 = 0;
bool rotangl4B = true;

float rotangl5 = 0;
bool rotangl5B = true;

float rotangl6 = 0;
bool rotangl6B = true;

double fenceR = (double)(rand() % 2) ;
double fenceG = (double)(rand() % 2);
double fenceB = (double)(rand() % 2);

class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 1.0f, float eyeY = 1.0f, float eyeZ = 1.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
		eye.x = 0.507887;
		eye.y = 0.719155;
		eye.z = 1.27969;
		center.x = 0.498003;
		center.y = 0.309974;
		center.z = 0.360699;
		up.x = 0.00173088;
		up.y = 0.918997;
		up.z = -0.39426;
	}

	void moveX(float d) {
		/*cout << "eyeX   " << eye.x  << endl;
		cout << "eye Y  " << eye.y << endl;
		cout << "eye Z  " << eye.z << endl;
		cout << "Center X   " << center.x << endl;
		cout << "Center Y " << center.y << endl;
		cout << "Center Z " << center.z << endl;

		cout << "upX      " << up.x << endl;
		cout << "upY      " << up.y << endl;
		cout << "upZ      " << up.z << endl;*/


		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void front() {
		eye.x = 0.507887;
		eye.y = 0.719155;
		eye.z = 1.27969;
		center.x = 0.498003;
		center.y = 0.309974;
		center.z = 0.360699;
		up.x = 0.00173088;
		up.y = 0.918997;
		up.z = -0.39426;
	}
	void top() {
		eye.x = 0.56518;
		eye.y = 1.60668;
		eye.z = 0.390157;
		center.x = 0.523749;
		center.y = 0.617217;
		center.z = 0.251428;
		up.x = -0.00870141;
		up.y = 0.1392;
		up.z = -0.990226;
	}
	void side1() {
		eye.x = 1.47761;
		eye.y = 0.614124;
		eye.z = 0.407803;
		center.x = 0.500466;
		center.y = 0.403429;
		center.z = 0.436161;
		up.x = -0.211346;
		up.y = 0.977156;
		up.z = -0.0223291;
	}
	void side2() {
		eye.x = -0.820023;
		eye.y = 0.610852;
		eye.z = 0.470015;
		center.x = 0.177794;
		center.y = 0.545157;
		center.z = 0.476665;
		up.x = 0.065693;
		up.y = 0.99784;
		up.z = 0.000437761;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
};

Camera camera;

void drawWall(double thickness) {
	glPushMatrix();
	glTranslated(0.5, 0.5 * thickness, 0.5);
	glScaled(1.0, thickness, 1.0);
	glutSolidCube(1);
	glPopMatrix();
}
void drawTableLeg(double thick, double len) {
	glPushMatrix();
	glTranslated(0, len / 2, 0);
	glScaled(thick, len, thick);
	glutSolidCube(1.0);
	glPopMatrix();
}
void drawJackPart() {
	glPushMatrix();
	glScaled(0.2, 0.2, 1.0);
	glutSolidSphere(1, 15, 15);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 0, 1.2);
	glutSolidSphere(0.2, 15, 15);
	glTranslated(0, 0, -2.4);
	glutSolidSphere(0.2, 15, 15);
	glPopMatrix();
}
void drawJack() {
	glPushMatrix();
	drawJackPart();
	glRotated(90.0, 0, 1, 0);
	drawJackPart();
	glRotated(90.0, 1, 0, 0);
	drawJackPart();
	glPopMatrix();
}
void drawTable(double topWid, double topThick, double legThick, double legLen) {
	glPushMatrix();
	glTranslated(0, legLen, 0);
	glScaled(topWid, topThick, topWid);
	glutSolidCube(1.0);
	glPopMatrix();

	double dist = 0.95 * topWid / 2.0 - legThick / 2.0;
	glPushMatrix();
	glTranslated(dist, 0, dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(-2 * dist, 0, 2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glPopMatrix();
}
void drawRect(int x, int y, int w, int h) {
	glBegin(GL_POLYGON);
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y + h);
	glVertex2f(x, y + h);
	glEnd();
}
void setupLights() {
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}
void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}


double GenerateRandom(double min, double max)
{
	static bool first = true;
	if (first)
	{
		srand(time(NULL));
		first = false;
	}
	if (min > max)
	{
		std::swap(min, max);
	}
	return min + (double)rand() * (max - min) / (double)RAND_MAX;
}

void Timer(int value) {
	//cout << "ana d5lt wlahi" << endl;
	fenceR = GenerateRandom(0, 1);
	fenceG = GenerateRandom(0, 1);
	fenceB = GenerateRandom(0, 1);
	//cout << fenceB << endl;
	//cout << fenceR << endl;
	//cout << fenceG << endl;
	timee += 20;
	if (timee == 40)
		playerMelt += 0.1;
	if (timee == 80)
		done = true;

	glutPostRedisplay();
	glutTimerFunc(20 * 1000, Timer, 0);

}

void drawCube() {
	glPushMatrix();
	glTranslated((0.01*100)/2,0, (0.01 * 100) / 2); // big cube at (0.5, 0.5, 0.5)
	glScalef(100, 1, 100);
	glutSolidCube(0.01);
	glPopMatrix();
}
void drawFence() {
	glPushMatrix();
	glTranslated((0.01*5)/2, (0.01 * 30) / 2, (0.01 * 5) / 2); // big cube at (0.5, 0.5, 0.5)
	glScalef(5, 30, 5);
	glutSolidCube(0.01);
	glPopMatrix();
}
void drawTree() {
	glPushMatrix();
	glColor3f(0.647059, 0.164706, 0.164706);
	glTranslated(0, 0,0); // cylinder at (0,0,1)  
	glRotatef(-90, 1, 0, 0);
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj, 0.03, 0.03, 0.6, 8, 8);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 1, 0);
	glTranslated(0.0, 0.6, 0); // sphere at (1,1,0)  
	glutSolidSphere(0.2, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 0.8, 0); // sphere at (1,1,0)  
	glutSolidSphere(0.2, 10, 10);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0.0, 0.7, 0.1); // sphere at (1,1,0)  
	glutSolidSphere(0.2, 10, 10);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0.0, 0.7, -0.1); // sphere at (1,1,0)  
	glutSolidSphere(0.2, 10, 10);
	glPopMatrix();
}
void drawBench() {
	glPushMatrix();
	glTranslated(0.025, (0.01 * 18)-0.03,0.02); // big cube at (0.5, 0.5, 0.5)
	glScalef(5, 18, 5);
	glutSolidCube(0.01);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.028, (0.01 * 18) - 0.03, 0.15); // big cube at (0.5, 0.5, 0.5)
	glScalef(5, 18, 5);
	glutSolidCube(0.01);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.28, (0.01 * 18) - 0.03, 0.15); // big cube at (0.5, 0.5, 0.5)
	glScalef(5, 18, 5);
	glutSolidCube(0.01);
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(0.28, (0.01 * 18) - 0.03, 0); // big cube at (0.5, 0.5, 0.5)
	glScalef(5, 18, 5);
	glutSolidCube(0.01);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.15, (0.01 * 18) +0.1, 0.08); // big cube at (0.5, 0.5, 0.5)
	glScalef(30, 8, 20);
	glutSolidCube(0.01);
	glPopMatrix();
	
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslated(0.15,0,0.4); // big cube at (0.5, 0.5, 0.5)
	glScalef(30, 5, 15);
	glutSolidCube(0.01);
	glPopMatrix();



}
void drawPergula() {
	glPushMatrix();
	glColor3f(0.35, 0.16, 0.14);
	glTranslated(0, (0.01 * 40)-0.1 ,0); // big cube at (0.5, 0.5, 0.5)
	glScalef(3, 40, 3);
	glutSolidCube(0.01);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.35, (0.01 * 40) - 0.1, 0); // big cube at (0.5, 0.5, 0.5)
	glScalef(3, 40, 3);
	glutSolidCube(0.01);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.175, (0.01 * 40)+0.05 , -0.01); // big cube at (0.5, 0.5, 0.5)
	glScalef(40, 5, 5);
	glutSolidCube(0.01);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.13, (0.01 * 20) +0.15, 0); // big cube at (0.5, 0.5, 0.5)
	glScalef(2, 22, 2);
	glutSolidCube(0.01);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.23, (0.01 * 20) + 0.15, 0); // big cube at (0.5, 0.5, 0.5)
	glScalef(2, 22, 2);
	glutSolidCube(0.01);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(0.175, (0.01 * 20) + 0.05, 0); // big cube at (0.5, 0.5, 0.5)
	glScalef(20, 3, 10);
	glutSolidCube(0.01);
	glPopMatrix();
}
void drawUmbrella() {

	glPushMatrix();
	glColor3f(0.35, 0.16, 0.14);
	glRotatef(-90, 1, 0, 0);
	glTranslated(0, 0, 0.1); // cylinder at (0,0,1)  
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj, 0.02, 0.02, 0.55, 8, 8);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.74902, 0.847059, 0.847059);
	glTranslated(0, 0.5, 0); // cone at (1,0,1)
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(0.2, 0.2, 10, 8);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, (0.01 * 20) + 0.08, 0); // big cube at (0.5, 0.5, 0.5)
	glScalef(10, 3, 10);
	glutSolidCube(0.01);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(0, 0.68, 0); // sphere at (1,1,0)  
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();


}
void drawGrass() {
	for (float i = 0.05;i < 1;i += 0.02) {
		for (float j = 0.05;j < 1;j += 0.03) {
			glPushMatrix();
			glColor3f(0, 1, 0);
			glTranslated(i, -0.03, j); // cone at (1,0,1)
			glRotatef(-90, 1, 0, 0);
			glutSolidCone(0.03, 0.15, 10, 8);
			glPopMatrix();
		}
	}

}
void drawGoal() {
	if (!goalDrawn) {
	
		goalX = GenerateRandom(0, 1);
		goalZ = GenerateRandom(0, 1);
		/*cout << x << endl;
		cout << z << endl;*/
		if (goalZ < 0.2)
			goalZ +=0.1;
		if (goalX < 0.1)
			goalX += 0.1;
		if (goalX > 0.9)
			goalX -= 0.2;

		/*cout << x << endl;
		cout << z << endl;*/
		goalDrawn = true;
	}
	//fenceB = GenerateRandom(0, 1);
	glPushMatrix();
	glColor3f(0.89, 0.47, 0.20);
	glTranslated(goalX, 0.4, goalZ); // cone at (1,0,1)
	glRotatef(90, 1, 0, 0);
	glutSolidCone(0.02, 0.15, 10, 8);
	glPopMatrix();
}
void drawPlayer() {

	glPushMatrix();
	glColor3f(0.74902, 0.847059, 0.847059);
	glTranslated(0.0, 0.25, 0); // sphere at (1,1,0)  
	glutSolidSphere(0.15, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 0.45, 0); // sphere at (1,1,0)  
	glutSolidSphere(0.10, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 0.58, 0); // sphere at (1,1,0)  
	glutSolidSphere(0.06, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(-0.001, 0.6, 0.06); // sphere at (1,1,0)  
	glutSolidSphere(0.01, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0.03, 0.6, 0.05); // sphere at (1,1,0)  
	glutSolidSphere(0.01, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0.033, 0.56, 0.055); // sphere at (1,1,0)  
	glutSolidSphere(0.01, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0.016, 0.56, 0.055); // sphere at (1,1,0)  
	glutSolidSphere(0.01, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0.00, 0.56, 0.055); // sphere at (1,1,0)  
	glutSolidSphere(0.01, 10, 10);
	glPopMatrix();
}


void Display() {
	setupCamera();
	setupLights();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (done) {
		glColor3f(0, 0, 0);
		drawRect(-100, -100, 650, 650);
		goalX = 10000;
		goalZ = 1000000000;
	}
//<<<<<<<<<<<<<<<<<<<floor>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	glPushMatrix();
	glColor3f(0,1 , 0);
	drawWall(0.1);
	glPopMatrix();

	drawGoal();
	glPushMatrix();
	glRotatef(rotangl6, 0, 1, 0);
    drawGrass();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX, 0, playerZ);
	glScalef(0.8-playerMelt, 0.8-playerMelt, 1);
	drawPlayer();
	glPopMatrix();

//<<<<<<<<<<<<<<<<<<<<<<<DRAW UMBRELLA>>>>>>>>>>>>>>>>>>>>>>>>
	glPushMatrix();
	glTranslatef(0.2,0,0.8);
	glRotatef(rotangl, 0, 1, 0);
	drawUmbrella();
	glPopMatrix();


//<<<<<<<<<<<<<<<<<<<<<<DRAW PERGULA>>>>>>>>>>>>>>>>>>>>>>>>
	glPushMatrix();
	glRotatef(-90, 0, 1, 0);
	glTranslatef(0.5,0,-0.8);
	glRotatef(rotangl3, 0, 1, 0);
	drawPergula();
	glPopMatrix();


//<<<<<<<<<<<<<<<<<<<<DRAW BENCH>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	glPushMatrix();
	glColor3f(0.647059, 0.164706, 0.164706);
	glTranslatef(0.35, 0, 0.2);
	glRotatef(rotangl4, 0, 1, 0);
	glScalef(1, 0.8, 1);
	drawBench();
	glPopMatrix();

//<<<<<<<<<<<<<<<<<<<<<<DRAW TREES>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	glPushMatrix();
	glTranslatef(0.2,0,0.2);
	glRotatef(rotangl2, 0, 1, 0);
	drawTree();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.85, 0, 0.2);
	glRotatef(rotangl2, 0, 1, 0);
	drawTree();
	glPopMatrix();

	
//<<<<<<<<<<<<<<<<<<<<DRAW LEFT FENCE>>>>>>>>>>>>>>>>>>>>>>>>>
	for (float i = 0;i < 1;i += 0.1) {
		glPushMatrix();
		glColor3f(fenceR, fenceG, fenceB);
		glRotatef(rotangl5, 0, 1, 0);
		glTranslated(0, 0, i);
		drawFence();
		glPopMatrix();
	}
//<<<<<<<<<<<<<<<<<<<<<<DRAW MID FENCE>>>>>>>>>>>>>>>>>>>>>>>>
	for (float i = 0;i < 1;i += 0.1) {
		glPushMatrix();
		glRotatef(rotangl5, 0, 1, 0);
		glTranslated(i, 0, 0);
		drawFence();
		glPopMatrix();
	}
	//<<<<<<<<<<<<<<<<<<<<<<DRAW RIGHT FENCE>>>>>>>>>>>>>>>>>>>>>
	for (float i = 0;i < 1;i += 0.1) {
		glPushMatrix();
		glRotatef(rotangl5, 0, 1, 0);
		glTranslated(0.95, 0, i);
		drawFence();
		glPopMatrix();
	}


	
	glFlush();
}

void Anim() {
	if (done) {
		is_e_pressed=false;
		is_r_pressed = false;
		is_t_pressed = false;
		is_y_pressed = false;
		is_u_pressed = false;
		is_i_pressed = false;
	}
	if (is_e_pressed) {
		
		if (rotanglB) {
			rotangl += 0.1;
			if (rotangl > 100) {
				//cout << rotangl << endl;
				rotanglB = !rotanglB;
			}
		}
		else {
			rotangl -= 0.1;
			if (rotangl < 0)
				rotanglB = !rotanglB;
		}
	}
	if (is_u_pressed) {

		if (rotangl5B) {
			rotangl5 += 0.1;
			if (rotangl5 > 100) {
				//cout << rotangl << endl;
				rotangl5B = !rotangl5B;
			}
		}
		else {
			rotangl5 -= 0.1;
			if (rotangl5 < 0)
				rotangl5B = !rotangl5B;
		}
	}
	if (is_i_pressed) {

		if (rotangl6B) {
			rotangl6 += 0.1;
			if (rotangl6 > 100) {
				//cout << rotangl << endl;
				rotangl6B = !rotangl6B;
			}
		}
		else {
			rotangl6 -= 0.1;
			if (rotangl6 < 0)
				rotangl6B = !rotangl6B;
		}
	}
	if (is_r_pressed) {
		if (rotangl3B) {
			rotangl3 += 0.1;
			if (rotangl3 > 100) {
				//cout << rotangl << endl;
				rotangl3B = !rotangl3B;
			}
		}
		else {
			rotangl3 -= 0.1;
			if (rotangl3 < 0)
				rotangl3B = !rotangl3B;
		}
	}
	if (is_t_pressed) {

		if (rotangl2B) {
			rotangl2 += 0.1;
			if (rotangl2 > 100) {
				//cout << rotangl << endl;
				rotangl2B = !rotangl2B;
			}
		}
		else {
			rotangl2 -= 0.1;
			if (rotangl2 < 0)
				rotangl2B = !rotangl2B;
		}
	}
	if (is_y_pressed) {
		if (rotangl4B) {
			rotangl4 += 0.1;
			if (rotangl4 > 100) {
				//cout << rotangl << endl;
				rotangl4B = !rotangl4B;
			}
		}
		else {
			rotangl4 -= 0.1;
			if (rotangl4 < 0)
				rotangl4B = !rotangl4B;
		}
	}


	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y) {
	float d = 0.01;

	switch (key) {
	case 'e':
		PlaySound(TEXT("bleep.wav"), NULL, SND_ASYNC | SND_FILENAME);
		is_e_pressed = !is_e_pressed;
		break;
	case 'r':
		PlaySound(TEXT("bleep.wav"), NULL, SND_ASYNC | SND_FILENAME);
		is_r_pressed = !is_r_pressed;
		break;
	case 't':
		PlaySound(TEXT("bleep.wav"), NULL, SND_ASYNC | SND_FILENAME);
		is_t_pressed = !is_t_pressed;
		break;
	case 'y':
		PlaySound(TEXT("bleep.wav"), NULL, SND_ASYNC | SND_FILENAME);
		is_y_pressed = !is_y_pressed;
		break;
	case 'u':
		PlaySound(TEXT("bleep.wav"), NULL, SND_ASYNC | SND_FILENAME);
		is_u_pressed = !is_u_pressed;
		break;
	case 'i':
		PlaySound(TEXT("bleep.wav"), NULL, SND_ASYNC | SND_FILENAME);
		is_i_pressed = !is_i_pressed;
		break;
	case 'x':
		camera.top();
		break;
	case 'c':
		camera.side1();
		break;
	case 'v':
		camera.side2();
		break;
	case 'z':
		camera.front();
		break;
	case 'w':
		if (playerZ < 0.1) {
			PlaySound(TEXT("solid.wav"), NULL, SND_ASYNC | SND_FILENAME);
			break;
		}
		if (done)
			break;
		playerZ -= 0.01;
		break;
	case 's':
		if (playerZ > 1) {
			PlaySound(TEXT("solid.wav"), NULL, SND_ASYNC | SND_FILENAME);
			break;
		}
		if (done)
			break;
		playerZ += 0.01;
		break;
	case 'a':
		if (playerX < 0.1) {
			PlaySound(TEXT("solid.wav"), NULL, SND_ASYNC | SND_FILENAME);
			break;
		}
		
		if (done)
			break;
		playerX -= 0.01;
		break;
	case 'd':
		if (playerX > 0.9) {
			PlaySound(TEXT("solid.wav"), NULL, SND_ASYNC | SND_FILENAME);
			break;
		}
		if (done)
			break;
		playerX += 0.01;
		break;

	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}
	/*cout << "playerX  "<< playerX << endl;
	cout << "playerZ  " << playerZ << endl;
	cout << "  x  " << goalX<< endl;
	cout << "z  " << goalZ << endl;*/
	if (playerX -0.1 <= goalX && playerX + 0.1 >= goalX && playerZ + 0.1 >= goalZ && playerZ - 0.1 <= goalZ) {
		PlaySound(TEXT("Ta Da.wav"), NULL, SND_ASYNC | SND_FILENAME);
		done = true;
	}

	glutPostRedisplay();
}
void Special(int key, int x, int y) {
	float a = 1.0;

	switch (key) {
	case GLUT_KEY_UP:
		camera.rotateX(a);
		break;
	case GLUT_KEY_DOWN:
		camera.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		camera.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		camera.rotateY(-a);
		break;
	}

	glutPostRedisplay();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Lab 5");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutTimerFunc(0, Timer, 0);
	glutIdleFunc(Anim);
	srand(time(NULL));

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}
