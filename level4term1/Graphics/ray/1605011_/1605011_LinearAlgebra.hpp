
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
		cout << "vector: x = " << x << ", y = " << y << ", z = " << z;
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

	Vector* multiply(double multiplier){
		return new Vector(this->x*multiplier,this->y*multiplier, this->z*multiplier);
	}

	Vector* sum(Vector* a){
		return new Vector(this->x + a->x, this->y + a->y, this->z + a->z);
	}

	Vector* subtract(Vector* a){
		return new Vector(this->x - a->x, this->y - a->y, this->z - a->z);
	}

	// Vector* subtract(Point* a){
	// 	return new Vector(this->x - a->x, this->y - a->y, this->z - a->z);
	// }

	double dot(Vector* a){
		return this->x*a->x+this->y*a->y+this->z*a->z;
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
		cout << "point: x = " << x << ", y = " << y << ", z = " << z;
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
	Vector* sum(Vector* a){
		return new Vector(this->x+a->x,this->y+a->y, this->z+a->z);
	}
};