/*
Authors:
1. Debasmit Roy, JU CSE
2. Anurup Naskar,JU CSE
3. Imon Raj, JU CSE
4. Kushal Das, JU CSE

*/

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "./pass1.h"
#include "./pass2.h"
using namespace std;


void assemble(char const* file_name){
    Pass1 pass1(file_name);

    pass1.performStage1();
    pass1.performStage2();

    pass1.displaySymbolTable();
    pass1.printConvertedLines();

    pair<vector<string>, SymbolTable> intermReult = pass1.getIntermresults();

    Pass2 pass2(intermReult);
    pass2.convertTowords();
    pass2.print_16bitWords();
}

int main(int argc, char const *argv[]){
    assemble(argv[1]);
}