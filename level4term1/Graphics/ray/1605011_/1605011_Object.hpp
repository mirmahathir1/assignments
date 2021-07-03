#define AMB 0
#define DIFF 1
#define SPEC 2
#define RECUR 3
class Ray
{
public:
	Vector *start, *dir;
	Ray(Vector *start, Vector *dir)
	{
		this->start = start;
		this->dir = dir;
	}
	Vector *getRayFromT(double t)
	{
		return this->start->sum(this->dir->multiply(t));
	}
};

class Light
{
public:
	Vector *position;
	double color[3];
	Light(Vector *position, double red, double green, double blue)
	{
		this->position = position;
		this->color[0] = red;
		this->color[1] = green;
		this->color[2] = blue;
	}

	void draw()
	{
		glColor3f(color[0], color[1], color[2]); //grey
		glPushMatrix();
		glTranslatef(position->x, position->y, position->z);
		drawSphere(1, 10, 10);
		glPopMatrix();
	}
	void print()
	{
		cout << "Light= ";
		position->print();
		cout << ", color= (" << color[0] << "," << color[1] << "," << color[2] << ")";
		cout << endl;
	}
};

vector<Light *> lights;

class Object
{
public:
	Vector *reference_point;
	double height, width, length;
	double color[3];
	double coEfficients[4];
	int shine;
	virtual void draw() {}
	void setColor(double red, double green, double blue)
	{
		color[0] = red;
		color[1] = green;
		color[2] = blue;
	}
	void setShine(int shine)
	{
		this->shine = shine;
	}
	void setCoEfficients(double ambient, double diffuse, double specular, double recurseCoeff)
	{
		this->coEfficients[0] = ambient;
		this->coEfficients[1] = diffuse;
		this->coEfficients[2] = specular;
		this->coEfficients[3] = recurseCoeff;
	}
	virtual void print() {}
	void printCommons()
	{
		cout << "Color= (" << color[0] << "," << color[1] << "," << color[2] << "), ";
		cout << "Ambient= " << coEfficients[0] << ", ";
		cout << "Diffuse= " << coEfficients[1] << ", ";
		cout << "Specular= " << coEfficients[2] << ", ";
		cout << "Recursive reflection= " << coEfficients[3] << ", ";
		cout << "Shine= " << shine << ", ";
		cout << endl;
	}
	virtual double intersect(Ray *ray, double *color, int level) { return -1; }
};

vector<Object *> objects;

class Sphere : Object
{
public:
	struct point points[100][100];
	int stacks = 50, slices = 50;
	Sphere(Vector *center, double radius)
	{
		this->reference_point = center;
		this->length = radius;
	}
	void draw()
	{
		// drawSphere(100, 100, 100);
		glColor3f(color[0], color[1], color[2]); //grey
		glPushMatrix();
		glTranslatef(reference_point->x, reference_point->y, reference_point->z);
		drawSphere(this->length, stacks, slices);
		glPopMatrix();
	}
	void print()
	{
		cout << "Sphere: ";
		cout << "Center= ";
		reference_point->print();
		cout << ", radius= " << this->length;
		cout << endl;
	}
	virtual double intersect(Ray *ray, double *color, int level)
	{
		Vector *translatedOrigin = ray->start->subtract(this->reference_point);

		double a = ray->dir->dot(ray->dir);
		double b = 2 * ray->dir->dot(translatedOrigin);
		double c = translatedOrigin->dot(translatedOrigin) - this->length * this->length;
		double d_square = b * b - 4 * a * c;
		if (d_square < 0)
			return -1;
		double d = sqrt(d_square);

		double t = min((-b + d) / (2 * a), (-b - d) / (2 * a));

		double intersectionPoint_red = this->color[0];
		double intersectionPoint_green = this->color[1];
		double intersectionPoint_blue = this->color[2];

		color[0] = intersectionPoint_red * coEfficients[AMB];
		color[1] = intersectionPoint_green * coEfficients[AMB];
		color[2] = intersectionPoint_blue * coEfficients[AMB];

		if (level == 0)
		{
			return t;
		}

		Vector* intersectionPoint = translatedOrigin->sum(ray->dir->multiply(t));
		Vector* normal = intersectionPoint;

		for (int i = 0; i < lights.size(); i++)
		{
			Light *light = lights.at(i);
			Ray *lightRay = new Ray(light->position, intersectionPoint->subtract(light->position));

			Vector *V = ray->dir->negate();
			Vector *R = lightRay->dir->reflect(normal);

			double phong = R->dot(V) / (R->abs() * V->abs());

			if (phong > 0)
			{
				color[0] = min(1.0, color[0] + light->color[0] * coEfficients[SPEC] * pow(phong, shine) * intersectionPoint_red);
				color[1] = min(1.0, color[1] + light->color[1] * coEfficients[SPEC] * (pow(phong, shine)) * intersectionPoint_green);
				color[2] = min(1.0, color[2] + light->color[2] * coEfficients[SPEC] * (pow(phong, shine)) * intersectionPoint_blue);
			}

			Vector *L = lightRay->dir->negate();
			Vector *N = normal;

			double lambert = L->dot(N) / (L->abs() * N->abs());

			if (lambert > 0)
			{
				color[0] = min(1.0, color[0] + light->color[0] * coEfficients[DIFF] * lambert * intersectionPoint_red);
				color[1] = min(1.0, color[1] + light->color[1] * coEfficients[DIFF] * lambert * intersectionPoint_green);
				color[2] = min(1.0, color[2] + light->color[2] * coEfficients[DIFF] * lambert * intersectionPoint_blue);
			}

			if (level >= levelOfRecursion)
				return t;

			Ray* recursiveRay = new Ray(intersectionPoint->add(reference_point),ray->dir->reflect(normal));

			double *color_recurse = new double[3];
			double tRecurse, tMinRecurse = INF;
			for(int k = 0 ; k < objects.size(); k++){
				Object* o = objects.at(k);
				if(o== this){
					continue;
				}
				tRecurse = o->intersect(recursiveRay,color_recurse,level+1);
				if(tRecurse!= 99999 && tRecurse>0 && tRecurse < tMinRecurse){
					tMinRecurse = tRecurse;
					color[0] = min(1.0,color[0]+coEfficients[RECUR]*color_recurse[0]);
					color[1] = min(1.0,color[1]+coEfficients[RECUR]*color_recurse[1]);
					color[2] = min(1.0,color[2]+coEfficients[RECUR]*color_recurse[2]);
				}
			}

			
		}

		

		return t;
	}
};

