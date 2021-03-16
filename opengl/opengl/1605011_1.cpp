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
		normalize();
	}

	static Vector crossProduct(Vector a, Vector b)
	{
		return Vector((a.y * b.z) - (b.y * a.z), (b.x * a.z) - (a.x * b.z), (a.x * b.y) - (a.y * b.x));
	}

	void normalize()
	{
		GLdouble denominator = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
		this->x /= denominator;
		this->y /= denominator;
		this->z /= denominator;
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
	Point()
	{
	}
	static Point crossProduct(Vector a, Point b)
	{
		return Point((a.y * b.z) - (b.y * a.z), (b.x * a.z) - (a.x * b.z), (a.x * b.y) - (a.y * b.x));
	}

	void rotate(Vector rotationAxis, GLdouble degree)
	{
		GLdouble x;
		GLdouble y;
		GLdouble z;

		Point crossOfUandAxis = Point::crossProduct(rotationAxis, *(this));

		GLdouble cosineOfDegree = cos((double)degreeToRadian(degree));
		GLdouble sineOfDegree = sin((double)degreeToRadian(degree));
		this->x = this->x * cosineOfDegree + crossOfUandAxis.x * sineOfDegree;
		this->y = this->y * cosineOfDegree + crossOfUandAxis.y * sineOfDegree;
		this->z = this->z * cosineOfDegree + crossOfUandAxis.z * sineOfDegree;
	}
};

//MY OWN CONTROL VARIABLES
Point position(0, 100, 0);
Vector upVector(0, 0, 1);
Vector lookVector(0, -1, 0);
Vector rightVector(-1, 0, 0);
int movementSpeed = 3;

Point cylinderPoints[100][100];
Vector cylinderVector(1, 0, 0);
int cylinderRadius = 10;
int cylinderHeight = 100;
int cylinderSlices = 100;
int cylinderStacks = 20;

Point gunshots[100];
int gunshotCount = 0;
int gunDegreeRange = 30;
int gunCurrentDegreeY = 0;
int gunCurrentDegreeZ = 0;

void drawAxes()
{
	if (drawaxes == 1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		{
			glVertex3f(100, 0, 0);
			glVertex3f(-100, 0, 0);

			glVertex3f(0, -100, 0);
			glVertex3f(0, 100, 0);

			glVertex3f(0, 0, 100);
			glVertex3f(0, 0, -100);
		}
		glEnd();
	}
}

void drawGrid()
{
	int i;
	if (drawgrid == 1)
	{
		glColor3f(0.6, 0.6, 0.6); //grey
		glBegin(GL_LINES);
		{
			for (i = -8; i <= 8; i++)
			{

				if (i == 0)
					continue; //SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i * 10, -90, 0);
				glVertex3f(i * 10, 90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i * 10, 0);
				glVertex3f(90, i * 10, 0);
			}
		}
		glEnd();
	}
}

void getCylinderPoints()
{
	
	int i, j;
	double h, r;
	//generate points
	for (i = 0; i <= cylinderStacks; i++)
	{
		// h=radius*sin(((double)i/(double)stacks)*(pi/2));
		h = i * (cylinderHeight / cylinderStacks);
		//r=radius*cos(((double)i/(double)stacks)*(pi/2));
		r = cylinderRadius;
		for (j = 0; j <= cylinderSlices; j++)
		{
			cylinderPoints[i][j].y = r * cos(((double)j / (double)cylinderSlices) * 2 * pi);
			cylinderPoints[i][j].z = r * sin(((double)j / (double)cylinderSlices) * 2 * pi);
			cylinderPoints[i][j].x = h;
		}
	}
}

void drawCylinder()
{
	int i, j;
	for (i = 0; i < cylinderStacks; i++)
	{
		// glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		// glColor3f(0,0,0);
		glColor3f(i % 2, i % 2, i % 2);
		for (j = 0; j < cylinderSlices; j++)
		{
			glBegin(GL_QUADS);
			{
				//upper hemisphere
				glVertex3f(cylinderPoints[i][j].x, cylinderPoints[i][j].y, cylinderPoints[i][j].z);
				glVertex3f(cylinderPoints[i][j + 1].x, cylinderPoints[i][j + 1].y, cylinderPoints[i][j + 1].z);
				glVertex3f(cylinderPoints[i + 1][j + 1].x, cylinderPoints[i + 1][j + 1].y, cylinderPoints[i + 1][j + 1].z);
				glVertex3f(cylinderPoints[i + 1][j].x, cylinderPoints[i + 1][j].y, cylinderPoints[i + 1][j].z);
			}
			glEnd();
		}
	}
}

