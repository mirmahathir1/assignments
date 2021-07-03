#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <GL/glut.h>
#include "bitmap_image.hpp"
using namespace std;

#define pi (2 * acos(0.0))
#define INF 99999;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

int levelOfRecursion, numberOfObjects, numberOfLightSources;
double imageSize;
double windowHeight, windowWidth;

#include "1605011_LinearAlgebra.hpp"
#include "1605011_Shapes.hpp"
#include "1605011_Object.hpp"
#include "1605011_Ray.hpp"

//MY OWN CONTROL VARIABLES
Point position(0, 100, 0);
Vector upVector(0, 0, 1);
Vector lookVector(0, -1, 0);
Vector rightVector(-1, 0, 0);
int movementSpeed = 3;
#define viewAngle 80

// inputs from files





void drawSS()
{
	for (int i = 0; i < objects.size(); i++)
	{
		objects.at(i)->draw();
	}

	for (int i = 0; i < lights.size(); i++)
	{
		lights.at(i)->draw();
	}
}

void capture()
{
	cout << "Capture method called" << endl;
	bitmap_image image(imageSize, imageSize);
	for (int i = 0; i < imageSize; i++)
	{
		for (int j = 0; j < imageSize; j++)
		{
			image.set_pixel(i, j, 0, 0, 0);
		}
	}

	double plane_distance = (windowHeight / 2.0) / tan(degreeToRadian(viewAngle / 2.0));

	rightVector.set(Vector::crossProduct(lookVector, upVector));

	Vector *multiplied_lookVector = lookVector.multiply(plane_distance);

	Vector *multiplied_rightVector = rightVector.multiply(windowWidth / 2);

	Vector *multiplied_upVector = upVector.multiply(windowHeight / 2);

	Vector *top_left = position.sum(multiplied_lookVector)->subtract(multiplied_rightVector)->sum(multiplied_upVector);

	double du = windowWidth / imageSize;
	double dv = windowHeight / imageSize;

	top_left = top_left->sum(rightVector.multiply(0.5 * du))->subtract(upVector.multiply(0.5 * dv));

	int nearest;

	for (int i = 0; i < imageSize; i++)
	{
		for (int j = 0; j < imageSize; j++)
		{
			Vector *curPixel = top_left->sum(rightVector.multiply(i * du)->subtract(upVector.multiply(j * dv)));
			Ray *ray = new Ray(new Vector(position.x, position.y, position.z), curPixel->subtract(new Vector(position.x, position.y, position.z)));
			double *color = new double[3];
			color[0] = 0;
			color[1] = 0;
			color[2] = 0;

			double t, tMin = INF;
			for(int k = 0 ; k < objects.size(); k++){
				Object* o = objects.at(k);
				t = o->intersect(ray,color,1);
				if(t!= 99999 && t>0 && t < tMin){
					tMin = t;
					image.set_pixel(i, j, color[0]*255, color[1]*255, color[2]*255);
				}
			}
		}
	}

	image.save_image("out.bmp");
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
	gluPerspective(viewAngle, 1, 1, 1000.0);
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
	//cout << "KEYBOARD LISTENER: key=" << key << ",x=" << x << ",y=" << y << endl;
	switch (key)
	{
	case '0':
		capture();
		break;
	case 'g':
		drawgrid = 1 - drawgrid;
		break;
	case 'p':
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
		lookVector.rotate(Vector::crossProduct(lookVector, upVector), 5);
		break;
	case '4':
		//rotate look down
		lookVector.rotate(Vector::crossProduct(lookVector, upVector), -5);
		break;
	case '5':
		//tilt clockwise
		upVector.rotate(lookVector, 5);
		break;
	case '6':
		//tilt counter clockwise
		upVector.rotate(lookVector, -5);
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
		position.x += movementSpeed * rightVector.x;
		position.y += movementSpeed * rightVector.y;
		position.z += movementSpeed * rightVector.z;
		break;
	case GLUT_KEY_LEFT:
		rightVector.set(Vector::crossProduct(lookVector, upVector));
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
	// cout << "MOUSE LISTENER: button=" << button << ",state=" << state << ",x=" << x << ",y=" << y << endl;
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{ // 2 times?? in ONE click? -- solution is checking DOWN or UP
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
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void loadData()
{
	Floor *floor = new Floor();
	floor->setCoEfficients(0.2, 0.3, 0.3, 0.2);
	floor->setShine(5);
	objects.push_back((Object *)floor);

	ifstream inputFile;
	inputFile.open("scene.txt");

	inputFile >> levelOfRecursion;
	inputFile >> imageSize;
	windowHeight = 30;
	windowWidth = 30;
	inputFile >> numberOfObjects;

	//for all objects
	double color_red, color_green, color_blue, ambient, diffuse, specular, recurseCoeff, shine;
	//for Sphere
	double sphere_center_x, sphere_center_y, sphere_center_z, radius;
	//for triangle
	double point1_x, point1_y, point1_z, point2_x, point2_y, point2_z, point3_x, point3_y, point3_z;
	//for general quadratic
	double general_A, general_B, general_C, general_D, general_E, general_F, general_G, general_H, general_I, general_J, gquad_x, gquad_y, gquad_z, gquad_l, gquad_w, gquad_h;

	for (int i = 0; i < numberOfObjects; i++)
	{
		string objectType;
		Object *temp;
		inputFile >> objectType;
		if (objectType == "sphere")
		{
			inputFile >> sphere_center_x >> sphere_center_y >> sphere_center_z >> radius;
			temp = (Object *)new Sphere(new Vector(sphere_center_x, sphere_center_y, sphere_center_z), radius);
		}
		else if (objectType == "triangle")
		{
			inputFile >> point1_x >> point1_y >> point1_z >> point2_x >> point2_y >> point2_z >> point3_x >> point3_y >> point3_z;
			temp = (Object *)new Triangle(
				new Point(point1_x, point1_y, point1_z),
				new Point(point2_x, point2_y, point2_z),
				new Point(point3_x, point3_y, point3_z));
		}
		else if (objectType == "general")
		{
			inputFile >> general_A >> general_B >> general_C >> general_D >> general_E >> general_F >> general_G >> general_H >> general_I >> general_J >> gquad_x >> gquad_y >> gquad_z >> gquad_l >> gquad_w >> gquad_h;
			temp = (Object *)new GQuadratic(
				general_A,
				general_B,
				general_C,
				general_D,
				general_E,
				general_F,
				general_G,
				general_H,
				general_I,
				general_J,
				new Vector(gquad_x, gquad_y, gquad_z),
				gquad_l,
				gquad_w,
				gquad_h);
		}

		inputFile >> color_red >> color_green >> color_blue;
		// cout<<"red: "<<color_red<<" green: "<<color_green<<" blue: "<<color_blue;
		inputFile >> ambient >> diffuse >> specular >> recurseCoeff >> shine;
		temp->setColor(color_red, color_green, color_blue);
		temp->setCoEfficients(ambient, diffuse, specular, recurseCoeff);
		temp->setShine(shine);
		objects.push_back(temp);
	}

	inputFile >> numberOfLightSources;

	double light_x, light_y, light_z, light_red, light_green, light_blue;
	for (int i = 0; i < numberOfLightSources; i++)
	{
		inputFile >> light_x >> light_y >> light_z >> light_red >> light_green >> light_blue;
		lights.push_back(new Light(new Vector(light_x, light_y, light_z), light_red, light_green, light_blue));
	}
}
void printData()
{
	cout << "Level of recursion: " << levelOfRecursion << endl;
	cout << "Image size: " << imageSize << endl;
	cout << "Number of objects: " << numberOfObjects << endl;
	cout << "Printing Objects---" << endl;
	for (int i = 0; i < objects.size(); i++)
	{
		objects.at(i)->print();
		objects.at(i)->printCommons();
	}
	cout << "Printing Lights---" << endl;
	for (int i = 0; i < lights.size(); i++)
	{
		lights.at(i)->print();
	}
	cout << "Printing Done" << endl;
}

int main(int argc, char **argv)
{
	loadData();
	printData();

	glutInit(&argc, argv);
	glutInitWindowSize(imageSize, imageSize);
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
