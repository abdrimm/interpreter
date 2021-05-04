#include "lexical.h"
#include "const.h"
Lexem *get_operator(string codeline, int &pos) {
    string subline;
    for(int op = 0; op < OPER_LEN; ++op) {
        subline = codeline.substr(pos, OPERTEXT[op].size());
        if(OPERTEXT[op] == subline) {
            pos += OPERTEXT[op].size();
            if(OPERATOR(op) == COMMA) {
                return nullptr;
            }
            if(OPERATOR(op) == GOTO || OPERATOR(op) == IF || OPERATOR(op) == ELSE ||
                OPERATOR(op) == WHILE || OPERATOR(op) == ENDWHILE || OPERATOR(op) == FUNCTION) {
                return new Goto(op); 
            } else {
                return new Operators(op);
            }
        }
    }
    return nullptr;
}

Lexem *get_number(string codeline, int &pos) {
    int number = 0;
    bool pos_flag = true;
    while(codeline[pos] >= '0' && codeline[pos] <= '9') {
        number = number * 10 + codeline[pos] - '0';
        pos++;
        pos_flag = false;
    }
     if(pos_flag) {
        return nullptr;
    }
    return new Number(number);
}

Lexem *scan_variable(string codeline, int &pos) {
    string name;
    Variable *ptr;
    bool pos_flag = true;
    while((codeline[pos] >= 'a' && codeline[pos] <= 'z') || (codeline[pos] >= 'A' && codeline[pos] <= 'Z') || 
          (codeline[pos] >= '0' && codeline[pos] <= '9') || codeline[pos] == '_') 
    {
        name.push_back(codeline[pos]);
        pos++;
        pos_flag = false;
    }
    if(pos_flag) {
        return nullptr;
    } 
    auto search = var_table.find(name);
    if(search == var_table.end()) {
        ptr = new Variable(name);
        var_table.insert(make_pair(name, 0));
        return ptr;
    } else {
        return new Variable(search->first);
    }
}

bool isempty_char(char empty) {
    if(empty == ' ' || empty == '\t') {
        return true;
    } else {
        return false;
    }
}

vector <Lexem *> parse_lexem(const string &codeline) {
    Lexem *lexem;
    vector <Lexem *> parsed;
    for(int pos = 0; pos < codeline.size();) {
        if(isempty_char(codeline[pos])) {
            pos++;
        }
        lexem = get_operator(codeline, pos);
        if(lexem != nullptr) {    
            parsed.push_back(lexem);
            continue;
        }
        lexem = get_number(codeline, pos);
        if(lexem != nullptr) {
            parsed.push_back(lexem);
            continue;
        }
        lexem = scan_variable(codeline, pos);
        if(lexem != nullptr) {
            parsed.push_back(lexem);
            continue;
        }
    }
    return parsed;
}

void init_labels(vector <Lexem *> &infix, int row) {
    for(int i = 1; i < infix.size(); ++i) {
        if(infix[i - 1]->is_variable() && infix[i]->is_operator()) {
            Variable *lexemvar = (Variable *)infix[i - 1];
            Operators *lexemop = (Operators *)infix[i];
            if(lexemop->get_type() == COLON) {
                if(lable_table.find(lexemvar->get_name()) == lable_table.end()) {
                    lable_table[lexemvar->get_name()] = row;    
                       
                } else { 
                    try {
                        if(lable_table.find(lexemvar->get_name()) != lable_table.end()) {
                            throw 0;
                        }
                    }
                    catch(int error) {
                        cout << "Error: " <<  error << endl;
                    }
                }
                i++;
            }        
        }
        if(infix[i - 1] -> get_type() == FUNCTION) {
            ((Goto*)infix[i - 1])->set_row(row);
            int counter = 0;
            for(int i = 0 ; i < infix.size(); ++i) {
                if(infix[i]->get_type() == COMMA) {
                        counter++;                   
                }
            }
            fun_table[infix[i]->get_name()].arg_num = counter + 1;
            cout << "arg_num" << fun_table[infix[i]->get_name()].arg_num << endl;
            fun_table[infix[i]->get_name()].row = row;
            for(int i = 0 ; i < infix.size(); ++i) {
                if(fun_table.find(infix[i]->get_name()) != fun_table.end()) {
                    infix.erase(infix.begin() + i); 
                    i--;
                }
                if((infix[i]->get_type() == LBRACKET) || (infix[i]->get_type() == RBRACKET) || (infix[i]->get_type() == COMMA)) {
                    infix.erase(infix.begin() + i);
                    i--;
                }               
            }
            return;
        }
    }
}

void init_jumps(vector <vector <Lexem *>> &infix_lines) {
    stack <Goto *> stack_ifelse;
    stack <Goto *> stack_while;
    for(int row = 0; row <  (int)infix_lines.size(); ++row) {
        for(int i = 0; i < (int) infix_lines[row].size(); ++i) {
            if(infix_lines[row][i]->is_operator()) {
                Operators* lexemoper = (Operators *)infix_lines[row][i];
                if(lexemoper->get_type() == IF) {
                    stack_ifelse.push((Goto *)lexemoper);
                }
                if(lexemoper->get_type() == ELSE) {
                    stack_ifelse.top()->set_row(row + 1);
                    stack_ifelse.pop();
                    stack_ifelse.push((Goto *)lexemoper);
                }
                if(lexemoper->get_type() == ENDIF) {
                    stack_ifelse.top()->set_row(row + 1);
                    stack_ifelse.pop();
                }
                if(lexemoper->get_type() == WHILE) {
                    Goto * lexemgoto = (Goto *)lexemoper ;
                    lexemgoto->set_row(row);
                    stack_while.push(lexemgoto);
                }
                if(lexemoper->get_type() == ENDWHILE) {
                    Goto * lexemgoto = (Goto *)lexemoper ;
                    lexemgoto->set_row(stack_while.top()->get_row());
                    stack_while.top()->set_row(row + 1);
                    stack_while.pop();
                }
            }
        }
    }
}