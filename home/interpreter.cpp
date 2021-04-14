#include <string>
#include <vector>
#include <iostream>
#include "lexem.h"
#include <stack>
using namespace std;

vector <Lexem *> parseLexem(const string &line);
void join (Variable * lexem_var, vector <Lexem *> stack);
vector <Lexem *> buildPostfix(vector <Lexem *> &infix); 
int evaluatePostfix(vector <Lexem *> postfix);
bool is_operator(Lexem *object);

Lexem *get_operator(string line, int &position) {
    string subline;
    for(int oper = 0; oper < 29; ++oper) {
        subline = line.substr(position, OPERTEXT[oper].size());
        if(OPERTEXT[oper] == subline) {
            position += OPERTEXT[oper].size();
            if(OPERATOR(oper) == GOTO || OPERATOR(oper) == IF || OPERATOR(oper) == ELSE ||
                OPERATOR(oper) == WHILE || OPERATOR(oper) == ENDWHILE) {
                return new Goto(oper); 
            } else {
                return new Operators(oper);
            }
        }
    }
    return nullptr;
}

Lexem *get_number(string line, int &position) {
    int number = 0;
    bool pos_flag = true;
    while(line[position] >= '0' && line[position] <= '9') {
        number = number * 10 + line[position] - '0';
        position++;
        pos_flag = false;
    }
    if(pos_flag) {
        return nullptr;
    }
    return new Number(number);
}

Lexem *scan_var(string line, int &position) {
    string name;
    Variable *pointer;
    bool pos_flag = true;
    while((line[position] >= 'a' && line[position] <= 'z') || (line[position] >= 'A' && line[position] <= 'Z') || 
          (line[position] >= '0' && line[position] <= '9') || line[position] == '_') 
    {
        name.push_back(line[position]);
        position++;
        pos_flag = false;
    }
    if(pos_flag) {
        return nullptr;
    } 
    auto search = var_table.find(name);

    if(search == var_table.end()) {
        pointer = new Variable(name);
        var_table.insert(make_pair(name, pointer));
        return pointer;
    } else {
        return search->second;
    }
}

bool is_empty(char empty) {
    if(empty == ' ' || empty == '\t') {
        return true;
    } else {
        return false;
    }
}

