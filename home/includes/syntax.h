#ifndef SYNTAX_H
#define SYNTAX_H
#include "lexem.h"
void join_goto_labels(Variable * lexemvar, vector <Lexem *> stack);
void if_operator(vector <Lexem *> &stackOfOperators, vector <Lexem *> &postfix_, vector <Lexem *> &infix, int i);
vector <Lexem *> buildPostfix(vector <Lexem *> &infix);
#endif