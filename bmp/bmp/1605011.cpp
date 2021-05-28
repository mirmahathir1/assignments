#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <algorithm>
#define INFINITY 9999
#define pi (2 * acos(0.0))
#include "bitmap_image.hpp"
using namespace std;

double degreeToRadian(double degree)
{
    return (degree * (pi / 180.0));
}

class TwoDArray
{
protected:
    int row, col;
    double **data;
    double get(int row, int col)
    {
        check(row, col);
        return this->data[row][col];
    }
    void check(int row, int col)
    {
        if (row >= this->row || col >= this->col)
        {
            cout << "ERROR: TwoDArray index out of bound" << endl;
            exit(0);
        }
    }
    double set(int row, int col, double value)
    {
        check(row, col);
        if (value < 1.0 / 100 && value > -1.0 / 100)
        {
            this->data[row][col] = 0;
        }
        else
        {
            this->data[row][col] = value;
        }
    }

public:
    TwoDArray(int row, int col)
    {
        this->row = row;
        this->col = col;
        this->data = new double *[row];
        for (int i = 0; i < row; i++)
        {
            this->data[i] = new double[col];
        }
    }
    TwoDArray(TwoDArray *a)
    {
        this->row = a->row;
        this->col = a->col;
        this->data = new double *[row];
        for (int i = 0; i < row; i++)
        {
            this->data[i] = new double[col];
            for (int j = 0; j < col; j++)
            {
                this->data[i][j] = a->data[i][j];
            }
        }
    }
    static TwoDArray *multiply(TwoDArray *a, TwoDArray *b)
    {
        if (a->col != b->row)
        {
            cout << "ERROR: TwoDArray multiplication invalid dimensions" << endl;
            exit(0);
        }
        TwoDArray *resultArray = new TwoDArray(a->row, b->col);

        for (int i = 0; i < a->row; i++)
        {
            for (int j = 0; j < b->col; j++)
            {

                for (int k = 0; k < a->col; k++)
                    resultArray->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
        return resultArray;
    }
    static TwoDArray *subtract(TwoDArray *a, TwoDArray *b)
    {
        if (a->row != b->row || a->col != b->col)
        {
            cout << "ERROR: dimension mismatch in subtraction" << endl;
            exit(0);
        }

        TwoDArray *resultArray = new TwoDArray(a->row, a->col);

        for (int i = 0; i < a->row; i++)
        {
            for (int j = 0; j < a->col; j++)
            {
                resultArray->set(i,j,a->get(i,j)-b->get(i,j));
            }
        }
        return resultArray;
    }
    void print()
    {
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                cout << this->data[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
};

class OneDArray : public TwoDArray
{
public:
    OneDArray() : TwoDArray(4, 1) {}
    OneDArray(OneDArray *a) : TwoDArray(a)
    {
        this->set(3, 1);
    }

public:
    void set(int index, double value)
    {
        TwoDArray::set(index, 0, value);
    }
    double get(int index)
    {
        return TwoDArray::get(index, 0);
    }
    static OneDArray *crossProduct(OneDArray *a, OneDArray *b)
    {
        OneDArray *crossProductResult = new OneDArray();
        crossProductResult->set(0, a->get(1) * b->get(2) - a->get(2) * b->get(1));
        crossProductResult->set(1, a->get(2) * b->get(0) - a->get(0) * b->get(2));
        crossProductResult->set(2, a->get(0) * b->get(1) - a->get(1) * b->get(0));
        crossProductResult->set(3, 1);
        return crossProductResult;
    }
    static double dotProduct(OneDArray *a, OneDArray *b)
    {
        double product = 0;
        for (int i = 0; i < 3; i++)
            product = product + a->get(i) * b->get(i);
        return product;
    }
    OneDArray *multiply(double factor)
    {
        OneDArray *returnArray = new OneDArray(this);
        returnArray->set(0, returnArray->get(0) * factor);
        returnArray->set(1, returnArray->get(1) * factor);
        returnArray->set(2, returnArray->get(2) * factor);
        return returnArray;
    }
    OneDArray *sum(OneDArray *a)
    {
        OneDArray *returnArray = new OneDArray(this);
        returnArray->set(0, returnArray->get(0) + a->get(0));
        returnArray->set(1, returnArray->get(1) + a->get(1));
        returnArray->set(2, returnArray->get(2) + a->get(2));
        return returnArray;
    }
};

class Matrix : public TwoDArray
{
public:
    Matrix() : TwoDArray(4, 4) {}
    Matrix(Matrix *a) : TwoDArray(a) {}
    void set(int row, int col, double value)
    {
        TwoDArray::set(row, col, value);
    }
    double get(int row, int col)
    {
        return TwoDArray::get(row, col);
    }
    void init_as_identity()
    {
        for (int i = 0; i < 4; i++)
        {
            this->set(i, i, 1);
        }
    }
};

class Point : public OneDArray
{
public:
    Point() : OneDArray() {}
    Point(Point *a) : OneDArray(a) {}
    Point(double x, double y, double z):OneDArray(){
        this->set(0,x);
        this->set(1,y);
        this->set(2,z);
    }
    void scale()
    {
        for (int i = 0; i < 4; i++)
        {
            this->set(i, this->get(i) / this->get(3));
        }
    }
    Point *transform(Matrix *a)
    {
        Point *returnValue = (Point *)TwoDArray::multiply(a, this);
        returnValue->scale();
        return returnValue;
    }
};



class Vector : public OneDArray
{
public:
    Vector() : OneDArray() {}
    Vector(Vector *a) : OneDArray(a) {}
    static OneDArray *rotate(OneDArray *x, Vector *a, double theta)
    {

        double cosineOfDegree = cos((double)degreeToRadian(theta));
        double sineOfDegree = sin((double)degreeToRadian(theta));

        OneDArray *sumVector = new OneDArray();
        sumVector = sumVector->sum(x->multiply(cosineOfDegree));
        sumVector = sumVector->sum(a->multiply(OneDArray::dotProduct(a, x))->multiply(1 - cosineOfDegree));
        sumVector = sumVector->sum(OneDArray::crossProduct(a, x)->multiply(sineOfDegree));
        return sumVector;
    }

    static Matrix *generateRotationMatrix(Vector *a, double angle)
    {
        Matrix *resultMatrix = new Matrix();

        Vector *i = new Vector();
        i->set(0, 1);
        i->set(3, 1);
        Vector *j = new Vector();
        j->set(1, 1);
        j->set(3, 1);
        Vector *k = new Vector();
        k->set(2, 1);
        k->set(3, 1);

        OneDArray *c1;
        OneDArray *c2;
        OneDArray *c3;
        Vector *normalized_a = a->normalize();

        c1 = Vector::rotate(i, normalized_a, angle);
        c2 = Vector::rotate(j, normalized_a, angle);
        c3 = Vector::rotate(k, normalized_a, angle);

        resultMatrix->set(0, 0, c1->get(0));
        resultMatrix->set(0, 1, c2->get(0));
        resultMatrix->set(0, 2, c3->get(0));

        resultMatrix->set(1, 0, c1->get(1));
        resultMatrix->set(1, 1, c2->get(1));
        resultMatrix->set(1, 2, c3->get(1));

        resultMatrix->set(2, 0, c1->get(2));
        resultMatrix->set(2, 1, c2->get(2));
        resultMatrix->set(2, 2, c3->get(2));

        resultMatrix->set(3, 3, 1);

        return resultMatrix;
    }

    Vector *normalize()
    {
        Vector *result = new Vector(this);

        double denominator = sqrt(result->get(0) * result->get(0) + result->get(1) * result->get(1) + result->get(2) * result->get(2));
        result->set(0, result->get(0) / denominator);
        result->set(1, result->get(1) / denominator);
        result->set(2, result->get(2) / denominator);

        return result;
    }

    Vector *subtract(Point *a)
    {
        Vector *result = new Vector(this);
        result->set(0, result->get(0) - a->get(0));
        result->set(1, result->get(1) - a->get(1));
        result->set(2, result->get(2) - a->get(2));
        return result;
    }
};
class Triangle : public TwoDArray
{
public:
    int color[3] = {0, 0, 0};
    double max_y = -INFINITY;
    double min_y = INFINITY;
    double max_x = -INFINITY;
    double min_x = INFINITY;
    Triangle(Point *a, Point *b, Point *c) : TwoDArray(3, 3)
    {
        vector<Point *> points;

        points.push_back(a);
        points.push_back(b);
        points.push_back(c);

        sort(points.begin(), points.end(), [](Point *left, Point *right) {
            return left->get(1) < right->get(1);
        });

        // cout<<"Printing sorted points: "<<endl;
        // for(int i = 0 ; i < points.size(); i++){
        //     points.at(i)->print();
        // }

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                this->set(j, i, points.at(j)->get(i));
            }
            // this->set(0,i,a->get(i));
            // this->set(1,i,b->get(i));
            // this->set(2,i,c->get(i));
        }

        for (int i = 0; i < 3; i++)
        {
            if (this->get(i, 0) < min_x)
            {
                min_x = this->get(i, 0);
            }
            if (this->get(i, 0) > max_x)
            {
                max_x = this->get(i, 0);
            }
            if (this->get(i, 1) < min_y)
            {
                min_y = this->get(i, 1);
            }
            if (this->get(i, 1) > max_y)
            {
                max_y = this->get(i, 1);
            }
        }

        this->color[0] = this->getRandom(0, 255);
        this->color[1] = this->getRandom(0, 255);
        this->color[2] = this->getRandom(0, 255);
    }
    int getRandom(int min, int max)
    {
        return min + (std::rand() % (max - min + 1));
    }
    void print()
    {
        TwoDArray::print();
        cout << "Color: R=" << this->color[0] << ", G=" << this->color[1] << ", B=" << this->color[2] << endl;
        cout << "min_x: " << min_x << ", max_x: " << max_x << ", min_y: " << min_y << ", max_y:" << max_y << endl;
    }
    double *getIntersections(double y)
    {
        if (y < get(0, 1))
        {
            cout << "ERROR: scan line is passing over the triangle" << endl;
            exit(0);
        }
        if (y > get(2, 1))
        {
            cout << "ERROR: scan line is passing below the triangle" << endl;
            exit(0);
        }
        double x1, x2, y1, y2, x3, y3, x4, y4, z1, z2, z3, z4;
        x3 = this->get(0, 0);
        y3 = this->get(0, 1);
        z3 = this->get(0, 2);
        x4 = this->get(2, 0);
        y4 = this->get(2, 1);
        z4 = this->get(2, 2);
        if (y < this->get(1, 1))
        {
            x1 = this->get(0, 0);
            y1 = this->get(0, 1);
            z1 = this->get(0, 2);
            x2 = this->get(1, 0);
            y2 = this->get(1, 1);
            z2 = this->get(1, 2);
        }
        else if (y < this->get(2, 1))
        {
            x1 = this->get(1, 0);
            y1 = this->get(1, 1);
            z1 = this->get(1, 2);
            x2 = this->get(2, 0);
            y2 = this->get(2, 1);
            z2 = this->get(2, 2);
        }
        else if (y == this->get(2, 1))
        {
            cout << "ERROR: line is passing though the lowest point" << endl;
            exit(0);
        }

        double *x_return = new double[4];

        x_return[0] = ((y - y1) * (x1 - x2)) / (y1 - y2) + x1;
        x_return[2] = ((y - y1) * (z1 - z2)) / (y1 - y2) + z1;


        x_return[1] = ((y - y3) * (x3 - x4)) / (y3 - y4) + x3;
        x_return[3] = ((y - y3) * (z3 - z4)) / (y3 - y4) + z3;

        if (x_return[0] > x_return[1])
        {
            double temp = x_return[0];
            x_return[0] = x_return[1];
            x_return[1] = temp;

            temp = x_return[2];
            x_return[2] = x_return[3];
            x_return[3] = temp;
        }

        return x_return;
    }

};
class Stack
{
private:
    vector<Matrix *> matrixStack;

public:
    void push(Matrix *a)
    {
        matrixStack.push_back(a);
    }
    Matrix *pop()
    {
        if (isEmpty())
        {
            cout << "ERROR: Trying to POP from stack with no element" << endl;
            exit(0);
        }

        Matrix *popped = matrixStack[matrixStack.size() - 1];
        matrixStack.pop_back();
        return popped;
    }
    bool isEmpty()
    {
        return matrixStack.size() == 0;
    }
    Matrix *get_top()
    {
        return matrixStack[matrixStack.size() - 1];
    }

    void multiply_with_top(Matrix *a)
    {
        Matrix *top = this->pop();
        // this->push((Matrix *)TwoDArray::multiply(a, top));
        this->push((Matrix *)TwoDArray::multiply(top, a));
    }
};

ifstream fin;
ifstream finConfig;
ofstream f1out;
ofstream f2out;
ofstream f3out;
ofstream foutZbuffer;

void print_triangle_of_stage1(Point *a, Point *b, Point *c)
{
    f1out << a->get(0) << " " << a->get(1) << " " << a->get(2) << endl;
    f1out << b->get(0) << " " << b->get(1) << " " << b->get(2) << endl;
    f1out << c->get(0) << " " << c->get(1) << " " << c->get(2) << endl;
    f1out << endl;
}

void print_triangle_of_stage2(Point *a, Point *b, Point *c)
{
    f2out << a->get(0) << " " << a->get(1) << " " << a->get(2) << endl;
    f2out << b->get(0) << " " << b->get(1) << " " << b->get(2) << endl;
    f2out << c->get(0) << " " << c->get(1) << " " << c->get(2) << endl;
    f2out << endl;
}

void print_triangle_of_stage3(Point *a, Point *b, Point *c)
{
    f3out << a->get(0) << " " << a->get(1) << " " << a->get(2) << endl;
    f3out << b->get(0) << " " << b->get(1) << " " << b->get(2) << endl;
    f3out << c->get(0) << " " << c->get(1) << " " << c->get(2) << endl;
    f3out << endl;
}

Stack stack;
vector<Triangle *> triangles;

int main()
{

    fin.open("scene.txt");
    f1out.open("stage1.txt");
    f2out.open("stage2.txt");
    f3out.open("stage3.txt");
    foutZbuffer.open("z_buffer.txt");

    Point *eye = new Point();
    Vector *look = new Vector();
    Vector *up = new Vector();

    double fovY, aspectRatio, near, far;
    double numberInput;
    double numberOfTriangles = 0;

    fin >> numberInput;
    eye->set(0, numberInput);
    fin >> numberInput;
    eye->set(1, numberInput);
    fin >> numberInput;
    eye->set(2, numberInput);
    eye->set(3, 1);
    fin >> numberInput;
    look->set(0, numberInput);
    fin >> numberInput;
    look->set(1, numberInput);
    fin >> numberInput;
    look->set(2, numberInput);
    look->set(3, 1);
    fin >> numberInput;
    up->set(0, numberInput);
    fin >> numberInput;
    up->set(1, numberInput);
    fin >> numberInput;
    up->set(2, numberInput);
    up->set(3, 1);
    fin >> fovY >> aspectRatio >> near >> far;

    // eye->print();
    // look->print();
    // up->print();
    // cout << fovY << " " << aspectRatio << " " << near << " " << far << endl;

    //STAGE 2 PROCESSING

    Vector *l = look->subtract(eye);
    l = l->normalize();

    Vector *r = (Vector *)OneDArray::crossProduct(l, up);
    r = r->normalize();

    Vector *u = (Vector *)OneDArray::crossProduct(r, l);

    Matrix *T = new Matrix();
    T->init_as_identity();
    T->set(0, 3, -eye->get(0));
    T->set(1, 3, -eye->get(1));
    T->set(2, 3, -eye->get(2));

    Matrix *R = new Matrix();
    R->init_as_identity();
    for (int i = 0; i < 3; i++)
    {
        R->set(0, i, r->get(i));
        R->set(1, i, u->get(i));
        R->set(2, i, -l->get(i));
    }

    Matrix *V = (Matrix *)TwoDArray::multiply(R, T);
    // V->print();

    //STAGE 3 PROCESSING

    double fovX = fovY * aspectRatio;
    double tP = near * tan((double)degreeToRadian(fovY / 2));
    double rP = near * tan((double)degreeToRadian(fovX / 2));
    Matrix *P = new Matrix();
    P->set(0, 0, near / rP);
    P->set(1, 1, near / tP);
    P->set(2, 2, -(far + near) / (far - near));
    P->set(2, 3, -(2 * far * near) / (far - near));
    P->set(3, 2, -1);
    // cout<<"Printing P: "<<endl;
    // P->print();

    Matrix *initMatrix = new Matrix();
    initMatrix->init_as_identity();

    stack.push(initMatrix);

    string option;
    while (true)
    {
        fin >> option;
        if (option == "triangle")
        {
            // cout << "Triangle found" << endl;
            numberOfTriangles++;
            Point *point1 = new Point();
            Point *point2 = new Point();
            Point *point3 = new Point();
            fin >> numberInput;
            point1->set(0, numberInput);
            fin >> numberInput;
            point1->set(1, numberInput);
            fin >> numberInput;
            point1->set(2, numberInput);
            point1->set(3, 1);
            fin >> numberInput;
            point2->set(0, numberInput);
            fin >> numberInput;
            point2->set(1, numberInput);
            fin >> numberInput;
            point2->set(2, numberInput);
            point2->set(3, 1);
            fin >> numberInput;
            point3->set(0, numberInput);
            fin >> numberInput;
            point3->set(1, numberInput);
            fin >> numberInput;
            point3->set(2, numberInput);
            point3->set(3, 1);

            // cout<<"points before transformation: "<<endl;
            // point1->print();
            // point2->print();
            // point3->print();

            // cout<<"points after transformation: "<<endl;
            // TwoDArray::multiply(stack.get_top(), point1)->print();
            // TwoDArray::multiply(stack.get_top(), point2)->print();
            // TwoDArray::multiply(stack.get_top(), point3)->print();
            // print_triangle_of_stage1(
            //     (Point*)TwoDArray::multiply(stack.get_top(), point1),
            //     (Point*)TwoDArray::multiply(stack.get_top(), point2),
            //     (Point*)TwoDArray::multiply(stack.get_top(), point3)
            // );

            Point *stage1_point1 = point1->transform(stack.get_top());
            Point *stage1_point2 = point2->transform(stack.get_top());
            Point *stage1_point3 = point3->transform(stack.get_top());

            print_triangle_of_stage1(
                stage1_point1,
                stage1_point2,
                stage1_point3);

            Point *stage2_point1 = stage1_point1->transform(V);
            Point *stage2_point2 = stage1_point2->transform(V);
            Point *stage2_point3 = stage1_point3->transform(V);

            print_triangle_of_stage2(
                stage2_point1,
                stage2_point2,
                stage2_point3);

            Point *stage3_point1 = stage2_point1->transform(P);
            Point *stage3_point2 = stage2_point2->transform(P);
            Point *stage3_point3 = stage2_point3->transform(P);

            print_triangle_of_stage3(
                stage3_point1,
                stage3_point2,
                stage3_point3);

            triangles.push_back(new Triangle(stage3_point1, stage3_point2, stage3_point3));
        }
        else if (option == "scale")
        {
            // cout << "Scale found" << endl;
            Matrix *scaleMatrix = new Matrix();
            fin >> numberInput;
            scaleMatrix->set(0, 0, numberInput);
            fin >> numberInput;
            scaleMatrix->set(1, 1, numberInput);
            fin >> numberInput;
            scaleMatrix->set(2, 2, numberInput);
            scaleMatrix->set(3, 3, 1);
            // scaleMatrix->print();

            // cout << "stack before multiplying with top: " << endl;
            // stack.get_top()->print();
            stack.multiply_with_top(scaleMatrix);
            // cout << "stack after multiplying with top: " << endl;
            // stack.get_top()->print();
        }
        else if (option == "translate")
        {
            // cout << "Translate found" << endl;
            Matrix *translateMatrix = new Matrix();
            fin >> numberInput;
            translateMatrix->set(0, 3, numberInput);
            fin >> numberInput;
            translateMatrix->set(1, 3, numberInput);
            fin >> numberInput;
            translateMatrix->set(2, 3, numberInput);
            translateMatrix->set(0, 0, 1);
            translateMatrix->set(1, 1, 1);
            translateMatrix->set(2, 2, 1);
            translateMatrix->set(3, 3, 1);
            // translateMatrix->print();

            // cout << "stack before multiplying with top: " << endl;
            // stack.get_top()->print();
            stack.multiply_with_top(translateMatrix);
            // cout << "stack after multiplying with top: " << endl;
            // stack.get_top()->print();
        }
        else if (option == "rotate")
        {
            // cout << "Rotate found" << endl;
            Vector *axis = new Vector();
            double angle;
            fin >> angle;
            fin >> numberInput;
            axis->set(0, numberInput);
            fin >> numberInput;
            axis->set(1, numberInput);
            fin >> numberInput;
            axis->set(2, numberInput);
            axis->set(3, 1);

            Matrix *rotationMatrix = Vector::generateRotationMatrix(axis, angle);

            // rotationMatrix->print();

            // cout << "stack before multiplying with top: " << endl;
            // stack.get_top()->print();
            stack.multiply_with_top(rotationMatrix);
            // cout << "stack after multiplying with top: " << endl;
            // stack.get_top()->print();
        }
        else if (option == "push")
        {
            // cout << "Push found" << endl;

            stack.push(stack.get_top());
        }
        else if (option == "pop")
        {
            // cout << "Pop found" << endl;
            stack.pop();
        }
        else if (option == "end")
        {
            // cout << "End found" << endl;
            break;
        }
    }

    //################################STARTING STAGE 4###############################
    cout << "Amount of triangles: " << triangles.size() << endl;
    for (int i = 0; i < triangles.size(); i++)
    {
        triangles.at(i)->print();
        cout << endl;
    }

    finConfig.open("config.txt");
    int screen_width, screen_height;
    double left_limit_x, bottom_limit_y, front_limit_z, rear_limit_z;
    double right_limit_x, top_limit_y;

    finConfig >> screen_width >> screen_height >> left_limit_x >> bottom_limit_y >> front_limit_z >> rear_limit_z;
    right_limit_x = -left_limit_x;
    top_limit_y = -bottom_limit_y;

    cout << "screen_width: " << screen_width << endl;
    cout << "screen_height: " << screen_height << endl;
    cout << "left_limit_x: " << left_limit_x << endl;
    cout << "bottom_limit_y: " << bottom_limit_y << endl;
    cout << "front_limit_z: " << front_limit_z << endl;
    cout << "rear_limit_z: " << rear_limit_z << endl;

    bitmap_image image(screen_width, screen_height);
    for (int i = 0; i < screen_height; i++)
    {
        for (int j = 0; j < screen_width; j++)
        {
            image.set_pixel(i, j, 0, 0, 0);
        }
    }

    //keep the z distance of the point in the triangle
    double **z_buffer = new double *[screen_height];
    for (int i = 0; i < screen_height; i++)
    {
        z_buffer[i] = new double[screen_width];
        for (int j = 0; j < screen_width; j++)
        {
            z_buffer[i][j] = rear_limit_z;
        }
    }

    //keep the number of triangle that is going to be shown. -1 means no color
    int **frame_buffer = new int *[screen_height];
    for (int i = 0; i < screen_height; i++)
    {
        frame_buffer[i] = new int[screen_width];
        for (int j = 0; j < screen_width; j++)
        {
            frame_buffer[i][j] = -1;
        }
    }

    double dx = (right_limit_x - left_limit_x) / screen_width;
    double dy = (top_limit_y - bottom_limit_y) / screen_height;
    double top_y = (top_limit_y - dy / 2);
    double bottom_y = (bottom_limit_y + dy / 2);
    double left_x = (left_limit_x + dx / 2);
    double right_x = (right_limit_x - dx / 2);

    //printing the z buffer to file
    for (int i = 0; i < screen_height; i++)
    {
        for (int j = 0; j < screen_width; j++)
        {
            foutZbuffer << z_buffer[i][j] << '\t';
        }
        foutZbuffer << endl;
    }

    cout << "triangles.size(): " << triangles.size() << endl;

    for (int i = 0; i < triangles.size(); i++)
    {
        cout << "start scan of triangle" << endl;
        Triangle *triangle = triangles.at(i);
        triangle->print();

        double clipped_max_y = min(top_y, triangle->max_y);
        double clipped_min_y = max(bottom_y, triangle->min_y);

        cout << "clipped_max_y: " << clipped_max_y << ", clipped_min_y: " << clipped_min_y << endl;
        double current_y = top_y;
        for (int j = 0; j < screen_height; j++)
        {
            current_y -= dy;

            if (current_y < clipped_min_y)
                break;
            if (current_y > clipped_max_y)
                continue;

            // cout<<"current_y: "<<current_y<<endl;
            double *intersects = triangle->getIntersections(current_y);

            // cout<<"(x1,z1): ("<<intersects[0]<<","<<intersects[2]<<"), (x2,z2): ("<<intersects[1]<<","<<intersects[3]<<")"<<endl;

            double clipped_min_x = max(intersects[0], left_x);
            double clipped_max_x = min(intersects[1], right_x);


            double z_of_clipped_min_x = ((clipped_min_x - intersects[0]) * (intersects[2] - intersects[3])) / (intersects[0] - intersects[1]) + intersects[2];
            double z_of_clipped_max_x = ((clipped_max_x - intersects[0]) * (intersects[2] - intersects[3])) / (intersects[0] - intersects[1]) + intersects[2];

            double current_x = left_x;
            for (int k = 0; k < screen_width; k++)
            {
                current_x += dx;

                if (current_x < clipped_min_x)
                    continue;
                if (current_x > clipped_max_x)
                    break;

                double current_z = ((current_x - clipped_min_x) * (z_of_clipped_min_x - z_of_clipped_max_x)) / (clipped_min_x - clipped_max_x) + z_of_clipped_min_x;


                // frame_buffer[j][k]=i;
                if(current_z > front_limit_z && current_z < rear_limit_z && current_z < z_buffer[j][k] ){
                    image.set_pixel(k, j, triangle->color[0], triangle->color[1], triangle->color[2]);
                    z_buffer[j][k]= current_z;
                }

                // cout<<"current x: "<<current_x<<endl;
            }
        }

        cout << "completed scan of triangle " << i << endl;

        // if(i==6)break;
    }

    image.save_image("out.bmp");



}
