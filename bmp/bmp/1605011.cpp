#include <iostream>
#include <vector>
using namespace std;

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
protected:
    OneDArray() : TwoDArray(4, 1) {}
    OneDArray(OneDArray *a) : TwoDArray(a) {}

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
};
class Point : public OneDArray
{
public:
    Point() : OneDArray() {}
    Point(Point *a) : OneDArray(a) {}
};

class Stack{
private:
    vector<Matrix*> matrixStack;
public:
    void push(Matrix* a){
        matrixStack.push_back(a);
    }
    Matrix* pop(){
        if(isEmpty()){
            cout << "ERROR: Trying to POP from stack with no element" << endl;
            exit(0);
        }

        Matrix* popped = matrixStack[matrixStack.size()-1];
        matrixStack.pop_back();
        return popped;
    }
    bool isEmpty(){
        return matrixStack.size()==0;
    }
};

Stack stack;

int main()
{

    Matrix *a = new Matrix();
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            //a->data[i][j]= i+1;
            a->set(i, j, i + 1);
        }
    }

    Matrix *b = new Matrix(a);

    Matrix *c = new Matrix((Matrix *)TwoDArray::multiply(a, b));

    

    stack.push(a);
    a->print();
    stack.push(b);
    b->print();
    stack.push(c);
    c->print();
    cout<<"POPPING"<<endl;
    while(!stack.isEmpty()){
        Matrix* popped = stack.pop();
        popped->print();
    }
}