vector <Lexem *> parseLexem(const string &line) {
    Lexem *lexem;
    vector <Lexem *> parsed;
    for(int position = 0; position < line.size();) {
        if(is_empty(line[position])) {
            position++;
        }
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

int evaluatePostfix(vector <Lexem *> postfix, int rank) {
    int result = 0;
    stack <Lexem *> evaluationStack;
    Lexem *left, *right;
    for(int i = 0; i < postfix.size(); ++i) {
        if(postfix[i]->is_operator()) {
            Operators *lexem_op = (Operators *)postfix[i];
            if(lexem_op->get_type() == GOTO ||
                lexem_op->get_type() == ELSE ||
                lexem_op->get_type() == ENDWHILE ) {
                Goto * lexem_goto = (Goto *)lexem_op;
                return lexem_goto->get_rank();
            } else if(lexem_op->get_type() == IF ||
                lexem_op->get_type() == WHILE ) {
                Goto * lexem_goto = (Goto *)lexem_op ;
                int rvalue = evaluationStack.top()->get_value();
                evaluationStack.pop();
                if(!rvalue) {
                    return lexem_goto->get_rank();
                }
            } else { 
                if(evaluationStack.size() >= 2) {
                    right = evaluationStack.top();
                    evaluationStack.pop();
                    left = evaluationStack.top();
                    evaluationStack.pop();
                    result = postfix[i]->get_value(left, right);
                    evaluationStack.push(new Number(result));
                }
            }
        } else {
                evaluationStack.push(postfix[i]);               
            }
        }
    return rank + 1;
}

void if_oper(vector <Lexem *> &stackOfOperators, vector <Lexem *> postfix_, vector <Lexem *> infix, int i) {
    int priority = 0;
    if(infix[i]->get_type() == RBRACKET) {
        while(!stackOfOperators.empty() && (stackOfOperators.back()->get_type() != LBRACKET)) {    
            postfix_.push_back(stackOfOperators.back()); 
            stackOfOperators.pop_back();                
        }   
        stackOfOperators.pop_back();
    } else {
        priority = infix[i]->get_priority();
        while((priority != -1) && !stackOfOperators.empty() && (stackOfOperators.back()->get_priority() >= priority)) {
            postfix_.push_back(stackOfOperators.back());
            stackOfOperators.pop_back();
        }
        stackOfOperators.push_back(infix[i]);
    } 
}

vector <Lexem *> buildPostfix(vector <Lexem *> &infix) {
    vector <Lexem *> postfix_;
    vector <Lexem *> stackOfOperators;
    for(int i = 0; i < infix.size(); ++i) {       
        if(infix[i] == nullptr) {
            continue;
        }
        if(infix[i]->is_operator()) {
            if_oper(stackOfOperators, postfix_, infix, i);
            Operators * lexem_oper = (Operators *)infix[i];
            if (lexem_oper->get_type() == ENDIF) {
                continue ;
            }
        } else if(infix[i]->is_variable()) {
            Variable *lexem_var = (Variable *)infix[i];
            if(lexem_var->in_labeltable()) {
                join(lexem_var, stackOfOperators);
            } else {
                postfix_.push_back(infix[i]);
            }
        } else {
            postfix_.push_back(infix[i]);
        }
    }
    while(!stackOfOperators.empty()) {
        postfix_.push_back(stackOfOperators.back());
        stackOfOperators.pop_back();
    }
  
    cout << "postfix: ";
    for(int i = 0; i < postfix_.size(); ++i) {
        postfix_[i]->print();
    }
    cout << endl;
    return postfix_;
}

void join(Variable * lexem_var, vector <Lexem *> stack) {
    int rank = 0;
    if(stack.empty()) {
        return;
    }  
    if(stack.back()->get_type() == GOTO) {
        Goto * goto_lexem = (Goto *)stack.back();
        rank = lable_table[lexem_var->get_name()];
        goto_lexem->set_rank(rank);
    }
}

void labels(vector <Lexem *> &infix, int rank) {
    for(int i = 1; i < infix.size(); ++i) {
        if(infix[i - 1]->is_variable() && infix[i]->is_operator()) {
            Variable *lexem_var = (Variable *)infix[i - 1];
            Operators *lexem_op = (Operators *)infix[i];
            if(lexem_op->get_type() == COLON) {
                if(lable_table.find(lexem_var->get_name()) == lable_table.end()) {
                    lable_table[lexem_var->get_name()] = rank;    
                       
                } else { 
                    try {
                        if(lable_table.find(lexem_var->get_name()) != lable_table.end()) {
                            thrank 0;
                        }
                    }
                    catch(int error) {
                        cout << "Error: " <<  error << endl;
                    }
                }
                i++;
            }
        }
    }
}

void jumps(vector <vector <Lexem *>> &infix_lines) {
    stack <Goto *> stackIfElse;
    stack <Goto *> stackWhile;
    for(int rank = 0; rank <  (int)infix_lines.size(); ++rank) {
        for(int i = 0; i < (int) infix_lines[rank].size(); ++i) {
            if(infix_lines[rank][i]->is_operator()) {
                Operators* lexem_oper = (Operators *)infix_lines[rank][i];
                if(lexem_oper->get_type() == IF) {
                    stackIfElse.push((Goto *)lexem_oper);
                }
                if(lexem_oper->get_type() == ELSE) {
                    stackIfElse.top()->set_rank(rank + 1);
                    stackIfElse.pop();
                    stackIfElse.push((Goto *)lexem_oper);
                }
                if(lexem_oper->get_type() == ENDIF) {
                    stackIfElse.top()->set_rank(rank + 1);
                    stackIfElse.pop();
                }
                if(lexem_oper->get_type() == WHILE) {
                    Goto * lexem_goto = (Goto *)lexem_oper ;
                    lexem_goto->set_rank(rank);
                    stackWhile.push(lexem_goto);
                }
                if(lexem_oper->get_type() == ENDWHILE) {
                    Goto * lexem_goto = (Goto *)lexem_oper ;
                    lexem_goto->set_rank(stackWhile.top()->get_rank());
                    stackWhile.top()->set_rank(rank + 1);
                    stackWhile.pop();
                }
            }
        }
    }
}

int main () {
    string line;
    vector <vector <Lexem *>> infix_lines, postfix_lines;
    get_line(cin, line);
    while(line != "return") {
        infix_lines.push_back(parseLexem(line));
        get_line(cin, line);
    }
    for(int rank = 0; rank < (int)infix_lines.size(); ++rank) {
        labels(infix_lines[rank], rank);
    }
    jumps(infix_lines);
    for(int i = 0; i < (int)infix_lines.size(); ++i) {
        postfix_lines.push_back(buildPostfix(infix_lines[i]));
    }
    int rank = 0;
    while(0 <= rank && rank < (int)postfix_lines.size()) {
        rank = evaluatePostfix(postfix_lines[rank], rank);
    }
    return 0;
}


