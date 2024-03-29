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