/*******************************************************************
Title : Claims
Name :  G.Srichetan Reddy
Roll no : 2301CS54

Declaration of Authorship
This is a part of assignment of CS2102_PROJECT
at the Department of Computer Science and Engineering , IIT PATNA.
********************************************************************/
#include <bits/stdc++.h>
using namespace std;


typedef struct{
    string name;
    int address;
    bool set;
    int set_value;
}symbol;


vector<symbol> symtab;
map<string,int> instructions;
unordered_set<string> labels;
// Hexadecimal digits
string hexdigits= "0123456789abcdef";
void initializing_inst(){
    instructions["ldc"]=0;
    instructions["adc"]=1;
    instructions["ldl"]=2;
    instructions["stl"]=3;
    instructions["ldnl"]=4;
    instructions["stnl"]=5;
    instructions["add"]=6;
    instructions["sub"]=7;
    instructions["shl"]=8;
    instructions["shr"]=9;
    instructions["adj"]=10;
    instructions["a2sp"]=11;
    instructions["sp2a"]=12;
    instructions["call"]=13;
    instructions["return"]=14;
    instructions["brz"]=15;
    instructions["brlz"]=16;
    instructions["br"]=17;
    instructions["HALT"]=18;
    instructions["SET"]=19;
    instructions["data"]=20;
}

string inttohex(int num) {
    // Special case for zero
    if (num == 0) {
        return "00";
    }

    // Use unsigned int to handle two's complement for negative numbers
    uint32_t unsigned_num = static_cast<uint32_t>(num);

    string hexstr = "";

    // Convert the absolute value to hexadecimal (works for both positive and negative)
    while (unsigned_num > 0) {
        int remainder = unsigned_num % 16;
        hexstr = hexdigits[remainder] + hexstr;
        unsigned_num /= 16;
    }

    return hexstr;
}


string validinst(string inst) {
    // Trim leading and trailing whitespace (spaces, tabs, newlines, etc.)
    size_t first = inst.find_first_not_of(" \t\n");  // Including all whitespace characters
    size_t last = inst.find_last_not_of(" \t\n");   // Including all whitespace characters
    return (first == string::npos) ? "" : inst.substr(first, last - first + 1);
}

int converttonum(string value) {
    if (value.empty()) {
        return 0;
    }
    // hexadecimal (0x prefix)
    if (value.find("0x") == 0 || value.find("0X") == 0) {
        int result = 0;
        int i = (value[0] == '0' && (value[1] == 'x' || value[1] == 'X')) ? 2 : 0;
        for (; i < value.length(); ++i) {
            result *= 16;
            char digit = value[i];
            if ('0' <= digit && digit <= '9') {
                result += digit - '0';
            } else if ('a' <= digit && digit <= 'f') {
                result += digit - 'a' + 10;
            } else if ('A' <= digit && digit <= 'F') {
                result += digit - 'A' + 10;
            } else {
                throw invalid_argument("Invalid character in hexadecimal string");
            }
        }
        return result;
    }
    //octal (0 prefix)
    else if (value.find("0") == 0 && value.length() > 1) {
        int result = 0;
        int i = 1; // Skip the leading zero
        for (; i < value.length(); ++i) {
            result *= 8;
            char digit = value[i];
            if ('0' <= digit && digit <= '7') {
                result += digit - '0';
            } else {
                throw std::invalid_argument("Invalid character in octal string");
            }
        }
        return result;
    }
    //negative numbers
    else if (value.find("-") == 0) {
        int result = 0;
        for (int i = 1; i < value.length(); ++i) {
            result = result * 10 + (value[i] - '0');
        }
        return -result;
    }
    //positive numbers
    else if (value.find("+") == 0) {
        int result = 0;
        for (int i = 1; i < value.length(); ++i) {
            result = result * 10 + (value[i] - '0');
        }
        return result;
    }
    //decimal number
    else {
        int result = 0;
        for (int i = 0; i < value.length(); ++i) {
            result = result * 10 + (value[i] - '0');
        }
        return result;
    }
}

bool checkname(string label){
    if(labels.count(label)) return true;
    else return false;
}


bool checknum(string value) {
    // Check for signed numbers
    bool isSigned = (value[0] == '+' || value[0] == '-');
    if (isSigned) {
        value = value.substr(1);  // Remove sign for further checks
    }

    // Check for hexadecimal (starts with 0x or 0X)
    bool isHex = (value.size() > 2 && (value[0] == '0') && (value[1] == 'x' || value[1] == 'X'));
    if (isHex) {
        value = value.substr(2);  // Remove the '0x' or '0X' prefix
        if (value.empty()) {
            return false;  // A valid hex number must have digits after "0x"
        }
        // Check if all characters in the string are valid hex digits
        for (char c : value) {
            if (!isxdigit(c)) {
                return false;  // Invalid character for a hexadecimal number
            }
        }
    } else {
        // For decimal numbers, check if all characters are digits
        if (value.empty()) {
            return false;  // An empty string is not a valid number
        }
        for (char c : value) {
            if (!isdigit(c)) {
                return false;  // Invalid character for a decimal number
            }
        }
    }
    
    return true;
}


