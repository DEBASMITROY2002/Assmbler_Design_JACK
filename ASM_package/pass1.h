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
#include "./symboltable.h"

using namespace std;


// removes WhiteSpaces, Deliminators, Comments and EmptyLine
class Clipper
{
    private:
    public:
        virtual vector<string> removeWhiteSpacesAndDeliminators()=0;
        virtual vector<string> removeCommentsAndEmptyLine()=0;
};

// registers the symbols in symbol table
class Mapper
{
    private:
    public:
    virtual void solve_Labels()=0;
    virtual bool checkIfReserved(string s)=0;
    virtual void solve_starts_with_at()=0;
};

// takes the .asm file , performs clipping and mapping and creates .inter file
class Pass1: public Clipper,public Mapper
{
    private:
        const char* FILE_NAME;
        vector<string> lines;
        SymbolTable symbolTable;

        const int base_instruction_address = 0;
        const int base_data_address = 16;

    public:

    Pass1(const char* _FILE_NAME){
        FILE_NAME = _FILE_NAME;
    }
   
    ~Pass1(){
        string inter_file_name = FILE_NAME;
        inter_file_name+=".inter";
        FILE* inter_file = fopen(inter_file_name.c_str(), "w");
        for(auto x:lines)
            fprintf(inter_file,"%s\n",(x.c_str()));
        fclose(inter_file);
        lines.clear();

        cout<<"\nIntermediate file saved as "<<inter_file_name<<endl;
    }

    void printConvertedLines(){
        cout<<"... Intermediate State";
        for(auto line:lines)
            cout<<line<<endl;
        cout<<"\t\t\t~end";
    }
   
    void performStage1(){
        lines = fileToStrings();
        lines = removeWhiteSpacesAndDeliminators();
        lines = removeCommentsAndEmptyLine();
    }

    void performStage2(){
        solve_Labels();
        solve_starts_with_at();
    }
    
    void displaySymbolTable(){
        symbolTable.printSymbolTable();
    }

    pair<vector<string>, SymbolTable> getIntermresults(){
        return {lines, symbolTable};
    }
   
    // overriding satege 1 functions
    vector<string> fileToStrings(){
        FILE *file = fopen(FILE_NAME,"r");

        char inp_buffer[1024];

        while(fgets(inp_buffer, sizeof(inp_buffer), file)){
            string s = inp_buffer;
            lines.push_back(s);
        }

        fclose(file);
        return lines;
    }

    vector<string> removeWhiteSpacesAndDeliminators(){
        for(auto &line:lines){
            for(auto x:line){
                if(x<33 || x>126){
                    line.erase(remove(line.begin(), line.end(), (char)x), line.end());
                }
                line.erase(remove(line.begin(), line.end(), (char)(10)), line.end());
            }
        }
        return lines;
    }
    
    vector<string> removeCommentsAndEmptyLine(){
        vector<string>pureLines;
        for(auto &line:lines){
            int pos = line.find("//");
            string sub = (pos!=string::npos)?line.substr(0,pos):line; 
            if(sub.length()>1){
                pureLines.push_back(sub);
            }
        } 
        return pureLines;
    }

   // overriding satege 2 functions
    void solve_Labels(){
        int I = base_instruction_address;
        for(auto line:lines){
            if(line[0]=='('){
                string lab_symbol = line.substr(1, line.find(")")-1);
                SymbolTableEntry e{lab_symbol,I}; 
                symbolTable.addEntry(e);
            }
            else
                I++;
        }
    } 

    bool checkIfReserved(string s){
        return symbolTable.getAddress(s)!=-1;
    } 

    void solve_starts_with_at(){
       int D = base_data_address;
       for(auto line:lines){
            if(line[0]=='@' && !checkIfReserved(line.substr(1))){
                string lab_symbol = line.substr(1);
                SymbolTableEntry e{lab_symbol,D}; 
                symbolTable.addEntry(e);
                D++;
            }
        } 
    }
};
