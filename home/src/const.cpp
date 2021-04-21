#include "const.h"

string OPERTEXT [] = {
    "[", "]", "size",
    "if", "then",
    "else", "endif",
    "while", "endwhile",
    "goto", ":",
    "(", ")",
    "=",
    "or",
    "and",
    "|",
    "^",
    "&",
    "==", "!=",
    "<=", "<",
    ">=", ">",
    "<<", ">>",
    "+", "-",
    "*", "/", "%"
};
 int PRIORITY [] = {
    -1, -1, -4, 
    -3, -3,
    -3, -3,
    -3, -3,   
    -2, -2,
    -1, -1,
    0,
    1,
    2,
    3,
    4,
    5,
    6, 6,
    7, 7,
    7, 7,
    8, 8,
    9, 9,
    10, 10, 10
};
int OPER_LEN = sizeof(OPERTEXT) / sizeof(string);