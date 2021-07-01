class Ray{
public:
    Vector* start, *dir;
	Ray(Vector* start, Vector* dir){
		this->start = start;
		this->dir = dir;
	}
};

class Object
{
public:
	Vector *reference_point;
	double height, width, length;
	double color[3];
	double coEfficients[4];
	int shine;
	virtual void draw() {}
	void setColor(double red, double green, double blue) {
		color[0]=red;
		color[1]=green;
		color[2]=blue;
	}
	void setShine(int shine){
		this->shine = shine;
	}
	void setCoEfficients(double ambient, double diffuse, double specular, double recurseCoeff) {
		this->coEfficients[0]= ambient;
		this->coEfficients[1]= diffuse;
		this->coEfficients[2]= specular;
		this->coEfficients[3]= recurseCoeff;
	}
	virtual void print(){}
	void printCommons(){
		cout<<"Color= ("<<color[0]<<","<<color[1]<<","<<color[2]<<"), ";
		cout<<"Ambient= "<<coEfficients[0]<<", ";
		cout<<"Diffuse= "<<coEfficients[1]<<", ";
		cout<<"Specular= "<<coEfficients[2]<<", ";
		cout<<"Recursive reflection= "<<coEfficients[3]<<", ";
		cout<<"Shine= "<<shine<<", ";
		cout<<endl;
	}
	virtual double intersect(Ray* ray, double* color, int level){return INF}
};

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
	void print(){
		cout<<"Sphere: ";
		cout<<"Center= ";
		reference_point->print();
		cout<<", radius= "<<this->length;
		cout<<endl;
	}
	virtual double intersect(Ray* ray, double* color, int level){
		Vector* translatedOrigin = ray->start->subtract(this->reference_point);

		double a = ray->dir->dot(ray->dir);
		double b = 2 * ray->dir->dot(translatedOrigin);
		double c = translatedOrigin->dot(translatedOrigin)-this->length*this->length;
		double d_square = b*b-4*a*c;
		if(d_square<0)return -1;
		double d = sqrt(d_square);
		color[0]=this->color[0];color[1]=this->color[1];color[2]=this->color[2];
		double t = (-b-d)/(2*a);
		
		return t;
	}
};

class Triangle : Object
{
public:
	Point *a, *b, *c;
	Triangle(Point *a, Point *b, Point *c)
	{
		this->a = a;
		this->b = b;
		this->c = c;
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
	void print(){
		cout<<"Triangle: ";
		cout<<"first point= ";
		a->print();
		cout<<", second point= ";
		b->print();
		cout<<", third point= ";
		c->print();
		cout<<endl;
	}
};


class GQuadratic: Object{
public:
	double A, B, C, D, E, F, G, H, I, J;
	GQuadratic(double A,double B,double C,double D,double E,double F,double G,double H,double I,double J, Vector* reference_point, double length, double width, double height){
		this->A=A;
		this->B=B;
		this->C=C;
		this->D=D;
		this->E=E;
		this->F=F;
		this->G=G;
		this->H=H;
		this->I=I;
		this->J=J;

		this->reference_point = reference_point;
		this->length = length;
		this->width = width;
		this->height = height;
	}
	void draw(){}
	void print(){
		cout<<"GQuadratic: ";
		cout<<"A= "<<A<<", ";
		cout<<"B= "<<B<<", ";
		cout<<"C= "<<C<<", ";
		cout<<"D= "<<D<<", ";
		cout<<"E= "<<E<<", ";
		cout<<"F= "<<F<<", ";
		cout<<"G= "<<G<<", ";
		cout<<"H= "<<H<<", ";
		cout<<"I= "<<I<<", ";
		cout<<"J= "<<J<<", ";
		reference_point->print();cout<<", ";
		cout<<"length: "<<this->length<<", ";
		cout<<"width: "<<this->width<<", ";
		cout<<"height: "<<this->height;
		cout<<endl;
	}
};



class Floor: public Object{
public:
    int checkboard_number = 25;
    int checkboard_box = 20;
    int rangeOfFloor = checkboard_number* checkboard_box;

    Floor(){

    }
    void draw(){
        bool color= false;
        glPushMatrix();
        glTranslatef(checkboard_box/2,checkboard_box/2,0);
        for(int i = -checkboard_number ; i < checkboard_number; i++){
            color=!color;
            for(int j = -checkboard_number ; j < checkboard_number; j++){
                glPushMatrix();
                glColor3f(color,color,color);
                color=!color;
                glTranslatef(i*checkboard_box,j*checkboard_box,0);
                drawSquare(checkboard_box/2);
                glPopMatrix();
            }
        }
        glPopMatrix();
    }
    void print(){
        cout<<"Floor:"<<endl;
		cout<<"Size of tile= "<<checkboard_box<<", ";
		cout<<"Number of tiles in a row= "<<checkboard_number*2<<", ";
		cout<<endl;
    }
    int getColorOfPixel(int i, int j){
        if(abs(i)>rangeOfFloor || abs(j)>rangeOfFloor){
            return -1;
        }
        return (((i+rangeOfFloor)/checkboard_box)%2) == (((j+rangeOfFloor)/checkboard_box)%2);
    }
	double intersect(Ray* ray, double* color, int level){
		Vector* floorNormal = new Vector(0,0,1);
		double t = -(ray->start->dot(floorNormal))/(ray->dir->dot(floorNormal));
		Vector* intersection = ray->start->sum(ray->dir->multiply(t));
		int tile_color = getColorOfPixel(intersection->x,intersection->y);
		if(tile_color==-1)return INF;
		color[0]=tile_color;
		color[1]=tile_color;
		color[2]=tile_color;
		return t;
	}
};

class Light
{
	public:
	Vector* position;
	double color[3];
	Light(Vector* position,double red, double green, double blue){
		this->position = position;	
		this->color[0]=red;
		this->color[1]=green;
		this->color[2]=blue;
	}

	void draw(){
		glColor3f(color[0], color[1], color[2]); //grey
		glPushMatrix();
		glTranslatef(position->x, position->y, position->z);
		drawSphere(1, 10, 10);
		glPopMatrix();
	}
	void print(){
		cout<<"Light= ";
		position->print();
		cout<<", color= ("<<color[0]<<","<<color[1]<<","<<color[2]<<")";
		cout<<endl;
	}
};

