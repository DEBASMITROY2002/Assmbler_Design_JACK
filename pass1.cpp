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
using namespace std;

struct SymbolTableEntry
{
  string symbol;
  int address;
};

// manages symbol table
class SymbolTable
{
private:
  const int max_length = 20;
  const int max_entry = 1024;

  map<string, SymbolTableEntry> table;

public:
  SymbolTable()
  {
    table["sp"] = {"SP", 0};
    table["LCL"] = {"LCL", 1};
    table["ARG"] = {"ARG", 2};
    table["THIS"] = {"THIS", 3};
    table["THAT"] = {"THAT", 4};
    table["R0"] = {"R0", 0};
    table["R1"] = {"R1", 1};
    table["R2"] = {"R2", 2};
    table["R3"] = {"R3", 3};
    table["R4"] = {"R4", 4};
    table["R5"] = {"R5", 5};
    table["R6"] = {"R6", 6};
    table["R7"] = {"R7", 7};
    table["R8"] = {"R8", 8};
    table["R9"] = {"R9", 9};
    table["R10"] = {"R10", 10};
    table["R11"] = {"R11", 11};
    table["R12"] = {"R12", 12};
    table["R13"] = {"R13", 13};
    table["R14"] = {"R14", 14};
    table["R15"] = {"R15", 15};
  }

  /**
  * Returns the address of a specific symbol
  * For data-variable symbol, address is of Data Memory
  * Otherwise(i.e->labels) address of Instruction memory
  */
  int getAddress(string symbol)
  {
    if (table.find(symbol) != table.end())
      return table[symbol].address;
    return -1;
  }

  /** 
   * Adds a new Entry
   * If already there(i.e-> label with no address set),
   *  Label's address is Updated
   */
  void addEntry(SymbolTableEntry symbolTableEntry)
  {
    string symbol = symbolTableEntry.symbol;

    if (table.find(symbol) != table.end() && table[symbol].address == -1)
      table[symbol].address = symbolTableEntry.address;

    if (table.find(symbol) == table.end())
      table[symbolTableEntry.symbol] = symbolTableEntry;
  }

  void printSymbolTable(){
    printf("Symbol\t|\tValue\n______________________\n");
    for(auto x:table)
      cout<<x.second.symbol<<"\t\t"<<x.second.address<<endl;
    printf("\t\t\t~end\n\n");
  }
};

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
        cout<<inter_file_name<<endl;
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



int main(int argc, char const *argv[]){
    Pass1 pass1(argv[1]);

    pass1.performStage1();
    pass1.performStage2();

    pass1.printConvertedLines();
    pass1.displaySymbolTable();
}