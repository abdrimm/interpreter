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
        infix_lines.push_back(parse_lexem(codeline));
        getline(cin, codeline);
    }
    for(int i = 0; i < infix_lines.size(); ++i) {
        for(int j = 0; j < infix_lines[i].size(); ++j) {
            infix_lines[i][j]->print();
            cout << " ";
        }
    }
    for(int row = 0; row < (int)infix_lines.size(); ++row) {
        init_labels(infix_lines[row], row);
    }
    init_jumps(infix_lines);
    for(int i = 0; i < (int)infix_lines.size(); ++i) {
        postfix_lines.push_back(build_postfix(infix_lines[i]));
    }
    int row = 0;
    while(0 <= row && row < (int)postfix_lines.size()) {
        row = evaluate_postfix(postfix_lines, row);
    }
    map <string, int> ::iterator it = var_table.begin();
    for (int i = 0; it != var_table.end(); it++, i++) {
        cout << i << ") Key " << it->first << ", value " << it->second << endl;
    }
    map <string, int *> :: iterator itt = array_table.begin();
    for(int i = 0; itt != array_table.end(); itt++, i++) {    
        int j = 0;
        cout << i << ") Key " << itt->first << ", value: [";
        for (j = 0; j < sizeof(itt->second) / sizeof(int) - 1; ++j)
            cout << itt->second[j] << ", ";
        cout << itt->second[j] << "]";
    }
    var_table.clear();
    array_table.clear();
    return 0;
}


