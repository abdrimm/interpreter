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
class Goto;
map <string, Variable *> var_table;
map <string, int> label_table;

enum OPERATOR {
    IF, THEN,
    ELSE, ENDIF,
    WHILE, ENDWHILE,
    GOTO, COLON,
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

string OPERTEXT [] = {
    "if", "then",
    "else", "endif",
    "while", "endwhile",
    "goto", ":",
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
    -3, -3,
    -3, -3,
    -3, -3,   
    -2, -2,
    -1, -1,
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

Lexem::Lexem() {  
}

class Number : public Lexem {
    int value;
    public :
        Number();
        Number(int value);
        bool is_operator() {return false;};
        int get_value() const;
        void print();
};

Number::Number(int value) {
    this->value = value;
}
int Number::get_value() const {
    return value;
}
void Number::print() {
    cout << value << " ";
}

class Operators : public Lexem {
    OPERATOR opertype;
    public :
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

Operators::Operators() {}

OPERATOR Operators::get_type() {
    return opertype;
}
Operators::Operators(int idx) {
    opertype = (OPERATOR)idx;
}
void Operators::print() {
    cout << OPERTEXT[(int)get_type()];
}
int Operators::get_priority() {
    int priority = 0;
    for(int i = 0; i < sizeof(OPERTEXT); ++i) {
        if(get_type() == i) {
            priority = PRIORITY[i]; 
        }
    }
    return priority;
}
int Operators::get_value(Lexem *left, Lexem *right) {  
    if(opertype == PLUS) {
        return left->get_value() + right->get_value();
    }
    if(opertype == MINUS) {
        return left->get_value() - right->get_value();
    }
    if(opertype == MULTIPLY) {
        return left->get_value() * right->get_value();
    }
    if(opertype == DIV) {
        return left->get_value() / right->get_value();
    }
    if(opertype == MOD) {
        return left->get_value() % right->get_value();
    }
    if(opertype == AND) {
        return left->get_value() & right->get_value();
    }
    if(opertype == OR) {
        return left->get_value() || right->get_value();
    }
    if(opertype == BITOR) {
        return left->get_value() | right->get_value();
    }
    if(opertype == XOR) {
        return left->get_value() ^ right->get_value();
    }
    if(opertype == BITAND) {
        return left->get_value() & right->get_value();
    }
    if(opertype == EQ) {
        return left->get_value() == right->get_value();
    }
    if(opertype == NEQ) {
        return left->get_value() != right->get_value();
    }
    if(opertype == LEQ) {
        return left->get_value() <= right->get_value();
    }
    if(opertype == LT) {
        return left->get_value() < right->get_value();
    }
    if(opertype == GEQ) {
       return left->get_value() >= right->get_value();
    }
    if(opertype == GT) {
        return left->get_value() > right->get_value();
    }
    if(opertype == ASSIGN) {
        int val = right->get_value();
        left->set_value(val);
        return val;
    }
}

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
        bool in_labeltable();
};

string Variable::get_name() {
    return this->name;
}
Variable::Variable() {
    name = nullptr;
}
Variable::Variable(string name) {
    value = 0;
    this->name = name;
}
void Variable::set_value(int value) {
    this->value = value;
}
int Variable::get_value() const {
    return value;
}
void Variable::print() {
    cout << name;
}
bool Variable::in_labeltable() {  
    if(label_table.find(this->get_name()) == label_table.end()) {
        return false;
    } else {
        return true;
    }
}

class Goto : public Operators {
    int rank;
public:
    bool is_variable() {return false;};
    bool is_operator() {return true;};
    enum { UNDEFINED = - INT32_MAX };
    Goto(OPERATOR optype): Operators(optype) {
        rank = UNDEFINED;
    }
    Goto(int op = 0): Operators(op) {};
    void set_rank(int rank) {
        Goto :: rank = rank;
    }
    int get_rank(){
        return rank;
    }
    void print() {
        cout << " [ < rank " << rank << ">" << OPERTEXT[(int)get_type()] << " ] " << endl;
    }
};


#endif