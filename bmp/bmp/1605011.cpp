#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#define pi (2 * acos(0.0))
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
        this->data[row][col] = value;
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
    OneDArray(OneDArray *a) : TwoDArray(a) {
        this->set(3,1);
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
    OneDArray* multiply(double factor){
        OneDArray* returnArray = new OneDArray(this);
        returnArray->set(0,returnArray->get(0)*factor);
        returnArray->set(1,returnArray->get(1)*factor);
        returnArray->set(2,returnArray->get(2)*factor);
        return returnArray;
    }
    OneDArray* sum(OneDArray* a){
        OneDArray* returnArray = new OneDArray(this);
        returnArray->set(0,returnArray->get(0)+a->get(0));
        returnArray->set(1,returnArray->get(1)+a->get(1));
        returnArray->set(2,returnArray->get(2)+a->get(2));
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
};

class Vector : public OneDArray
{
public:
    Vector() : OneDArray() {}
    Vector(Vector *a) : OneDArray(a) {}
    static OneDArray* rotate(OneDArray* x, Vector* a, double theta){
        
        double cosineOfDegree = cos((double)degreeToRadian(theta));
		double sineOfDegree = sin((double)degreeToRadian(theta));
        
        OneDArray* sumVector = new OneDArray();
        sumVector = sumVector->sum(x->multiply(cosineOfDegree));
        sumVector = sumVector->sum(a->multiply(OneDArray::dotProduct(a,x))->multiply(1-cosineOfDegree));
        sumVector = sumVector->sum(OneDArray::crossProduct(a,x)->multiply(sineOfDegree));
        return sumVector;
    }

    static Matrix* generateRotationMatrix(Vector* a, double angle){
        Matrix* resultMatrix = new Matrix();
        
        Vector* i = new Vector(); i->set(0,1); i->set(3,1);
        Vector* j = new Vector(); j->set(1,1); j->set(3,1);
        Vector* k = new Vector(); k->set(2,1); k->set(3,1);
        
        OneDArray* c1; OneDArray* c2;OneDArray* c3;
        Vector* normalized_a = a->normalize();

        

        c1 = Vector::rotate(i,normalized_a,angle);
        c2 = Vector::rotate(j,normalized_a,angle);
        c3 = Vector::rotate(k,normalized_a,angle);

        resultMatrix->set(0,0,c1->get(0));
        resultMatrix->set(0,1,c2->get(0));
        resultMatrix->set(0,2,c3->get(0));

        resultMatrix->set(1,0,c1->get(1));
        resultMatrix->set(1,1,c2->get(1));
        resultMatrix->set(1,2,c3->get(1));

        resultMatrix->set(2,0,c1->get(2));
        resultMatrix->set(2,1,c2->get(2));
        resultMatrix->set(2,2,c3->get(2));

        resultMatrix->set(3,3,1);
        

        return resultMatrix;
    }

    Vector* normalize(){
        Vector* result = new Vector(this);

        double denominator = sqrt(result->get(0)*result->get(0)+result->get(1)*result->get(1)+result->get(2)*result->get(2));
        result->set(0,result->get(0)/denominator);
        result->set(1,result->get(1)/denominator);
        result->set(2,result->get(2)/denominator);

        return result;
    }


};
class Point : public OneDArray
{
public:
    Point() : OneDArray() {}
    Point(Point *a) : OneDArray(a) {}
    void normalize()
    {
        for (int i = 0; i < 4; i++)
        {
            this->set(i, this->get(i) / this->get(3));
        }
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
    Matrix* get_top(){
        return matrixStack[matrixStack.size() - 1];
    }
};

Stack stack;

int main()
{
    ifstream fin;
    fin.open("scene.txt");

    Point *eye = new Point();
    Vector *look = new Vector();
    Vector *up = new Vector();

    double fovY, aspectRatio, near, far;
    double numberInput;

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

    eye->print();
    look->print();
    up->print();
    cout << fovY << " " << aspectRatio << " " << near << " " << far << endl;

    Matrix* initMatrix = new Matrix();
    for(int i = 0 ; i < 4; i++){
        initMatrix->set(i,i,1);
    }
    
    stack.push(initMatrix);

    string option;
    while (true)
    {
        fin >> option;
        if (option == "triangle")
        {
            cout << "Triangle found" << endl;
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

            
            point1->print();
            point2->print();
            point3->print();
        }
        else if (option == "scale")
        {
            cout << "Scale found" << endl;
            Matrix *scaleMatrix = new Matrix();
            fin >> numberInput;
            scaleMatrix->set(0, 0, numberInput);
            fin >> numberInput;
            scaleMatrix->set(1, 1, numberInput);
            fin >> numberInput;
            scaleMatrix->set(2, 2, numberInput);
            scaleMatrix->set(3, 3, 1);
            scaleMatrix->print();
        }
        else if (option == "translate")
        {
            cout << "Translate found" << endl;
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
            translateMatrix->print();
        }
        else if (option == "rotate")
        {
            cout << "Rotate found" << endl;
            Vector* axis = new Vector();
            double angle;
            fin >> angle;
            fin >> numberInput;
            axis->set(0,numberInput);
            fin >> numberInput; 
            axis->set(1,numberInput);
            fin >> numberInput; 
            axis->set(2,numberInput);
            axis->set(3,1);

            
            
            Matrix* rotationMatrix = Vector::generateRotationMatrix(axis,angle);

            rotationMatrix->print();
            
          
        }
        else if (option == "push")
        {
            cout << "Push found" << endl;
        }
        else if (option == "pop")
        {
            cout << "Pop found" << endl;
        }
        else if (option == "end")
        {
            cout<< "End found" << endl;
            break;
        }
    }
}