bool checklabel(const string& label) {
    if (label.empty()) return false;
    if (!isalpha(label[0])) return false;
    for (size_t i = 1; i < label.size(); ++i) {
        if (!isalnum(label[i])) return false;
    }
    return true;
}

string writelog(string inst,int* pc,int linecnt){
    string log="";
    inst = validinst(inst);
    if(!(inst.find(":")!=string::npos)){
        //No label in the line 
        int spacepos=inst.find(" ",0);
        if(spacepos!=string::npos && spacepos+1<inst.size()){
            string value=inst.substr(spacepos+1,inst.size());
            value=validinst(value);
        }
    }
    else{
        //There is a label in the line
        int colpos=inst.find(":",0);
        string label=validinst(inst.substr(0,colpos));
        if(checkname(label)){
            log+="Error in"+to_string(linecnt)+": Label name already exists\n";
        }
        if(!checklabel(label)){
            log+="Error in"+to_string(linecnt)+": Incorrect Label name(not valid)\n";
        }
        if(colpos==inst.size()-1){
            symtab.push_back({label,*pc,0,-1});
            *pc=*pc-1;
            labels.insert(label);
        }else{
            string instv=validinst(inst.substr(colpos+1));
            int spacepos=instv.find(" ",0);
            string operation,value;
            if(spacepos!=string::npos){
                operation=instv.substr(0,spacepos);
                value=instv.substr(spacepos+1,instv.size());
            }else{
                operation=validinst(instv);
                value="";
            }
            operation=validinst(operation);
            value=validinst(value);
            if(operation == "SET") {
                if(checknum(value)) {
                    symtab.push_back({label, *pc, 1, converttonum(value)});
                } else {
                    log += "Error: In line " + to_string(linecnt) + " for "+operation+"\n";
                }
            }else{
                symtab.push_back({label,*pc,0,-1});
            }
            labels.insert(label);
        }
    }
    return log;
}

void pass_one(ofstream& logf, string given) {
    initializing_inst();  // Initialize instruction set
    string line;
    int pc = 0, linecnt = 1;
    ifstream input(given);  // Open the input file
    if (input.fail()) {
        cerr << "Error: Unable to open the input file: " << given << endl;
        return;
    }

    // Read and process each line in the file
    while (getline(input, line)) {
        size_t pos=line.find(';');
        if(pos!=string::npos){
            line=line.substr(0,pos);
        }
        string inst = validinst(line);  // Process the line
        if (!(inst.empty())) {
            // Process instruction and log the result
            logf << writelog(inst, &pc, linecnt);
            pc++;
        }
        linecnt++;
    }

    input.close();  // Close the input file
}

bool nooperand_required(string operation){
    if(operation=="add"){
        return true;
    }
    if(operation=="sub"){
        return true;
    }
    if(operation=="shl"){
        return true;
    }
    if(operation=="shr"){
        return true;
    }
    if(operation=="a2sp"){
        return true;
    }
    if(operation=="sp2a"){
        return true;
    }
    if(operation=="return"){
        return true;
    }
    if(operation=="HALT"){
        return true;
    }
    return false;
}

bool branching_instruction(string operation){
    if(operation=="call"){
        return true;
    }
    if(operation=="brz"){
        return true;
    }
    if(operation=="brlz"){
        return true;
    }
    if(operation=="br"){
        return true;
    }
    return false;
}

