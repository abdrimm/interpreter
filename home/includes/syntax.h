#ifndef SYNTAX_H
#define SYNTAX_H
#include "lexem.h"
void process_brackets(vector <Lexem *> &stackOfOperators, vector <Lexem *> &postfix);
void join_goto_label(Variable * lexemvar, vector <Lexem *> stack);
void process_lexem(vector <Lexem *> &stackof_operators, vector <Lexem *> &postfix_, Lexem* lexem, int i);
vector <Lexem *> build_postfix(vector <Lexem *> &infix);
#endif