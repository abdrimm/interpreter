#ifndef LEXICAL_H
#define LEXICAL_H
#include "lexem.h"
vector <Lexem *> parseLexem(const string &codeline);
Lexem *get_number(string codeline, int &pos);
Lexem *scan_variable(string codeline, int &pos);
bool empty_char(char empty);
Lexem *get_operator(string codeline, int &pos);
void init_labels(vector <Lexem *> &infix, int row);
void init_jumps(vector <vector <Lexem *>> &infix_lines);
#endif