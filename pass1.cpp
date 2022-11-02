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
};

// takes the .asm file , performs clipping and mapping and creates .inter file
class Pass1: public Clipper,public Mapper
{
    private:
        const char* FILE_NAME;
        vector<string> lines;
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
    }

    void performStage1(){
        lines = fileToStrings();
        lines = removeWhiteSpacesAndDeliminators();
        lines = removeCommentsAndEmptyLine();
    }

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
            line.erase(remove(line.begin(), line.end(), ' '), line.end());
            line.erase(remove(line.begin(), line.end(), '\n'), line.end());
        }
    return lines;
    }

    vector<string> removeCommentsAndEmptyLine(){
        vector<string>pureLines;
        for(auto &line:lines){
            int pos = line.find("//");
            string sub = line.substr(0,pos); 
            if(sub.length()>1)
                pureLines.push_back(sub);
        } 
        return pureLines;
    }

    void printConvertedLines(){
        for(auto line:lines)
            cout<<line<<endl;
    }

};

int main(int argc, char const *argv[]){

    Pass1 pass1("./Max.asm");
    pass1.performStage1();
    pass1.printConvertedLines();
}