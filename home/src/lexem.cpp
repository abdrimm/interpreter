#include "lexem.h"
#include "const.h"

map <string, int> var_table;
map <string, int> lable_table;
map <string, int* > array_table;
map <string, function> fun_table;
stack <int> program_stack;
Lexem::Lexem() {  
}

Number::Number(int value) { this->value = value; }
int Number::get_value() const { return value; }
void Number::print() { cout << value << " "; }
Operators::Operators() {}
OPERATOR Operators::get_type() { return opertype; }
Operators::Operators(int idx) { opertype = (OPERATOR)idx; }
void Operators::print() { cout << OPERTEXT[(int)get_type()]; }

int Operators::get_priority() {
    int priority = 0;
    for(int i = 0; i < OPER_LEN; ++i) {
        if(get_type() == i) {
            priority = PRIORITY[i]; 
        }
    }
    return priority;
}

int Operators::get_value(Lexem *left, Lexem *right) {  
    if(opertype == PLUS) {
        return left->get_value() + right->get_value();
    }
    if(opertype == MINUS) {
        return left->get_value() - right->get_value();
    }
    if(opertype == MULTIPLY) {
        return left->get_value() * right->get_value();
    }
    if(opertype == DIV) {
        return left->get_value() / right->get_value();
    }
    if(opertype == MOD) {
        return left->get_value() % right->get_value();
    }
    if(opertype == AND) {
        return left->get_value() & right->get_value();
    }
    if(opertype == OR) {
        return left->get_value() || right->get_value();
    }
    if(opertype == BITOR) {
        return left->get_value() | right->get_value();
    }
    if(opertype == XOR) {
        return left->get_value() ^ right->get_value();
    }
    if(opertype == BITAND) {
        return left->get_value() & right->get_value();
    }
    if(opertype == EQ) {
        return left->get_value() == right->get_value();
    }
    if(opertype == NEQ) {
        return left->get_value() != right->get_value();
    }
    if(opertype == LEQ) {
        return left->get_value() <= right->get_value();
    }
    if(opertype == LT) {
        return left->get_value() < right->get_value();
    }
    if(opertype == GEQ) {
       return left->get_value() >= right->get_value();
    }
    if(opertype == GT) {
        return left->get_value() > right->get_value();
    }
    if(opertype == ASSIGN) {
        int val = right->get_value();
        left->set_value(val);
        return val;
    }
}

string Variable::get_name() { return this->name; }
Variable::Variable() { name = nullptr; }
void Variable::set_value(int value) { var_table[name] = value; }
int Variable::get_value() const { return var_table[name]; }
void Variable::print() { cout << name; }
bool Variable::inlabel_table() {  
    if(lable_table.find(this->get_name()) == lable_table.end()) {
        return false;
    } else {
        return true;
    }
}

Variable::Variable(string name) {
    this->name = name;
}

Array::Array(Lexem *left, Lexem *right) {
    array_name = left->get_name();
    element_name = right->get_value();
}

int Array::get_value() const {
    return array_table[array_name][element_name];
}

void Array::set_value(int number) {
    array_table[array_name][element_name] = number;
}