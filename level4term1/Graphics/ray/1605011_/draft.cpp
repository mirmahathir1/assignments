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
int levelOfRecursion, numberOfObjects, numberOfLightSources;
double imageSize;
double windowHeight, windowWidth;

vector<Object *> objects;

int main(){
    Vector* a = new Vector(-1,0,-1);
    Vector* n = new Vector(0,0,1);
    Vector *reflectionDirection = a->reflect(n);
    reflectionDirection->print();

}