class Triangle : Object
{
public:
	Point *a, *b, *c;
	Vector *vector_a, *vector_b, *vector_c;
	Vector *normal;
	double d_of_plane;
	double area;
	Triangle(Point *a, Point *b, Point *c)
	{
		this->a = a;
		this->b = b;
		this->c = c;

		vector_a = new Vector(a->x, a->y, a->z);
		vector_b = new Vector(b->x, b->y, b->z);
		vector_c = new Vector(c->x, c->y, c->z);

		normal = vector_b->subtract(vector_a)->crossProduct(vector_c->subtract(vector_a));

		normal->normalize();

		d_of_plane = -normal->dot(vector_a);

		area = normal->abs() / 2;
	}
	double alphaOfBary(Vector *P)
	{
		return vector_b->subtract(P)->crossProduct(vector_c->subtract(P))->abs() / (2 * area);
	}
	double betaOfBary(Vector *P)
	{
		return vector_c->subtract(P)->crossProduct(vector_a->subtract(P))->abs() / (2 * area);
	}

	bool isInside(Vector *P)
	{
		double alpha = this->alphaOfBary(P);
		double beta = this->betaOfBary(P);
		double gamma = 1 - alpha - beta;
		if (0 <= alpha && alpha <= 1 &&
			0 <= beta && beta <= 1 &&
			0 <= gamma && gamma <= 1)
		{
			return true;
		}
		return false;
	}

	double getTOfIntersection(Ray *ray)
	{
		return -(d_of_plane + normal->dot(ray->start)) / (normal->dot(ray->dir));
	}
	void draw()
	{
		glColor3f(color[0], color[1], color[2]); //grey
		glBegin(GL_POLYGON);
		{
			glVertex3f(a->x, a->y, a->z);
			glVertex3f(b->x, b->y, b->z);
			glVertex3f(c->x, c->y, c->z);
		}
		glEnd();
	}
	void print()
	{
		cout << "Triangle: ";
		cout << "first point= ";
		a->print();
		cout << ", second point= ";
		b->print();
		cout << ", third point= ";
		c->print();
		cout << endl;
	}

