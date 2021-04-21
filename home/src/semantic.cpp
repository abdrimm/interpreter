#include "semantic.h"

int evaluatePostfix(vector <Lexem *> postfix, int row) {
    int result = 0;
    stack <Lexem *> evaluation_stack;
    Lexem *left, *right;
    for(int i = 0; i < postfix.size(); ++i) {
        if(postfix[i]->is_operator()) {
            Operators *lexemop = (Operators *)postfix[i];
            if(lexemop->get_type() == GOTO ||
                lexemop->get_type() == ELSE ||
                lexemop->get_type() == ENDWHILE ) {
                Goto *lexemgoto = (Goto *)lexemop;
                return lexemgoto->get_row();
            } else if(lexemop->get_type() == IF ||
                lexemop->get_type() == WHILE ) {
                Goto *lexemgoto = (Goto *)lexemop ;
                int rvalue = evaluation_stack.top()->get_value();
                evaluation_stack.pop();
                if(!rvalue) {
                    return lexemgoto->get_row();
                }
            } else if(lexemop->get_type() == SIZE) {
                right = evaluation_stack.top();
                evaluation_stack.pop();
                left = evaluation_stack.top();
                evaluation_stack.pop();
                array_table[left->get_name()] = new int[right->get_value()];
            } else if(lexemop->get_type() == LSQRBRACKET) {
                right = evaluation_stack.top();
                evaluation_stack.pop();
                left = evaluation_stack.top();
                evaluation_stack.pop();
                evaluation_stack.push(new Array(left, right));
            } 
            else { 
                if(evaluation_stack.size() >= 2) {
                    right = evaluation_stack.top();
                    evaluation_stack.pop();
                    left = evaluation_stack.top();
                    evaluation_stack.pop();
                    result = postfix[i]->get_value(left, right);
                    evaluation_stack.push(new Number(result));
                }
            }
        } else {
                evaluation_stack.push(postfix[i]);               
            }
        }
    return row + 1;
}