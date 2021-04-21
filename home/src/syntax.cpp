#include "syntax.h"

void if_operator(vector <Lexem *> &stackOfOperators, vector <Lexem *> &postfix_, vector <Lexem *> &infix, int i) {
    int priority = 0;
    if(infix[i]->get_type() == RBRACKET) {
        while(!stackOfOperators.empty() && (stackOfOperators.back()->get_type() != LBRACKET)) {    
            postfix_.push_back(stackOfOperators.back()); 
            stackOfOperators.pop_back();                
        }   
        stackOfOperators.pop_back();
    } else if(infix[i]->get_type() == RSQRBRACKET) {
        while(!stackOfOperators.empty() && (stackOfOperators.back()->get_type() != LSQRBRACKET)) {
            postfix_.push_back(stackOfOperators.back());
            stackOfOperators.pop_back();  
        }
        postfix_.push_back(stackOfOperators.back());
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
            if_operator(stackOfOperators, postfix_, infix, i);
            Operators * lexemoper = (Operators *)infix[i];
            if(lexemoper->get_type() == ENDIF) {
                continue ;
            } 
        } else if(infix[i]->is_variable()) {
            Variable *lexem_var = (Variable *)infix[i];
            if(lexem_var->inlabel_table()) {
                join_goto_labels(lexem_var, stackOfOperators);
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

void join_goto_labels(Variable * lexemvar, vector <Lexem *> stack) {
    int row = 0;
    if(stack.empty()) {
        return;
    }  
    if(stack.back()->get_type() == GOTO) {
        Goto * goto_lexem = (Goto *)stack.back();
        row = lable_table[lexemvar->get_name()];
        goto_lexem->set_row(row);
    }
}