	double intersect(Ray *ray, double *color, int level)
	{
		double t = this->getTOfIntersection(ray);
		if (t < 0)
			return -1;
		Vector *intersectionPoint = ray->getRayFromT(t);
		// bool isInside = this->isInside(intersectionPoint);
		bool isInside = this->point_in_triangle(intersectionPoint);
		if (!isInside)
			return -1;

		double intersectionPoint_red = this->color[0];
		double intersectionPoint_green = this->color[1];
		double intersectionPoint_blue = this->color[2];

		color[0] = intersectionPoint_red * coEfficients[AMB];
		color[1] = intersectionPoint_green * coEfficients[AMB];
		color[2] = intersectionPoint_blue * coEfficients[AMB];

		if (level == 0)
		{
			return t;
		}

		if(level >= levelOfRecursion)
			return t;

		for (int i = 0; i < lights.size(); i++)
		{
			Light *light = lights.at(i);
			Ray *lightRay = new Ray(light->position, intersectionPoint->subtract(light->position));

			Vector *V = ray->dir->negate();
			Vector *R = lightRay->dir->reflect(normal);

			double phong = R->dot(V) / (R->abs() * V->abs());

			if (phong > 0)
			{
				color[0] = min(1.0, color[0] + light->color[0] * coEfficients[SPEC] * pow(phong, shine) * intersectionPoint_red);
				color[1] = min(1.0, color[1] + light->color[1] * coEfficients[SPEC] * (pow(phong, shine)) * intersectionPoint_green);
				color[2] = min(1.0, color[2] + light->color[2] * coEfficients[SPEC] * (pow(phong, shine)) * intersectionPoint_blue);
			}

			Vector *L = lightRay->dir->negate();
			Vector *N = normal;

			double lambert = L->dot(N) / (L->abs() * N->abs());

			if (lambert > 0)
			{
				color[0] = min(1.0, color[0] + light->color[0] * coEfficients[DIFF] * lambert * intersectionPoint_red);
				color[1] = min(1.0, color[1] + light->color[1] * coEfficients[DIFF] * lambert * intersectionPoint_green);
				color[2] = min(1.0, color[2] + light->color[2] * coEfficients[DIFF] * lambert * intersectionPoint_blue);
			}

			if (level >= levelOfRecursion)
				return t;

			Ray* recursiveRay = new Ray(intersectionPoint,ray->dir->reflect(normal));

			double *color_recurse = new double[3];
			double tRecurse, tMinRecurse = INF;
			for(int k = 0 ; k < objects.size(); k++){
				Object* o = objects.at(k);
				if(o== this){
					continue;
				}
				tRecurse = o->intersect(recursiveRay,color_recurse,level+1);
				if(tRecurse!= 99999 && tRecurse>0 && tRecurse < tMinRecurse){
					tMinRecurse = tRecurse;
					color[0] = min(1.0,color[0]+coEfficients[RECUR]*color_recurse[0]);
					color[1] = min(1.0,color[1]+coEfficients[RECUR]*color_recurse[1]);
					color[2] = min(1.0,color[2]+coEfficients[RECUR]*color_recurse[2]);
				}
			}
		}
		return t;
	}

	bool same_side(Vector *p1, Vector *p2, Vector *a, Vector *b)
	{
		Vector *cp1 = b->subtract(a)->crossProduct(p1->subtract(a));
		Vector *cp2 = b->subtract(a)->crossProduct(p2->subtract(a));
		if (cp1->dot(cp2) >= 0)
		{
			return true;
		}
		return false;
	}

