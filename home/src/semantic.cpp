#include "semantic.h"

void right_left_lexems(stack <Lexem*> &evaluation_stack, Lexem* &left, Lexem* &right) {
    right = evaluation_stack.top();
    evaluation_stack.pop();
    left = evaluation_stack.top();
    evaluation_stack.pop();
}

int evaluate_postfix(vector <vector <Lexem *>> postfix_lines, int row) {
    int result = 0;
    stack <Lexem*> evaluation_stack;
    vector<Lexem *> postfix = postfix_lines[row];
    Lexem *left, *right;
    for(int i = 0; i < postfix.size(); ++i) {
        if(postfix[i] == nullptr) {
            continue;
        } else if(postfix[i]->is_operator()) {
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
                right_left_lexems(evaluation_stack, right, left);
                array_table[left->get_name()] = new int[right->get_value()];
            } else if(lexemop->get_type() == LSQRBRACKET) {
                right_left_lexems(evaluation_stack, right, left);
                evaluation_stack.push(new Array(left, right));
            } else if(postfix[i]->get_type() == RETURN) {
                cout << "is return" << endl;
                if(!evaluation_stack.empty()) {
                    program_stack.push(evaluation_stack.top()->get_value());
                    evaluation_stack.pop();
                }
                return -1;
            } else if(postfix[i]->get_type() == FUNCTION) {
                cout << "is function" << endl;
                while(!program_stack.empty()) {
                    evaluation_stack.top()->set_value(program_stack.top());
                    evaluation_stack.pop();
                    program_stack.pop();
                }
                continue;
            } else {               
                if(evaluation_stack.size() >= 2) {
                    right_left_lexems(evaluation_stack, right, left);
                    result = postfix[i]->get_value(left, right);
                    evaluation_stack.push(new Number(result));
                }
            }
        } else {
            if(fun_table.find(postfix[i]->get_name()) != fun_table.end()) {
                Lexem *func_name = postfix[i];
                for(int arg = 0; arg < fun_table[func_name->get_name()].arg_num; arg++) {
                    program_stack.push(evaluation_stack.top()->get_value());
                    evaluation_stack.pop();
                }
                map <string, int> old_variables = var_table;
                map <string, int*> old_arrays = array_table;
                var_table.clear();
                array_table.clear();
                int tmp_row = fun_table[func_name->get_name()].row;
                while(tmp_row >= 0) {
                    tmp_row = evaluate_postfix(postfix_lines, tmp_row);
                }
                var_table.clear();
                array_table.clear();
                var_table = old_variables;
                array_table = old_arrays;
                if(!program_stack.empty()) {
                    evaluation_stack.push(new Number(program_stack.top()));
                    program_stack.pop();
                }
                continue;
            }
                evaluation_stack.push(postfix[i]);               
            }
        }
    return row + 1;