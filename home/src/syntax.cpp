#include "syntax.h"

void if_operator(vector <Lexem *> &stackof_operators, vector <Lexem *> &postfix_, vector <Lexem *> &infix, int i) {
    int priority = 0;
    if(infix[i]->get_type() == RBRACKET) {
        bool found = false;
        for (int j = 0; j < stackof_operators.size(); ++j)
            if (stackof_operators[j]->get_type() == LBRACKET)
                found = true;
        if (found)
            while (!stackof_operators.empty() && (stackof_operators.back()->get_type() != LBRACKET)) {
                postfix_.push_back(stackof_operators.back());
                stackof_operators.pop_back();
            }
        else
            postfix_.push_back(stackof_operators.back());
        stackof_operators.pop_back();
    } else if(infix[i]->get_type() == RSQRBRACKET) {
        while(!stackof_operators.empty() && (stackof_operators.back()->get_type() != LSQRBRACKET)) {
            postfix_.push_back(stackof_operators.back());
            stackof_operators.pop_back();  
        }
        postfix_.push_back(stackof_operators.back());
        stackof_operators.pop_back();  
    } else {
        priority = infix[i]->get_priority();
        while((priority != -1 ) && !stackof_operators.empty() && (stackof_operators.back()->get_priority() >= priority)) {
            postfix_.push_back(stackof_operators.back());
            stackof_operators.pop_back();
        }
        stackof_operators.push_back(infix[i]);
    } 
}

vector <Lexem *> build_postfix(vector <Lexem *> &infix) {
    vector <Lexem *> postfix_;
    vector <Lexem *> stackof_operators;
    for(int i = 0; i < infix.size(); ++i) {       
        if(infix[i] == nullptr) {
            continue;
        }
        if(infix[i]->is_operator()) {
            if_operator(stackof_operators, postfix_, infix, i);
            Operators *lexemoper = (Operators*)infix[i];
            if(lexemoper->get_type() == ENDIF) {
                continue ;
            }
        } else if(infix[i]->is_variable()) {
            if(fun_table.find(infix[i]->get_name()) != fun_table.end()) {
                string nameof_fun = infix[i]->get_name();
                while(infix[i]->get_type() != RBRACKET) {
                    stackof_operators.push_back(infix[i]);
                    i++;
                }
                while(stackof_operators.back()->get_name() != nameof_fun) {
                    postfix_.push_back(stackof_operators.back());
                    stackof_operators.pop_back();
                }
                postfix_.push_back(stackof_operators.back());
                stackof_operators.pop_back();
                continue;
            } else {
                Variable *lexem_var = (Variable *)infix[i];
                if(lexem_var->inlabel_table()) {
                    join_goto_label(lexem_var, stackof_operators);
                } else {
                    postfix_.push_back(infix[i]);
                }
            }
        } else {
            postfix_.push_back(infix[i]);
        }
    }
    while(!stackof_operators.empty()) {
        postfix_.push_back(stackof_operators.back());
        stackof_operators.pop_back();
    }
    for(int i = 0; i < postfix_.size(); ++i) {
        postfix_[i]->print();
    }
    return postfix_;
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
