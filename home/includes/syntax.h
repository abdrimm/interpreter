#ifndef SYNTAX_H
#define SYNTAX_H
#include "lexem.h"
void join_goto_label(Variable * lexemvar, vector <Lexem *> stack);
void if_operator(vector <Lexem *> &stackof_operators, vector <Lexem *> &postfix_, vector <Lexem *> &infix, int i);
vector <Lexem *> build_postfix(vector <Lexem *> &infix);
#endif