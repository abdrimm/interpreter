#ifndef LEXEM_H
#define LEXEM_H
#include <string>
#include <vector>
#include <iostream>
#include <map>
using namespace std;
class Lexem;
class Number;
class Operators;
class Variable;

map <string, Variable*> varTable;


enum OPERATOR {
    LBRACKET, RBRACKET,
    ASSIGN,
    OR,
    AND,
    BITOR,
    XOR,
    BITAND,
    EQ, NEQ,
    LEQ, LT,
    GEQ, GT,
    SHL, SHR,
    PLUS, MINUS,
    MULTIPLY, DIV, MOD
};

string OPERTEXT[] = {
    "(", ")",
    "=",
    "or",
    "and",
    "|",
    "^",
    "&",
    "==", "!=",
    "<=", "<",
    ">=", ">",
    "<<", ">>",
    "+", "-",
    "*", "/", "%"
};

int PRIORITY [] = {
    -1 , -1 ,
    0,
    1,
    2,
    3,
    4,
    5,
    6, 6,
    7, 7,
    7, 7,
    8, 8,
    9, 9,
    10, 10, 10
};
class Lexem {
    public :
        Lexem();
        virtual OPERATOR getType() {};
        virtual bool isOperator() {return false;};
        virtual int getValue() const {};
        virtual int getValue(Lexem *left, Lexem *right) {};
        virtual int getPriority() {};
        virtual void setValue(int value) {};
        virtual void print() {}; 
};
Lexem::Lexem() {
    
}
class Number : public Lexem {
    int value;
    public :
        Number();
        Number(int value);
        virtual bool isOperator() {return false;};
        virtual int getValue() const;
        virtual void print();
};

Number::Number(int value) {
    this->value = value;
}

int Number::getValue() const {
    return value;
}

void Number::print() {
    cout << value;
}

class Operators : public Lexem {
    OPERATOR opertype;
    public :
        Operators();
        Operators(int idx);
        virtual bool isOperator() {return true;};
        Operators(vector <Lexem *> vec);
        virtual OPERATOR getType();
        virtual int getPriority();
        virtual int getValue(Lexem *left, Lexem *right);
        virtual void print();
};

Operators::Operators() {}
OPERATOR Operators::getType() {
    return opertype;
}
Operators::Operators(int idx) {
    opertype = (OPERATOR)idx;
}

void Operators::print() {
    cout << OPERTEXT[(int)getType()];
}

int Operators::getPriority() {
    int importance = 0;
    for(int i = 0; i < sizeof(OPERTEXT); ++i) {
        if(getType() == i) {
            importance = PRIORITY[i]; 
        }
    }
    return importance;
}

int Operators::getValue(Lexem *left, Lexem *right) {  
    if(opertype == PLUS) {
        return left->getValue() + right->getValue();
    }
    if(opertype == MINUS) {
        return left->getValue() - right->getValue();
    }
    if(opertype == MULTIPLY) {
        return left->getValue() * right->getValue();
    }
    if(opertype == DIV) {
        return left->getValue() / right->getValue();
    }
    if(opertype == MOD) {
        return left->getValue() % right->getValue();
    }
    if(opertype == AND) {
        return left->getValue() & right->getValue();
    }
    if(opertype == OR) {
        return left->getValue() || right->getValue();
    }
    if(opertype == BITOR) {
        return left->getValue() | right->getValue();
    }
    if(opertype == XOR) {
        return left->getValue() ^ right->getValue();
    }
    if(opertype == BITAND) {
        return left->getValue() & right->getValue();
    }
    if(opertype == EQ) {
        return left->getValue() == right->getValue();
    }
    if(opertype == NEQ) {
        return left->getValue() != right->getValue();
    }
    if(opertype == LEQ) {
        return left->getValue() <= right->getValue();
    }
    if(opertype == LT) {
        return left->getValue() < right->getValue();
    }
    if(opertype == GEQ) {
       return left->getValue() >= right->getValue();
    }
    if(opertype == GT) {
        return left->getValue() > right->getValue();
    }
    if(opertype == ASSIGN) {
        int val = right->getValue();
        left->setValue(val);
        return val;
    }
}

class Variable : public Lexem {
    string name;
    int value;
    public:
        Variable();
        Variable(string name);
        virtual bool isOperator() {return false;};
        virtual void print();
        virtual int getValue() const;
        virtual void setValue(int value);
};

Variable::Variable() {
    name = nullptr;
}

Variable::Variable(string name) {
    value = 0;
    this->name = name;
}

void Variable::setValue(int value) {
    this->value = value;
}

int Variable::getValue() const {
    return value;
}

void Variable::print() {
    cout << name << "[" << value << "]";
}

#endif