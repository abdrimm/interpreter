#include "syntax.h"

void process_lexem(vector <Lexem *> &stackof_operators, vector <Lexem *> &postfix, Lexem *lexem, int i) {
    int priority = 0;
    if(lexem->get_type() == RBRACKET) {
        while (!stackof_operators.empty() && (stackof_operators.back()->get_type() != LBRACKET) 
    && (stackof_operators.back()->get_type() != LSQRBRACKET)) {
        postfix.push_back(stackof_operators.back());
        stackof_operators.pop_back();
    }
        stackof_operators.pop_back();
    } else if (lexem->get_type() == COMMA) {
        while (!stackof_operators.empty() && (stackof_operators.back()->get_type() != LBRACKET) 
    && (stackof_operators.back()->get_type() != LSQRBRACKET)) {
        postfix.push_back(stackof_operators.back());
        stackof_operators.pop_back();
    }
    } 
    else if(lexem->get_type() == RSQRBRACKET) {
        while (!stackof_operators.empty() && (stackof_operators.back()->get_type() != LBRACKET) 
    && (stackof_operators.back()->get_type() != LSQRBRACKET)) {
        postfix.push_back(stackof_operators.back());
        stackof_operators.pop_back();
    }
        postfix.push_back(stackof_operators.back());
        stackof_operators.pop_back();  
    } else {
        priority = lexem->get_priority();
        while((priority != -1 ) && !stackof_operators.empty() 
        && (stackof_operators.back()->get_priority() >= priority)) {
            postfix.push_back(stackof_operators.back());
            stackof_operators.pop_back();
        }
        stackof_operators.push_back(lexem);
    } 
}

vector <Lexem *> build_postfix(vector <Lexem *> &infix) {
    vector <Lexem *> postfix;
    vector <Lexem *> stackof_operators;
    for(int i = 0; i < infix.size(); ++i) {
        infix[i]->print();
    }
    cout << endl;
    for(int i = 0; i < infix.size(); ++i) {      
        if(infix[i] == nullptr) {
            continue;
        }
        if(infix[i]->is_operator()) {
            process_lexem(stackof_operators, postfix, infix[i], i);
            Operators *lexemoper = (Operators*)infix[i];
            if(lexemoper->get_type() == ENDIF) {
                continue ;
            }
        } else if(infix[i]->is_variable()) {
            if(fun_table.find(infix[i]->get_name()) != fun_table.end()) {
                process_lexem(stackof_operators, postfix, infix[i], i);
            } else {
                Variable *lexem_var = (Variable *)infix[i];
                if(lexem_var->inlabel_table()) {
                    join_goto_label(lexem_var, stackof_operators);
                } else {
                    postfix.push_back(infix[i]);
                }
            }
        } else {
            postfix.push_back(infix[i]);
        }
    }
    while(!stackof_operators.empty()) {
        postfix.push_back(stackof_operators.back());
        stackof_operators.pop_back();
    }
    for(int i = 0; i < postfix.size(); ++i) {
        postfix[i]->print();
    }
    return postfix;
}

void join_goto_label(Variable * lexemvar, vector <Lexem *> stack) {
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