pair<pair<string,string>,string> instructiontocode(string inst,int *pc,int linecnt){
    string encoding,more_errors,machinecode;
    string tt=inttohex(*pc);
    while(tt.size()<8) tt="0"+tt;
    encoding=tt+" ";
    string temp; // For temporary instruction encoding
    if (inst.find(':') != string::npos){
        int colpos=inst.find(":");
        if(colpos==inst.size()-1){
            encoding += "           "; // Maintain alignment in list file
            *pc=*pc- 1;

        }else{
            string instv=inst.substr(colpos+1);
            instv=validinst(instv);
            auto  s= instructiontocode(instv,pc, linecnt);
            temp+=s.first.first;
            more_errors+=s.first.second;
            machinecode+=s.second;
            encoding += temp.substr(9,temp.length());
        }
        encoding+=inst+"\n";
    }
    else{
        inst=validinst(inst);
        int spacepos=inst.find(" ");
        string operation,value;
        if (spacepos!= string::npos) {
            operation = inst.substr(0, spacepos);
            value= inst.substr(spacepos + 1);
        } else {
            operation =inst; // No value present
        }
        operation = validinst(operation);
        value=validinst(value);
        if (instructions.find(operation)==instructions.end()) {    
            //Checks if operation is in (mot).If not found, logs an error indicating an undefined mnemonic.
            more_errors+= "ERROR: Undefined mnemonic " + operation + "at line " + to_string(linecnt) + "\n";
        }else if (nooperand_required(operation)) {
            string op= inttohex(instructions[operation]) ;
            while(op.size()<2) op="0"+op;
            int flag=0;
            
            if (!value.empty()) { // Check if an unexpected operand is present like HALT val
                operation = validinst(operation);
                flag=1;
                more_errors+= "ERROR: Operand not expected for" + operation + " at line "+to_string(linecnt) + "\n";
            }
            if(flag==0){
                encoding +="000000"+op+ " "; 
                machinecode+= "000000"+op+ " ";
            }
        }else if (checknum(value)) {
            int number=converttonum(value);
            string hex=inttohex(number);
            
            if (operation == "SET" || operation=="data") {    //set and data need only the operand value directly, so no opcode is appended
                while (hex.size()<8)
                {
                    hex="0"+hex;
                }
                encoding += hex+ " ";
                machinecode += hex+ " ";
            } else {
                string op=inttohex(instructions[operation]);
                while(op.size()<2){
                    op="0"+op;
                }
                // For other operations, assume the operand is an address requiring 6 hex digits
                if(hex.length() > 6){
                    encoding += hex.substr(hex.length() - 6) +op+ " "; 
                    machinecode += hex.substr(hex.length() - 6) +op+ " "; 
                }
                else{
                    while(hex.size()<6) hex="0"+hex;
                    // If hex_operand is less than 6 digits, pad with leading zeros
                    encoding +=hex+op+ " "; 
                    machinecode +=hex+op+ " "; 
                }
            }
        }
        else {
            bool found = false; // To check if the variable is defined
            string hex;

            for (const auto& sym : symtab) { // Loop through the symbol table
                if (sym.name ==value) { // If the operand matches a symbol
                    found = true; // Mark as found

                    //If is SET
                    if (sym.set) {
                        hex= inttohex(sym.set_value);
                    }
                    else if (branching_instruction(operation)) {
                        // For branching operations,store the offset
                        hex= inttohex(sym.address -*pc-1);
                    } 
                    else {
                        // Normal case, just get the address
                        hex= inttohex(sym.address);
                    }

                    // Append the hex string to encoding
                    if(hex.length() >=6){
                        string op=inttohex(instructions[operation]);
                        while(op.size()<2){
                            op="0"+op;
                        }
                        encoding += hex.substr(hex.length() - 6) +op+ " "; 
                        machinecode += hex.substr(hex.length() - 6) +op+ " "; 
                    }
                    else{
                        string op=inttohex(instructions[operation]);
                        while(op.size()<2){
                            op="0"+op;
                        }
                        while(hex.size()<6) hex="0"+hex;
                        // If hex_string is less than 6 digits, pad with leading zeros
                        encoding += hex+op+ " "; 
                        machinecode += hex+op+ " "; 
                    }
                    break; // Break after finding the symbol
                }
            }

            // If the variable was not found, log an error
            if (!found) {
                more_errors+= "ERROR: Unknown Symbol " +value+" as operand is at line " + to_string(linecnt) + "\n";
            }
        }
        encoding +=inst+ "\n"; // Add the original instruction to encoding
    }

    return {{encoding,more_errors},machinecode};
}

void pass_two(ofstream &logf,string given,ofstream &listf,ofstream &objectf){

    string line;
    int pc = 0, linecnt = 1;
    ifstream input(given);  // Open the input file
    if (input.fail()) {
        cerr << "Error: Unable to open the input file: " << given << endl;
        return;
    }

    // Read and process each line in the file
    while (getline(input, line)) {
        size_t pos=line.find(';');
        if(pos!=string::npos){
            line=line.substr(0,pos);
        }
        string inst;
        inst=validinst(line);
        if(!inst.empty()){
            string encoding,more_errors,machinecode;
            auto s=instructiontocode(inst,&pc, linecnt);
            encoding=s.first.first;
            more_errors=s.first.second;
            machinecode=s.second;
            pc++;
            listf<<encoding;
            if(!(machinecode.empty())){
                uint32_t temp=stoul("0x"+validinst(machinecode),nullptr,16);
                objectf.write(reinterpret_cast<char*>(&temp),sizeof(temp));
            }
            logf<<more_errors;
        }
        linecnt++;
    }
    input.close();
}


int main() {
    cout<<"Enter the name of file to assemble: ";
    string given;
    cin>>given;
    int p=given.find(".",0);
    if (p == string::npos) {
        cerr << "Error: File name does not have an extension." << endl;
        return 1;
    }
    string list=given.substr(0,p)+".L";
    string log=given.substr(0,p)+".log";
    string object=given.substr(0,p)+".o";
    
    ofstream listf(list);
    ofstream logf(log);
    ofstream objectf(object, ios::binary);
    if (!listf || !logf || !objectf) {
        cerr << "Failed to open the onr of the file for writing.Try Again!" << endl;
        return 1;
    }
    pass_one(logf,given);
    pass_two(logf,given,listf,objectf);
    listf.close();
    logf.close();
    objectf.close();

    return 0;
}
