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

using namespace std;


int main(int argc, char const *argv[]){
    Pass1 pass1(argv[1]);

    pass1.performStage1();
    pass1.performStage2();

    pass1.printConvertedLines();
    pass1.displaySymbolTable();
}