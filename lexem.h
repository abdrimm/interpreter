#ifndef LEXEM_H
#define LEXEM_H
#include <string>
#include <vector>
#include <iostream>
using namespace std;
class Lexem;
class Number;
class Operators;

enum OPERATOR {
    PLUS , MINUS ,
    MULTIPLY ,
    LBRACKET , RBRACKET
};
class Lexem {
    public :
        Lexem();
        virtual OPERATOR getType() {};
        virtual bool isOperator() {return false;};
        virtual int getValue() const {};
        virtual int getValue(const Number &left, const Number &right) {};
        virtual int getPriority() {};
};
class Number : public Lexem {
    int value;
    public :
        Number();
        Number(int value);
        virtual int getValue() const ;
};
class Operators : public Lexem {
    OPERATOR opertype;
    public :
        Operators();
        Operators(char ch);
        virtual bool isOperator() {return true;};
        Operators(vector <Lexem *> vec);
        virtual OPERATOR getType();
        virtual int getPriority();
        virtual int getValue(const Number &left, const Number &right);
};

int PRIORITY [] = {
     0 , 0 ,
     1 , 
     2 , 2
};
char arrayOfOperators[] = {
    '+', '-',
    '*',
    '(', ')'
};
char arrayOfBrackets[] = {'(', ')'};
Number::Number() {
    value = 0;
}
Lexem::Lexem() {
    
}
Number::Number(int value) {
    this->value = value;
}
int Number::getValue() const {
    return value;
}
Operators::Operators() {}

OPERATOR Operators::getType() {
    return opertype;
}
Operators::Operators(char ch) {
    int operatorNumber = 0;
    for(int i = 0; i < sizeof(arrayOfOperators); ++i) {
        if(ch == arrayOfOperators[i]) {
            opertype = (OPERATOR)i;    
        }
    }  
}
int Operators::getPriority() {
    int priority = 0;
    for(int i = 0; i < sizeof(arrayOfOperators); ++i) {
        if(getType() == i) {
            priority = PRIORITY[i]; 
        }
    }
    return priority;
}
int Operators::getValue(const Number &left, const Number &right) {  
    if(getType() == PLUS) {
        return left.getValue() + right.getValue();
    }
    if(getType() == MINUS) {
        return left.getValue() - right.getValue();
    }
    if(getType() == MULTIPLY) {
        return left.getValue() * right.getValue();
    }
}

#endif