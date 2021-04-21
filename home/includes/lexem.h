#ifndef LEXEM_H
#define LEXEM_H
#include "const.h"
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <stack>
using namespace std;
class Lexem;
class Number;
class Operators;
class Variable;
class Goto;
class Array;
extern map <string, Variable *> var_table;
extern map <string, int> lable_table;
extern map <string, int *> array_table;

class Lexem {
public:
    Lexem();
    virtual OPERATOR get_type() {};
    virtual bool is_operator() {return false;};
    virtual bool is_variable() {return false;};
    virtual string get_name() {};
    virtual int get_value() const {};
    virtual int get_value(Lexem *left, Lexem *right) {};
    virtual int get_priority() {};
    virtual void set_value(int value) {};
    virtual void print() {}; 
};

class Number : public Lexem {
    int value;
public:
    Number();
    Number(int value);
    bool is_operator() {return false;};
    int get_value() const;
    void print();
};

class Operators : public Lexem {
    OPERATOR opertype;
public:
    Operators();
    Operators(int idx);
    Operators(vector <Lexem *> vec);
    OPERATOR get_type();
    bool is_operator() {return true;};
    bool is_variable() {return false;};
    int get_priority();
    int get_value(Lexem *left, Lexem *right);
    void print();
};

class Variable : public Lexem {
    string name;
    int value;
    public:
        Variable();
        Variable(string name);
        bool is_operator() {return false;};
        bool is_variable() {return true;};
        void print();
        int get_value() const;
        void set_value(int value);
        string get_name();
        bool inlabel_table();
};


class Goto : public Operators {
    int row;
public:
    bool is_variable() {return false;};
    bool is_operator() {return true;};
    enum { UNDEFINED = - INT32_MAX };
    Goto(OPERATOR optype): Operators(optype) { row = UNDEFINED; }
    Goto(int op = 0): Operators(op) {};
    void set_row(int row) { Goto :: row = row; }
    int get_row(){ return row; }
    void print() { cout << " [ < row " << row << ">" << OPERTEXT[(int)get_type()] << " ] " << endl; }
};
class Array: public Lexem {
    string array_name;
    int el_number;
    int value;
public:
    Array(Lexem *left, Lexem *right);
    void set_value(int number);
};

#endif