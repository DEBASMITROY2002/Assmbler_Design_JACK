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
class SymbolTable
{
private:
  const int max_length = 20;
  const int max_entry = 1024;

  map<string, SymbolTableEntry> table;

public:
  SymbolTable()
  {
    // Data Memory
    table["SP"] = {"SP", 0};
    table["LCL"] = {"LCL", 1};
    table["ARG"] = {"ARG", 2};
    table["THIS"] = {"THIS", 3};
    table["THAT"] = {"THAT", 4};
    
    // Instruction Memory
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


int main(int argc, char const *argv[])
{
  SymbolTable symbolTable;
  SymbolTableEntry e1{"LOOP",-1};
  symbolTable.addEntry(e1);
  symbolTable.printSymbolTable();
  SymbolTableEntry e2{"END",98};
  symbolTable.addEntry(e2);
  symbolTable.printSymbolTable();
  SymbolTableEntry e3{"LOOP",10};
  symbolTable.addEntry(e3);
  symbolTable.printSymbolTable();
  
  return 0;
}