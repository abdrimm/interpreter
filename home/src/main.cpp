#include <string>
#include <vector>
#include <iostream>
#include "lexem.h"
#include "semantic.h"
#include "lexical.h"
#include "syntax.h"
#include "const.h"
#include <stack>

using namespace std;

int main () {
    string codeline;
    vector <vector <Lexem *>> infix_lines, postfix_lines;
    getline(cin, codeline);
    while(codeline != "exit") {
        infix_lines.push_back(parseLexem(codeline));
        getline(cin, codeline);
    }
    for(int row = 0; row < (int)infix_lines.size(); ++row) {
        init_labels(infix_lines[row], row);
    }
    init_jumps(infix_lines);
    for(int i = 0; i < (int)infix_lines.size(); ++i) {
        postfix_lines.push_back(buildPostfix(infix_lines[i]));
    }
    map <string, Variable *> :: iterator it = var_table.begin();
    for(int i = 0; it != var_table.end(); it++, i++) {  
        cout << i << ") Ключ " << it->first << ", значение " << it->second << endl;
    }
    int row = 0;
    while(0 <= row && row < (int)postfix_lines.size()) {
        row = evaluatePostfix(postfix_lines[row], row);
    }
    map <string, int *> :: iterator itt = array_table.begin();
    for(int i = 0; itt != array_table.end(); itt++, i++) {    
        cout << i << ") Ключ " << itt->first << ", значение " << itt->second << endl;
    }
    return 0;
}