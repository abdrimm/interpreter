#include <string>
#include <vector>
#include <iostream>
#include "lexem.h"
using namespace std;

vector <Lexem *> parseLexem(string codeline);
vector <Lexem *> buildPoliz(vector <Lexem *> infix );
int evaluatePoliz(vector <Lexem *> poliz );
int evaluatePostfix(vector <Lexem *> postfix);
vector <Lexem *> buildPostfix(vector <Lexem *> infix); 
bool isOperator(Lexem *object);

vector <Lexem *> parseLexem(string codeline) {
    int number = 0;
    Lexem *lex;
    bool numberFlag = false;
    vector <Lexem *> parsed;
    for(int i = 0; i < codeline.size(); ++i) {
        if(codeline[i] >= '0' && codeline[i] <= '9') {
            number = number * 10 + codeline[i] - '0';
            numberFlag = true;              
        } else {
            if(numberFlag) {
                lex = new Number(number);
                parsed.push_back(lex);    
            }
            number = 0;
            numberFlag = false;
            lex = new Operators(codeline[i]);
            parsed.push_back(lex);
        }
    }
    if(numberFlag) {
        lex = new Number(number);
        parsed.push_back(lex);
    }  
    cout << "parsedsize: " << parsed.size() << endl;
    return  parsed;
}
int evaluatePostfix(vector <Lexem *> postfix) {
    Lexem *result; 
    int i = 0;
    while(postfix.size() != 1) {
        if(postfix[i]->isOperator() == true) {
            result = new Number(postfix[i]->getValue(Number(postfix[i - 2]->getValue()),
            Number(postfix[i - 1]->getValue())));
            delete postfix[i];
            delete postfix[i - 1];
            delete postfix[i - 2];
            postfix.erase(postfix.begin() + i);
            postfix.erase(postfix.begin() + (i - 1));
            postfix.erase(postfix.begin() + (i - 2));
            i -= 2;
            postfix.insert(postfix.begin() + i, result);
        } else {
            i++;
        }
    }
    cout << postfix[0]->getValue() << endl;
    return postfix[0]->getValue();
}

vector <Lexem *> buildPostfix(vector <Lexem *> infix) {
    vector <Lexem *> postfix_;
    vector <Lexem *> stackOfOperators;
    int n = 0;
    int infix_size = 0;
    int priority = 0;
    infix_size = infix.size();
    cout << infix_size << endl;
    for(int i = 0; i < infix.size(); ++i) {
        if(infix[i]->isOperator() == true) { 
            stackOfOperators.push_back(infix[i]);
            
            priority = stackOfOperators.back()->getPriority();

        } else {
            postfix_.push_back(infix[i]);
        }
        if(infix[i]->getType() == RBRACKET) {
            stackOfOperators.pop_back();
            while(stackOfOperators[stackOfOperators.size() - 1]->getType() != LBRACKET) {    
                postfix_.push_back(stackOfOperators.back()); 
                stackOfOperators.pop_back();                
            }   
            stackOfOperators.pop_back();
        }
    }
    while(stackOfOperators.empty() != true) {
        postfix_.push_back(stackOfOperators.back());
        stackOfOperators.pop_back();
    }
    return postfix_;
}

 int main () {
    string codeline;
    vector <Lexem *> infix;
    vector <Lexem *> postfix;
    int value = 0;
    cin >> codeline;
        infix = parseLexem(codeline);
        postfix = buildPostfix(infix);
        value = evaluatePostfix(postfix);
        cout << value << endl ;

    return 0;
}