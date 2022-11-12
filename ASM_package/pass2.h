#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

class Pass2
{
private:
    map<string, string> map_compbits;
    map<string, string> map_destbits;
    map<string, string> map_jumpbits;

    vector<string> words;

    vector<string> lines;
    SymbolTable symbolTable;

public:
    Pass2(pair<vector<string>, SymbolTable> intermReult)
    {
        map_compbits["0"] = "101010";
        map_compbits["1"] = "111111";
        map_compbits["-1"] = "111010";
        map_compbits["D"] = "001100";
        map_compbits["A"] = "110000";
        map_compbits["!D"] = "001101";
        map_compbits["!A"] = "110001";
        map_compbits["-D"] = "111111";
        map_compbits["-A"] = "101010";
        map_compbits["D+1"] = "011111";
        map_compbits["A+1"] = "110111";
        map_compbits["D-1"] = "001110";
        map_compbits["A-1"] = "110010";
        map_compbits["D+A"] = "000010";
        map_compbits["D-A"] = "010011";
        map_compbits["A-D"] = "000111";
        map_compbits["D&A"] = "000000";
        map_compbits["D|A"] = "010101";

        map_destbits["null"] = "000";
        map_destbits["M"] = "001";
        map_destbits["D"] = "010";
        map_destbits["MD"] = "011";
        map_destbits["A"] = "100";
        map_destbits["AM"] = "101";
        map_destbits["AD"] = "110";
        map_destbits["AMD"] = "111";

        map_jumpbits["null"] = "000";
        map_jumpbits["JGT"] = "001";
        map_jumpbits["JEQ"] = "010";
        map_jumpbits["JGE"] = "011";
        map_jumpbits["JLT"] = "100";
        map_jumpbits["JNE"] = "101";
        map_jumpbits["JLE"] = "110";
        map_jumpbits["JMP"] = "111";

        lines = intermReult.first;
        symbolTable = intermReult.second;
    }

    ~Pass2()
    {
        string hack_file_name = "obj";
        hack_file_name += ".hack";
        FILE *hack_file = fopen(hack_file_name.c_str(), "w");
        for (auto w : words)
            fprintf(hack_file, "%s\n", (w.c_str()));
        fclose(hack_file);
        lines.clear();

        cout << "\nHack file saved as " << hack_file_name << endl;
    }

    string decimalToBinary(int n)
    {
        string s;
        for (int i = 14; i >= 0; i--)
        {
            int k = n >> i;
            if (k & 1)
                s.push_back('1');
            else
                s.push_back('0');
        }
        return s;
    }

    void convertTowords()
    {
        for (auto line : lines)
        {
            string str;

            if (line[0] == '(')
            {
                //IGNORE
            }
            else if (line[0] == '@')
            {
                str += "0";

                string s = line.substr(1), val_15bit;

                if (isdigit(s[0]))
                    val_15bit = decimalToBinary(stoi(s.c_str()));
                else
                    val_15bit = decimalToBinary(symbolTable.getAddress(s));

                str += val_15bit;
            }
            else
            {
                str += "111";

                string dest;
                string comp;
                string jump;
                string aval;

                if (line[1] == '=')
                {
                    dest = line.substr(0, 1);
                    comp = line.substr(2);
                    jump = "null";

                    aval = (comp.find("M") != string::npos) ? "1" : "0";
                    for (auto &x : comp)
                        if (x == 'M')
                            x = 'A';
                }
                else
                {
                    /**
                         * HERE IMON THINKS ANOTHER CHECK 
                         * IS TO BE DONE.
                         * LIKE line[2]=='=' 
                         * LIKE line[3]=='=' 
                         * FOR CASES LIKE MD=SOMETHING
                         * OR AMD = SKDHFK
                         * 
                         * 
                         * 
                         * 
                         */

                    dest = "null";
                    comp = line.substr(0, 1);
                    jump = line.substr(2);

                    aval = (comp.find("M") != string::npos) ? "1" : "0";
                    for (auto &x : comp)
                        if (x == 'M')
                            x = 'A';
                }

                str = str + aval + map_compbits[comp] + map_destbits[dest] + map_jumpbits[jump];
            }
            words.push_back(str);
        }
    }

    void print_16bitWords()
    {
        cout << "\n\n.... 16 bit Binary Word\n";
        for (auto word : words)
            cout << word << endl;
        cout << "\t\t\t~end";
    }
};
