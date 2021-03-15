#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
using namespace std;

#include <GL/glut.h>

#define pi (2 * acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

struct point
{
	double x, y, z;
};

double degreeToRadian(double degree)
{
	return (degree * (pi / 180.0));
}

class Vector
{
public:
	GLdouble x = 0, y = 0, z = 0;
	Vector(GLdouble x, GLdouble y, GLdouble z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	void set(GLdouble x, GLdouble y, GLdouble z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void set(Vector a)
	{
		this->x = a.x;
		this->y = a.y;
		this->z = a.z;
	}

	void print()
	{
		cout << "vector: x = " << x << ", y = " << y << ", z = " << z << endl;
	}

	void rotate(Vector rotationAxis, GLdouble degree)
	{
		GLdouble x;
		GLdouble y;
		GLdouble z;

		Vector crossOfUandAxis = crossProduct(rotationAxis, *(this));

		GLdouble cosineOfDegree = cos((double)degreeToRadian(degree));
		GLdouble sineOfDegree = sin((double)degreeToRadian(degree));
		this->x = this->x * cosineOfDegree + crossOfUandAxis.x * sineOfDegree;
		this->y = this->y * cosineOfDegree + crossOfUandAxis.y * sineOfDegree;
		this->z = this->z * cosineOfDegree + crossOfUandAxis.z * sineOfDegree;
	}

	static Vector crossProduct(Vector a, Vector b)
	{
		return Vector((a.y * b.z) - (b.y * a.z), (b.x * a.z) - (a.x * b.z), (a.x * b.y) - (a.y * b.x));
	}
};

class Point
{
public:
	GLdouble x = 0, y = 0, z = 0;
	Point(GLdouble x, GLdouble y, GLdouble z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	void set(GLdouble x, GLdouble y, GLdouble z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	void print()
	{
		cout << "point: x = " << x << ", y = " << y << ", z = " << z << endl;
	}
};

//MY OWN CONTROL VARIABLES
Point position(0, 100, 0);
Vector upVector(0, 0, 1);
Vector lookVector(0, -1, 0);
Vector rightVector(-1, 0, 0);
int movementSpeed = 3;



#include "shapes.h"

void drawSS()
{
	/*
    glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();// atokkhon ja draw kora holo ta main matrix e multiply kore deya lagbe akhon. eita stack e push kora
    { // upogroho gular jonno
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);*/

	glColor3f(1, 1, 1);
	drawSphere(50, 100, 20);
}

void init()
{
	//codes for initialization
	drawgrid = 1;
	drawaxes = 1;

	position.set(10, -100, 50);
	upVector.set(0, 0, 1);
	lookVector.set(0, 1, 0);

	//clear the screen
	glClearColor(0, 0, 0, 0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80, 1, 1, 1000.0);
	//field of view in the Y (vertically) in degrees
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

void display()
{

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0); //color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	gluLookAt(position.x, position.y, position.z, position.x + lookVector.x, position.y + lookVector.y, position.z + lookVector.z, upVector.x, upVector.y, upVector.z);

	// first three arguments denote the (x,y,z) position of the camera
	// second three arguments denote the (x,y,z) coordinates of the point the camera is looking at
	// third three arguments denote the (x,y,z) coordinates of the up direction of the camera


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);

	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

	//glColor3f(1,0,0);
	//drawSquare(10);

	drawSS();

	//drawCircle(30,24);

	//drawCone(20,50,24);

	//drawSphere(30,24,20);

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void keyboardListener(unsigned char key, int x, int y)
{
	cout << "KEYBOARD LISTENER: key=" << key << ",x=" << x << ",y=" << y << endl;
	switch (key)
	{
	case 'g':
		drawgrid = 1 - drawgrid;
		break;
	case 'a':
		drawaxes = 1 - drawaxes;
		break;
	case '1':
		//rotate look left
		lookVector.rotate(upVector, 5);
		break;
	case '2':
		//rotate look right
		lookVector.rotate(upVector, -5);
		break;
	case '3':
		//rotate look up
		lookVector.rotate(Vector::crossProduct(lookVector, upVector),5);

		break;
	case '4':
		//rotate look down
		lookVector.rotate(Vector::crossProduct(lookVector, upVector),-5);
		break;
	case '5':
		//tilt clockwise
		upVector.rotate(lookVector,5);

		break;
	case '6':
		//tilt counter clockwise
		upVector.rotate(lookVector,-5);
		break;
	case 'r':
		init();
		break;
	default:
		break;
	}
}

void specialKeyListener(int key, int x, int y)
{
	//cout<<"SPECIAL KEY LISTENER: key="<<key<<",x="<<x<<",y="<<y<<endl;
	switch (key)
	{
	case GLUT_KEY_DOWN: //down arrow key
		position.x -= movementSpeed * lookVector.x;
		position.y -= movementSpeed * lookVector.y;
		break;
	case GLUT_KEY_UP: // up arrow key
		position.x += movementSpeed * lookVector.x;
		position.y += movementSpeed * lookVector.y;
		break;
	case GLUT_KEY_RIGHT:
		rightVector.set(Vector::crossProduct(lookVector, upVector));

		//rightVector.print();

		position.x += movementSpeed * rightVector.x;
		position.y += movementSpeed * rightVector.y;
		position.z += movementSpeed * rightVector.z;
		break;
	case GLUT_KEY_LEFT:
		position.x -= movementSpeed * rightVector.x;
		position.y -= movementSpeed * rightVector.y;
		position.z -= movementSpeed * rightVector.z;

		break;

	case GLUT_KEY_PAGE_UP:

		position.x += movementSpeed * upVector.x;
		position.y += movementSpeed * upVector.y;
		position.z += movementSpeed * upVector.z;
		break;
	case GLUT_KEY_PAGE_DOWN:
		position.x -= movementSpeed * upVector.x;
		position.y -= movementSpeed * upVector.y;
		position.z -= movementSpeed * upVector.z;
		break;

	case GLUT_KEY_INSERT:
		break;

	case GLUT_KEY_HOME:
		break;
	case GLUT_KEY_END:
		break;

	default:
		break;
	}
}

void mouseListener(int button, int state, int x, int y)
{ //x, y is the x-y of the screen (2D)
	cout << "MOUSE LISTENER: button=" << button << ",state=" << state << ",x=" << x << ",y=" << y << endl;
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{ // 2 times?? in ONE click? -- solution is checking DOWN or UP
			drawaxes = 1 - drawaxes;
		}
		break;

	case GLUT_RIGHT_BUTTON:
		//........
		break;

	case GLUT_MIDDLE_BUTTON:
		//........
		break;

	default:
		break;
	}
}

void animate()
{

	angle += 0.005;

	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); //Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST); //enable Depth Testing

	glutDisplayFunc(display); //display callback function
	glutIdleFunc(animate);	  //what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop(); //The main loop of OpenGL

	return 0;
}
