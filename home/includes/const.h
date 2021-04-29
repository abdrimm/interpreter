#ifndef CONST_H
#define CONST_H
#include <string>

using namespace std;

enum OPERATOR {
    COMMA,
    RETURN,
    FUNCTION, ENDFUNCTION,
    LSQRBRACKET, RSQRBRACKET, SIZE,
    IF, THEN,
    ELSE, ENDIF,
    WHILE, ENDWHILE,
    GOTO, COLON,
    LBRACKET, RBRACKET,
    ASSIGN,
    OR,
    AND,
    BITOR,
    XOR,
    BITAND,
    EQ, NEQ,
    LEQ, LT,
    GEQ, GT,
    SHL, SHR,
    PLUS, MINUS,
    MULTIPLY, DIV, MOD
};
extern string OPERTEXT[];
extern int PRIORITY[];
extern int OPER_LEN;
#endif