	bool point_in_triangle(Vector *p)
	{
		if (same_side(p, vector_a, vector_b, vector_c) && same_side(p, vector_b, vector_a, vector_c) && same_side(p, vector_c, vector_a, vector_b))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

class GQuadratic : Object
{
public:
	double A, B, C, D, E, F, G, H, I, J;
	GQuadratic(double A, double B, double C, double D, double E, double F, double G, double H, double I, double J, Vector *reference_point, double length, double width, double height)
	{
		this->A = A;
		this->B = B;
		this->C = C;
		this->D = D;
		this->E = E;
		this->F = F;
		this->G = G;
		this->H = H;
		this->I = I;
		this->J = J;

		this->reference_point = reference_point;
		this->length = length;
		this->width = width;
		this->height = height;
	}
	void draw() {}
	void print()
	{
		cout << "GQuadratic: ";
		cout << "A= " << A << ", ";
		cout << "B= " << B << ", ";
		cout << "C= " << C << ", ";
		cout << "D= " << D << ", ";
		cout << "E= " << E << ", ";
		cout << "F= " << F << ", ";
		cout << "G= " << G << ", ";
		cout << "H= " << H << ", ";
		cout << "I= " << I << ", ";
		cout << "J= " << J << ", ";
		reference_point->print();
		cout << ", ";
		cout << "length: " << this->length << ", ";
		cout << "width: " << this->width << ", ";
		cout << "height: " << this->height;
		cout << endl;
	}
};

class Floor : public Object
{
public:
	int checkboard_number = 25;
	int checkboard_box = 20;
	int rangeOfFloor = checkboard_number * checkboard_box;

	Floor()
	{
	}
	void draw()
	{
		bool color = false;
		glPushMatrix();
		glTranslatef(checkboard_box / 2, checkboard_box / 2, 0);
		for (int i = -checkboard_number; i < checkboard_number; i++)
		{
			color = !color;
			for (int j = -checkboard_number; j < checkboard_number; j++)
			{
				glPushMatrix();
				glColor3f(color, color, color);
				color = !color;
				glTranslatef(i * checkboard_box, j * checkboard_box, 0);
				drawSquare(checkboard_box / 2);
				glPopMatrix();
			}
		}
		glPopMatrix();
	}
	void print()
	{
		cout << "Floor:" << endl;
		cout << "Size of tile= " << checkboard_box << ", ";
		cout << "Number of tiles in a row= " << checkboard_number * 2 << ", ";
		cout << endl;
	}
	int getColorOfPixel(int i, int j)
	{
		if (abs(i) > rangeOfFloor || abs(j) > rangeOfFloor)
		{
			return -1;
		}
		return (((i + rangeOfFloor) / checkboard_box) % 2) == (((j + rangeOfFloor) / checkboard_box) % 2);
	}
	double intersect(Ray *ray, double *color, int level)
	{
		Vector *normal = new Vector(0, 0, 1);
		double t = -(ray->start->dot(normal)) / (ray->dir->dot(normal));
		Vector *intersection = ray->start->sum(ray->dir->multiply(t));
		double tile_color = getColorOfPixel(intersection->x, intersection->y);
		if (tile_color == -1)
			return -1;

		double intersectionPoint_red = tile_color;
		double intersectionPoint_green = tile_color;
		double intersectionPoint_blue = tile_color;

		color[0] = intersectionPoint_red * coEfficients[AMB];
		color[1] = intersectionPoint_green * coEfficients[AMB];
		color[2] = intersectionPoint_blue * coEfficients[AMB];

		if (level == 0)
		{
			return t;
		}

		for (int i = 0; i < lights.size(); i++)
		{
			Light *light = lights.at(i);
			Ray *lightRay = new Ray(light->position, intersection->subtract(light->position));

			Vector *V = ray->dir->negate();
			Vector *R = lightRay->dir->reflect(normal);

			double phong = R->dot(V) / (R->abs() * V->abs());

			if (phong > 0)
			{
				color[0] = min(1.0, color[0] + light->color[0] * coEfficients[SPEC] * pow(phong, shine) * intersectionPoint_red);
				color[1] = min(1.0, color[1] + light->color[1] * coEfficients[SPEC] * (pow(phong, shine)) * intersectionPoint_green);
				color[2] = min(1.0, color[2] + light->color[2] * coEfficients[SPEC] * (pow(phong, shine)) * intersectionPoint_blue);
			}

			Vector *L = lightRay->dir->negate();
			Vector *N = normal;

			double lambert = L->dot(N) / (L->abs() * N->abs());

			if (lambert > 0)
			{
				color[0] = min(1.0, color[0] + light->color[0] * coEfficients[DIFF] * lambert * intersectionPoint_red);
				color[1] = min(1.0, color[1] + light->color[1] * coEfficients[DIFF] * lambert * intersectionPoint_green);
				color[2] = min(1.0, color[2] + light->color[2] * coEfficients[DIFF] * lambert * intersectionPoint_blue);
			}

			if (level >= levelOfRecursion)
				return t;

			Ray* recursiveRay = new Ray(intersection,ray->dir->reflect(normal));

			double *color_recurse = new double[3];
			double tRecurse, tMinRecurse = INF;
			for(int k = 0 ; k < objects.size(); k++){
				Object* o = objects.at(k);
				if(o== this){
					continue;
				}
				tRecurse = o->intersect(recursiveRay,color_recurse,level+1);
				if(tRecurse!= 99999 && tRecurse>0 && tRecurse < tMinRecurse){
					tMinRecurse = tRecurse;
					color[0] = min(1.0,color[0]+coEfficients[RECUR]*color_recurse[0]);
					color[1] = min(1.0,color[1]+coEfficients[RECUR]*color_recurse[1]);
					color[2] = min(1.0,color[2]+coEfficients[RECUR]*color_recurse[2]);
				}
			}
		}

		return t;
	}
};
