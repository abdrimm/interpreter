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

struct function {
    int arg_num;
    int row;
};

extern map <string, int> var_table;
extern map <string, int> lable_table;
extern map <string, int* > array_table;
extern map <string, function> fun_table;
extern stack <int> program_stack;

class Lexem {
public:
    Lexem();
    virtual OPERATOR get_type() {return RETURN;};
    virtual bool is_operator() {return false;};
    virtual bool is_variable() {return false;};
    virtual string get_name() {return "";};
    virtual int get_value() const {return 0;};
    virtual int get_value(Lexem* left, Lexem* right) {return 0;};
    virtual int get_priority() {return 0;};
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
public:
    Variable();
    Variable(string name);
    bool is_operator() {return false;};
    bool is_variable() {return true;};
    int get_priority();
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
    void print() { cout << " [ < row " << row << ">" << OPERTEXT[(int)get_type()] << " ] "; }
};

class Array: public Lexem {
    string array_name;
    int element_number;
public:
    Array(Lexem *left, Lexem *right);
    void set_value(int number);
    int get_value() const;
};

#endif