void drawCylinder(double radius, double height, int slices, int stacks)
{
	//struct point points[100][100];

	Point points[100][100];

	int i, j;
	double h, r;
	//generate points
	for (i = 0; i <= stacks; i++)
	{
		// h=radius*sin(((double)i/(double)stacks)*(pi/2));
		h = i * (height / stacks);
		//r=radius*cos(((double)i/(double)stacks)*(pi/2));
		r = radius;
		for (j = 0; j <= slices; j++)
		{
			points[i][j].y = r * cos(((double)j / (double)slices) * 2 * pi);
			points[i][j].z = r * sin(((double)j / (double)slices) * 2 * pi);
			points[i][j].x = h;

			// points[i][j].rotate(Vector(0,1,0),45);
		}
	}
	//draw quads using generated points
	for (i = 0; i < stacks; i++)
	{
		// glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		// glColor3f(0,0,0);
		glColor3f(i % 2, i % 2, i % 2);
		for (j = 0; j < slices; j++)
		{
			glBegin(GL_QUADS);
			{
				//upper hemisphere
				glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
				glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
				glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
				glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
				//lower hemisphere
				// glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
				// glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
				// glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
				// glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
			}
			glEnd();
		}
	}
}

void drawSquare(Point upperRight, Point lowerRight, Point upperLeft, Point lowerLeft)
{
	glColor3f(0.5,0.5,0.5);
	glBegin(GL_QUADS);
	{
		glVertex3f(upperRight.x, upperRight.y, upperRight.z);
		glVertex3f(lowerRight.x, lowerRight.y, lowerRight.z);
		glVertex3f(lowerLeft.x, lowerLeft.y, lowerLeft.z);
		glVertex3f(upperLeft.x, upperLeft.y, upperLeft.z);
	}
	glEnd();
}

void drawGunShot(int y, int z){
	glColor3f(1,0.5,0.5);
	glBegin(GL_QUADS);
	{
		glVertex3f(200, y+3, z+3);
		glVertex3f(200, y+3, z-3);
		glVertex3f(200, y-3, z-3);
		glVertex3f(200, y-3, z+3);
	}
	glEnd();
}

void drawGunShotAll(){
	for(int i = 0 ; i < gunshotCount; i++){
		drawGunShot(gunshots[i].y,gunshots[i].z);
	}
}

void drawSquare(double a)
{
	//glColor3f(1.0,0.0,0.0);

	//gl begin er moddhe bole dite hobe ami ki draw korte chai
	//line draw korte gl_lines bolte hobe
	// tarpor 4 vertext bolte hobe ja diye quadrangle draw korbe.
	// shudhu boundary na, vitorer shob draw korbe
	glBegin(GL_QUADS);
	{
		glVertex3f(a, a, 2);
		glVertex3f(a, -a, 2);
		glVertex3f(-a, -a, 2);
		glVertex3f(-a, a, 2);
	}
	glEnd();
}

void drawCircle(double radius, int segments)
{
	int i;
	struct point points[100];
	glColor3f(0.7, 0.7, 0.7);
	//generate points
	for (i = 0; i <= segments; i++)
	{
		points[i].x = radius * cos(((double)i / (double)segments) * 2 * pi); //sin and cos functions take arguments as radians
		points[i].y = radius * sin(((double)i / (double)segments) * 2 * pi);
	}
	//draw segments using generated points
	for (i = 0; i < segments; i++)
	{
		glBegin(GL_LINES);
		{
			glVertex3f(points[i].x, points[i].y, 0);
			glVertex3f(points[i + 1].x, points[i + 1].y, 0);
		}
		glEnd();
	}
}

