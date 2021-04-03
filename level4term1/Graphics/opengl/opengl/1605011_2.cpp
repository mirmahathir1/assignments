#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
using namespace std;

#include <GL/glut.h>

#define pi (2 * acos(0.0))

int drawgrid;
int drawaxes;
double angle;
bool paused = false;
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
	Vector()
	{
	}
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
	static GLdouble dotProduct(Vector a, Vector b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	void normalize()
	{
		GLdouble denominator = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
		this->x /= denominator;
		this->y /= denominator;
		this->z /= denominator;
	}

	static Vector multiply(Vector vector, GLdouble scale)
	{
		return Vector(vector.x * scale, vector.y * scale, vector.z * scale);
	}

	static Vector subtract(Vector a, Vector b)
	{
		return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	static Vector reflect(Vector a, Vector n)
	{
		n.normalize();
		//⃗ a-−2×(a⃗ )⋅n⃗ ×n
		return subtract(a, multiply(multiply(n, 2), dotProduct(a, n)));
	}

	static Vector negate(Vector a)
	{
		return Vector(-a.x, -a.y, -a.z);
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

class Color
{
public:
	GLdouble red, green, blue;
	Color()
	{
	}
	Color(GLdouble red, GLdouble green, GLdouble blue)
	{
		this->red = red;
		this->green = green;
		this->blue = blue;
	}
};

void drawCircle(double radius, int segments, Color color);

class Circle
{
public:
	GLdouble x;
	GLdouble y;
	GLdouble r;
	Color color;

	Vector direction;
	GLdouble speed = 1;

	static constexpr GLdouble maxSpeed = 100;
	static constexpr GLdouble speedModifier = 0.1;

	bool insideSmallCircle = false;

	Circle(GLdouble x, GLdouble y, GLdouble r, Color color)
	{
		this->x = x;
		this->y = y;
		this->color = color;
		this->r = r;
	}

	void setDirection(Vector direction)
	{
		this->direction = direction;
		this->direction.normalize();
	}
	void increaseSpeed()
	{
		if (this->speed >= maxSpeed)
			return;
		this->speed++;
	}
	void decreaseSpeed()
	{
		if (this->speed <= 1)
			return;
		this->speed--;
	}

	void addDisplacement()
	{
		this->x += speedModifier * direction.x * speed;
		this->y += speedModifier * direction.y * speed;
	}

	void move()
	{
		addDisplacement();

		GLdouble distanceFromCenter = sqrt(pow(this->x - 50, 2) + pow(this->y - 50, 2));

		if (!insideSmallCircle && distanceFromCenter < 35 - this->r)
		{
			this->color = Color(0, 1, 1);
			insideSmallCircle = !insideSmallCircle;
		}

		if (insideSmallCircle)
		{
			if (distanceFromCenter > 35 - this->r)
			{
				centerCircleReflection();
				addDisplacement();
			}

			//largeBoxReflection();
		}
		else
		{
			largeBoxReflection();
		}
	}
	void centerCircleReflection()
	{
		Vector radius = Vector::subtract(Vector(50, 50, 0), Vector(this->x, this->y, 0));
		this->direction = Vector::reflect(this->direction, radius);
	}

	void largeBoxReflection()
	{
		if (this->x >= 100 - r)
		{
			direction = Vector::reflect(direction, Vector(100 - r, 0, 0));
		}
		else if (this->x < 0 + r)
		{
			direction = Vector::reflect(direction, Vector(0 + r, 0, 0));
		}
		if (this->y >= 100 - r)
		{
			direction = Vector::reflect(direction, Vector(0, 100 - r, 0));
		}
		else if (this->y <= 0 + r)
		{
			direction = Vector::reflect(direction, Vector(0, 0 + r, 0));
		}
	}

	void draw()
	{
		glPushMatrix();
		{
			glTranslatef(x, y, 0);
			drawCircle(r, 35, color);
		}
		glPopMatrix();
	}

	static void circleVsCircleReflection();

	GLdouble distance(Circle c)
	{
		return sqrt(pow(this->x - c.x, 2) + pow(this->y - c.y, 2));
	}
};

Color circleColor(1, 1, 0);
GLdouble circleRadius = 3;
Circle circles[5] = {
	Circle(circleRadius, circleRadius, circleRadius, circleColor),
	Circle(circleRadius, circleRadius, circleRadius, circleColor),
	Circle(circleRadius, circleRadius, circleRadius, circleColor),
	Circle(circleRadius, circleRadius, circleRadius, circleColor),
	Circle(circleRadius, circleRadius, circleRadius, circleColor)};

void Circle::circleVsCircleReflection()
{
	for (int i = 0; i < 4; i++)
	{
		if (circles[i].insideSmallCircle)
		{
			for (int j = i + 1; j < 5; j++)
			{
				if (circles[j].insideSmallCircle)
				{

					//if (Vector::dotProduct(circles[i].direction, circles[j].direction) > 0)
					//{
					GLdouble centerDistance = circles[i].distance(circles[j]);
					GLdouble radiusSum = (circles[i].r + circles[j].r);
					if (centerDistance > radiusSum && centerDistance - radiusSum < 1)
					{
						Vector radiusVector(circles[i].x - circles[j].x, circles[i].y - circles[j].y, 0);
						circles[i].direction = Vector::reflect(circles[i].direction, radiusVector);
						circles[j].direction = Vector::reflect(circles[j].direction, Vector::negate(radiusVector));

						// circles[i].addDisplacement();
						// circles[j].addDisplacement();
					}
					//}
				}
			}
		}
	}
}

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

void drawSquare(Point upperRight, Point lowerRight, Point upperLeft, Point lowerLeft)
{
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_QUADS);
	{
		glVertex3f(upperRight.x, upperRight.y, upperRight.z);
		glVertex3f(lowerRight.x, lowerRight.y, lowerRight.z);
		glVertex3f(lowerLeft.x, lowerLeft.y, lowerLeft.z);
		glVertex3f(upperLeft.x, upperLeft.y, upperLeft.z);
	}
	glEnd();
}

void drawSquareOutline(double a, Color color)
{
	glColor3f(color.red, color.green, color.blue);
	glBegin(GL_LINES);
	{
		glVertex3f(+a, +a, 0);
		glVertex3f(+a, -a, 0);

		glVertex3f(+a, -a, 0);
		glVertex3f(-a, -a, 0);

		glVertex3f(-a, -a, 0);
		glVertex3f(-a, +a, 0);

		glVertex3f(-a, +a, 0);
		glVertex3f(+a, +a, 0);
	}
	glEnd();
}

void drawSquare(double a)
{
	glBegin(GL_QUADS);
	{
		glVertex3f(a, a, 2);
		glVertex3f(a, -a, 2);
		glVertex3f(-a, -a, 2);
		glVertex3f(-a, a, 2);
	}
	glEnd();
}

void drawCircle(double radius, int segments, Color color)
{
	int i;
	struct point points[100];
	glColor3f(color.red, color.green, color.blue);
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

void drawSS()
{
	//move the whole field and draw
	glPushMatrix();
	{
		glTranslatef(50, 50, 0);
		drawSquareOutline(50, Color(0, 1, 0));
		drawCircle(35, 35, Color(1, 0, 0));
	}
	glPopMatrix();

	for (int i = 0; i < 5; i++)
	{
		circles[i].draw();
	}
}

int spawnedCircleCount = 0;

void spawnCircle()
{
	if (spawnedCircleCount == 5)
	{
		return;
	}

	for (int i = 0; i < spawnedCircleCount; i++)
	{
		if (circles[i].x < 20 || circles[i].y < 20)
		{
			return;
		}
	}

	circles[spawnedCircleCount].setDirection(Vector(rand(), rand(), 0));
	spawnedCircleCount++;
}

void init()
{

	//codes for initialization
	drawgrid = 0;
	drawaxes = 0;
	/*
	circles[0].setDirection(Vector(rand(), rand(), 0));
	circles[1].setDirection(Vector(rand(), rand(), 0));
	circles[2].setDirection(Vector(rand(), rand(), 0));
	circles[3].setDirection(Vector(rand(), rand(), 0));
	circles[4].setDirection(Vector(rand(), rand(), 0));*/

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

void animate()
{
	if (!paused)
	{
		spawnCircle();

		Circle::circleVsCircleReflection();

		for (int i = 0; i < 5; i++)
		{
			circles[i].move();
		}
	}

	//codes for any changes in Models, Camera
	glutPostRedisplay();
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

	gluLookAt(50, 50, 100, 50, 50, -1, 0, 1, 0);

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
	case 'a':
		drawaxes = 1 - drawaxes;
	case 'g':
		drawgrid = 1 - drawgrid;
		break;
	case 'p':
		paused = !paused;
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
		for (int i = 0; i < 5; i++)
		{
			circles[i].decreaseSpeed();
		}
		break;
	case GLUT_KEY_UP: // up arrow key
		for (int i = 0; i < 5; i++)
		{
			circles[i].increaseSpeed();
		}
		break;
	case GLUT_KEY_RIGHT:
		break;
	case GLUT_KEY_LEFT:
		break;

	case GLUT_KEY_PAGE_UP:
		break;
	case GLUT_KEY_PAGE_DOWN:
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

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
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
