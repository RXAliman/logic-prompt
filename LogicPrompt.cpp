#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

//Class to operate logic prompt
class LogicPrompt {
    //Structure for booleans
    typedef struct {
        string boolName;
        bool boolEquiv;
    } BOOL_;
    //List of booleans with their corresponding values
    BOOL_ bools[10] {
        {"T",true},{"~F",true},{"true",true},{"TRUE",true},{"!F",true},
        {"F",false},{"~T",false},{"false",false},{"FALSE",false},{"!T",false},
    };
    //Declaration of user-defined datatype for storage of operation functions
    typedef string (LogicPrompt::*opFunc)(bool, bool);
    //List of operations by index
    string _and_(bool lvalue, bool rvalue) { return (lvalue && rvalue ? "T" : "F"); }
    string _or_(bool lvalue, bool rvalue) { return (lvalue || rvalue ? "T" : "F"); }
    string _con_(bool lvalue, bool rvalue) { return (lvalue && !rvalue ? "F" : "T"); }
    string _bicon_(bool lvalue, bool rvalue) { return (lvalue && rvalue ? "T" : !lvalue && !rvalue ? "T" : "F"); }
        //string _nor_(bool lvalue, bool rvalue) { return (_or_(lvalue,rvalue) == "T" ? "F" : "T"); }
        //string _nand_(bool lvalue, bool rvalue) { return (_and_(lvalue,rvalue) == "T" ? "F" : "T"); }
        //string _xor_(bool lvalue, bool rvalue) { return (_bicon_(lvalue,rvalue) == "T" ? "F" : "T"); }
    //Array of operations
    opFunc operations[4] {
        &LogicPrompt::_and_,
        &LogicPrompt::_or_,
        &LogicPrompt::_con_,
        &LogicPrompt::_bicon_,
    };
    //Structure for operators
    typedef struct {
        string opName;
        string opSym;
        int opIndex;
    } OPS_;
    //List of operators with their corresponding index of operation
    OPS_ ops[6] {
        {"CONJUNCTION","^",0},
        {"CONJUNCTION","&",0},
        {"DISJUNCTION","v",1},
        {"DISJUNCTION","|",1},
        {"CONDITIONAL","=>",2},
        {"BICONDITIONAL","<=>",3},
    };
    string line; //Stores prompt input
    bool isRunning; //Running state
    //Returns lowercased string
    string toLower(string str) {
        for (auto &i : str)
            switch (int(i)) {
            case int('A') ... int('Z'):
                i = char(int(i) + 32);
            }
        return str;
    }
    //Displays help
    void help() {
        cout << " :: How to use Logic Prompt?\n";
        cout << "  - Enter a valid logical statement (without parentheses)\n";
        cout << "  - These are the valid logical elements:\n";
        for (auto i : bools)
            cout << "      " << setw(7) << right << '\''+i.boolName+'\'' << " = " << (i.boolEquiv ? "TRUE" : "FALSE") << endl;
        cout << "  - These are the valid logical operators:\n";
        for (auto j : ops)
            cout << "      " << setw(7) << right << '\''+j.opSym+'\'' << " = " << j.opName << endl;
        cout << "  - Separate each characters with spaces\n";
        cout << "   ex. T v F ^ T ^ F\n";
    }
    //Displays prompt
    void prompt() {
        cout << "> ";
        getline(cin,line);
    }
    //Reads and process input, then displays the result
    void read_and_execute() {
        if (line == "");
        else if (toLower(line) == "exit") isRunning = false;
        else if (toLower(line) == "help" || line == "?") help();
        else cout << " = " << evaluate(line) << endl;
    }
    //checks the syntax of input if correct
    bool checkSyntax(string *str) {
        stringstream sstr(*str);
        string lvalue;
        while (sstr >> lvalue) {
            string op;
            bool flag = false;
            sstr >> op;
            for (auto i : bools)
            if (lvalue == i.boolName) {
                if (op == "") return true;
                else {
                    for (auto j : ops)
                        if (op == j.opSym) {
                            flag = true;
                            break;
                        }
                    if (flag) break;
                    return false;
                }
            }
            if (flag) continue;
            return false;
        }
        return false;
    }
    //evaluates the input logical statement
    string evaluate(string str) {
        if (checkSyntax(&str)) {
            RESCAN:
            for (auto a : ops) {
                stringstream s1str(str);
                stringstream s2str(str);
                string left, mid, right;
                string str_t;
                while (s1str >> str_t) {
                    s1str >> str_t;
                    if (str_t == a.opSym) {
                        string lvalue, op, rvalue;
                        s2str >> lvalue >> op >> rvalue;
                        for (auto b : bools)
                        if (lvalue == b.boolName)
                            for (auto c : bools)
                            if (rvalue == c.boolName) {
                                mid = (this->*operations[a.opIndex])(b.boolEquiv,c.boolEquiv);
                                getline(s2str,right);
                                cout <<"   "<<left<<'('<<lvalue<<' '<<op<<' '<<rvalue<<')'<<right<< endl;
                                str = left + mid + right;
                                cout <<"   "<<str<< endl;
                                goto RESCAN;
                            }
                    }
                    else {
                        for (int a=0; a<2; a++) {
                            s2str >> str_t;
                            left += str_t+' ';
                        }
                    }
                }
            }
            for (auto i : bools)
                if (str == i.boolName) {
                    str = (i.boolEquiv ? "TRUE" : "FALSE");
                    break;
                }
            return str;
        }
        else return "Invalid Input";
    }
public:
    //Constructor
    LogicPrompt() {
        isRunning = true;
        cout << string(50,'-') << endl;
        cout << " LOGIC PROMPT\n <c> Rovic Aliman 2022\n";
        cout << string(50,'-') << "\n\n";
        cout << "Type 'EXIT' to close this program\n";
    }
    //Getter
    bool isOpen() { return isRunning; }
    //Engine
    void run() {
        prompt();
        read_and_execute();
    }
    //Destructor
    ~LogicPrompt() { isRunning = false; }
};

//Program entry point
int main() {
    LogicPrompt lp;
    while (lp.isOpen())
        lp.run();
    return 0;
}
