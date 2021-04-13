#include <string>
#include <vector>
#include <iostream>
#include "lexem.h"
using namespace std;
#include <stack>

vector <Lexem *> parseLexem(const string &line);
int evaluatePostfix(vector <Lexem *> postfix);
vector <Lexem *> buildPostfix(vector <Lexem *> infix); 
bool isOperator(Lexem *object);

Lexem *get_operator(string line, int &position) {
    string subline;
    for(int oper = 20; oper >= 0; --oper) {
        subline = line.substr(position, OPERTEXT[oper].size());
        if(OPERTEXT[oper] == subline) {
            position += OPERTEXT[oper].size();
            return new Operators(oper);
        }
    }
    return nullptr;
}

Lexem *get_number(string line, int &position) {
    int number = 0;
    int old_position = position;
    while(line[position] >= '0' && line[position] <= '9') {
        number = number * 10 + line[position] - '0';
        position++;
    }
    if (position == old_position) {
        return nullptr;
    }
    return new Number(number);
}

Lexem *scan_var(string line, int &position) {
    string name;
    int old_position = position;
    Variable *pointer;
    while(isalpha(line[position]) || isdigit(line[position]) || line[position] == '_') 
    {
        name.push_back(line[position]);
        position++;
    }
    if(position == old_position) {
        return nullptr;
    }
    auto search = varTable.find(name);
    if (search == varTable.end()) {
        pointer = new Variable(name);
        varTable[name] = pointer;
        return pointer;
    } else {
        return search->second;
    }
}

vector <Lexem *> parseLexem(const string &line) {
    Lexem *lexem;
    vector <Lexem *> parsed;
    for(int position = 0; position < line.size();) {
        lexem = get_operator(line, position);
        if(lexem != nullptr) {    
            parsed.push_back(lexem);
            continue;
        }
        lexem = get_number(line, position);
        if(lexem != nullptr) {
            parsed.push_back(lexem);
            continue;
        }
        lexem = scan_var(line, position);
        if(lexem != nullptr) {
            parsed.push_back(lexem);
            continue;
        }
    }
    return parsed;
}

int evaluatePostfix(vector <Lexem *> postfix) {
    int value = 0;
    Lexem *left_number, *right_number;
    stack <Lexem *> evaluationStack;
    for(int i = 0; i < postfix.size(); ++i) {
        if(!postfix[i]->isOperator()) {
            evaluationStack.push(postfix[i]);
        } else {
            right_number = evaluationStack.top();
            evaluationStack.pop();
            left_number = evaluationStack.top();
            evaluationStack.pop();
            value = postfix[i]->getValue(left_number, right_number);
            evaluationStack.push(new Number(value));
        }
    }
    return evaluationStack.top()->getValue();
}

vector <Lexem *> buildPostfix(vector <Lexem *> infix) {
    vector <Lexem *> postfix_temp;
    vector <Lexem *> stackOfOperators;
    int importance = 0;
    for(int i = 0; i < infix.size(); ++i) {
        if(infix[i]->isOperator()) {
            if(infix[i]->getType() == RBRACKET) {
                while(!stackOfOperators.empty() && (stackOfOperators.back()->getType() != LBRACKET)) {    
                    postfix_temp.push_back(stackOfOperators.back()); 
                    stackOfOperators.pop_back();                
                }   
                stackOfOperators.pop_back();
            } else {
                importance = infix[i]->getPriority();
                while((importance != -1) && !stackOfOperators.empty() && (stackOfOperators.back()->getPriority() >= importance)) {
                    postfix_temp.push_back(stackOfOperators.back());
                    stackOfOperators.pop_back();
                }
                stackOfOperators.push_back(infix[i]);
            }
        } else {
            postfix_temp.push_back(infix[i]);
        }
    }
    while(!stackOfOperators.empty()) {
        postfix_temp.push_back(stackOfOperators.back());
        stackOfOperators.pop_back();
    }
    return postfix_temp;
}

int main () {
    string line;
    vector <Lexem *> infix;
    vector <Lexem *> postfix;
    int answer = 0;
    cin >> line;
    while(line != "exit") {
        infix = parseLexem(line);
        postfix = buildPostfix(infix);
        answer = evaluatePostfix(postfix);
        cout << answer << endl ;
        cin >> line;
    }
    return 0;
}