void drawCone(double radius, double height, int segments)
{
	int i;
	double shade;
	struct point points[100];
	//generate points
	for (i = 0; i <= segments; i++)
	{
		points[i].x = radius * cos(((double)i / (double)segments) * 2 * pi);
		points[i].y = radius * sin(((double)i / (double)segments) * 2 * pi);
	}
	//draw triangles using generated points
	for (i = 0; i < segments; i++)
	{
		//create shading effect
		if (i < segments / 2)
			shade = 2 * (double)i / (double)segments;
		else
			shade = 2 * (1.0 - (double)i / (double)segments);
		glColor3f(shade, shade, shade);

		glBegin(GL_TRIANGLES);
		{
			glVertex3f(0, 0, height);
			glVertex3f(points[i].x, points[i].y, 0);
			glVertex3f(points[i + 1].x, points[i + 1].y, 0);
		}
		glEnd();
	}
}

void drawSphere(double radius, int slices, int stacks)
{
	struct point points[100][100];
	int i, j;
	double h, r;
	//generate points
	for (i = 0; i <= stacks; i++)
	{
		h = radius * sin(((double)i / (double)stacks) * (pi / 2));
		r = radius * cos(((double)i / (double)stacks) * (pi / 2));
		for (j = 0; j <= slices; j++)
		{
			points[i][j].x = r * cos(((double)j / (double)slices) * 2 * pi);
			points[i][j].y = r * sin(((double)j / (double)slices) * 2 * pi);
			points[i][j].z = h;
		}
	}
	//draw quads using generated points
	for (i = 0; i < stacks; i++)
	{
		// glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		glColor3f(i % 2, i % 2, i % 2);
		for (j = 0; j < slices; j++)
		{
			glBegin(GL_QUADS);
			{
				//upper hemisphere
				glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
				glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
				glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
				glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
				//lower hemisphere
				glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
				glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
				glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
				glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
			}
			glEnd();
		}
	}
}

void rotateCylinderPoints(Vector rotationAxis, int degree, int slices, int stacks)
{
	for (int i = 0; i <= stacks; i++)
	{

		for (int j = 0; j <= slices; j++)
		{
			cylinderPoints[i][j].rotate(rotationAxis, degree);
		}
	}
}

void addGunShotPoint()
{
	GLdouble scale = 200 / cylinderVector.x;
	GLdouble x = 200;
	GLdouble y = cylinderVector.y * scale;
	GLdouble z = cylinderVector.z * scale;

	if (y < 100 && y > -100 && z < 100 && z > -100)
	{
		gunshots[gunshotCount] = Point(x, y, z);
		gunshotCount++;
	}
}

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
	drawSphere(20, 100, 20);
	drawCylinder();

	drawSquare(Point(201, 100, 100), Point(201, 100, -100), Point(201, -100, 100), Point(201, -100, -100));

	drawGunShotAll();
}

void init()
{
	getCylinderPoints();

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
	case 'q':
		if (gunCurrentDegreeY + 2 > gunDegreeRange)
			break;

		gunCurrentDegreeY += 2;
		rotateCylinderPoints(Vector(0, 0, 1), 2, 100, 20);
		cylinderVector.rotate(Vector(0, 0, 1), 2);
		break;
	case 'w':
		if (gunCurrentDegreeY - 2 < -gunDegreeRange)
			break;

		gunCurrentDegreeY -= 2;
		rotateCylinderPoints(Vector(0, 0, 1), -2, 100, 20);
		cylinderVector.rotate(Vector(0, 0, 1), -2);
		break;
	case 'e':
		if (gunCurrentDegreeZ + 2 > gunDegreeRange)
			break;

		gunCurrentDegreeZ += 2;
		rotateCylinderPoints(Vector(0, 1, 0), -2, 100, 20);
		cylinderVector.rotate(Vector(0, 1, 0), -2);
		break;
	case 'r':
		if (gunCurrentDegreeZ - 2 < -gunDegreeRange)
			break;

		gunCurrentDegreeZ -= 2;
		rotateCylinderPoints(Vector(0, 1, 0), 2, 100, 20);
		cylinderVector.rotate(Vector(0, 1, 0), 2);
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
	cout << "MOUSE LISTENER: button=" << button << ",state=" << state << ",x=" << x << ",y=" << y << endl;
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{ // 2 times?? in ONE click? -- solution is checking DOWN or UP
			addGunShotPoint();
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
