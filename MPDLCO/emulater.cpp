/*******************************************************************
Title : Claims
Name :  G.Srichetan Reddy
Roll no : 2301CS54

Declaration of Authorship
This txt file , claims.txt is a part of assignment of CS2102_PROJECT
at the Department of Computer Science and Engineering , IIT PATNA.
********************************************************************/

#include <bits/stdc++.h>
using namespace std;

map<string, int> instructions;
// Hexadecimal digits
string hexdigits = "0123456789abcdef";

// Initialize the instructions map
void initializing_inst()
{
    instructions["ldc"] = 0;
    instructions["adc"] = 1;
    instructions["ldl"] = 2;
    instructions["stl"] = 3;
    instructions["ldnl"] = 4;
    instructions["stnl"] = 5;
    instructions["add"] = 6;
    instructions["sub"] = 7;
    instructions["shl"] = 8;
    instructions["shr"] = 9;
    instructions["adj"] = 10;
    instructions["a2sp"] = 11;
    instructions["sp2a"] = 12;
    instructions["call"] = 13;
    instructions["return"] = 14;
    instructions["brz"] = 15;
    instructions["brlz"] = 16;
    instructions["br"] = 17;
    instructions["HALT"] = 18;
    instructions["SET"] = 19;
    instructions["data"] = 20;
}

// Find instruction by operation code
string findinst(int operation)
{
    for (auto it : instructions)
    {
        if (it.second == operation)
            return it.first;
    }
    return "";  // In case of unknown operation
}

// Convert integer to hexadecimal string
string inttohex(int num)
{
    if (num == 0)
    {
        return "00000000";
    }

    uint32_t unsigned_num = static_cast<uint32_t>(num);
    string hexstr = "";

    while (unsigned_num > 0)
    {
        int remainder = unsigned_num % 16;
        hexstr = hexdigits[remainder] + hexstr;
        unsigned_num /= 16;
    }

    while (hexstr.size() < 8)
        hexstr = "0" + hexstr;

    return hexstr;
}

vector<int> machinecodes;
int memory[100000] = {0}; // Memory space for 100000 locations
int A = 0, B = 0, SP = 0, pc = 0;

void showbefore(int size)
{
    cout << "Memory dump before execution" << endl;
    cout<<"There are "<<size<<" instructions"<<endl;
    for (int i = 0; i < size; i += 4)
    {
        cout << inttohex(i) << " ";
        for (int j = i; j < min(size, i + 4); ++j)
        {
            cout << inttohex(machinecodes[j]) << " ";
        }
        cout << "\n";
    }
    cout<<"Memory is only occupied by machinecodes of size:"<<size<<endl;
}

void showafter()
{
    cout << "Memory dump after execution(showing only first 512)" << endl;
    for (int i = 0; i < 512; i += 4)
    {
        cout << inttohex(i) << " ";
        for (int j = i; j < min(512, i + 4); ++j)
        {
            cout << inttohex(memory[j]) << " ";
        }
        cout << "\n";
    }
}

// Execute and trace the instructions
void for_trace(int size, ofstream &tracef)
{
    int count = 0;
    while (pc < size)
    {
        count++;
        int prevpc = pc;
        int operation, value, inst = memory[pc];

        // Extract the opcode (lowest 8 bits)
        operation = inst & 0xFF;

        // Extract the operand (24 bits)
        value = (inst & 0xFFFFFF00) >> 8;

        // Check if the operand is negative by examining the 23rd bit
        if (value & 0x800000)
        {
            value |= 0xFF000000; // Sign-extend to 32 bits
        }

        tracef << "PC: " << inttohex(pc) << "\tSP: " << inttohex(SP) << "\tA: "
               << inttohex(A) << "\tB: " << inttohex(B) << " " << findinst(operation) << " " << value << endl;
        cout << "PC: " << inttohex(pc) << "\tSP: " << inttohex(SP) << "\tA: "
             << inttohex(A) << "\tB: " << inttohex(B) << " " << findinst(operation) << " " << value << endl;

        // Execute the instruction based on the opcode
        switch (operation)
        {
        case 0: // ldc insruction
            B = A;
            A = value;
            break;
        case 1: // adc instruction
            A += value;
            break;
        case 2: // ldl instruction
            if (SP + value < 0 || SP + value >= 100000)
            {
                cerr << "Memory access out of bounds!" << endl;
                return;
            }
            B = A;
            A = memory[SP + value];
            break;
        case 3: // stl instruction
            if (SP + value < 0 || SP + value >= 100000)
            {
                cerr << "Memory access out of bounds!" << endl;
                return;
            }
            memory[SP + value] = A;
            A = B;
            break;
        case 4: // ldnl instruction
            A = memory[A + value];
            break;
        case 5: // stnl instruction
            memory[A + value] = B;
            break;
        case 6: // add instruction
            A += B;
            break;
        case 7: // sub instruction
            A = B - A;
            break;
        case 8: // shl instruction
            A = B << A;
            break;
        case 9: // shr instruction
            A = B >> A;
            break;
        case 10: // adj instruction
            SP += value;
            break;
        case 11: // a2sp instruction
            SP = A;
            A = B;
            break;
        case 12: // sp2a instruction
            B = A;
            A = SP;
            break;
        case 13: // call instruction
            B = A;
            A = pc;
            pc += value;
            break;
        case 14: // return instruction
            pc = A;
            A = B;
            break;
        case 15: // brz instruction
            if (A == 0)
                pc += value;
            break;
        case 16: // brlz instruction
            if (A < 0)
                pc += value;
            break;
        case 17: // br instruction
            pc += value;
            break;
        case 18: // HALT instruction
            cout << count << " instructions executed" << endl;
            tracef<< count << " instructions executed" << endl;
            return;

        }
        pc++;
        // Check for infinite loop detection
        if (prevpc == pc)
        {
            cout << "Infinite loop detected at PC " << inttohex(pc) << endl;
            tracef<< "Infinite loop detected at PC " << inttohex(pc) << endl;
            break;
        }
    }
    cout << "Total of " << count << " instructions executed" << endl;
    tracef<< "Total of " << count << " instructions executed" << endl;
}

int main()
{
    cout << "Enter the name of machinecode file: ";
    string given;
    cin >> given;

    // Check file extension
    int p = given.find(".", 0);
    if (p == string::npos)
    {
        cerr << "Error: File name does not have an extension." << endl;
        return 1;
    }

    // Open the input file
    ifstream inputf(given, ios::binary);
    if (!inputf)
    {
        cerr << "Error: Unable to open the input file!" << endl;
        return 1;
    }

    // Create trace file
    string trace = given.substr(0, p) + ".trace";
    ofstream tracef(trace);
    if (!tracef)
    {
        cerr << "Error: Unable to create trace file!" << endl;
        return 1;
    }

    unsigned int temp,i=0;
    while (inputf.read((char *)&temp, sizeof(int)))
    {
        machinecodes.push_back(temp);
        memory[i]=temp;
        i++;
    }

    if (inputf.fail() && !inputf.eof())
    {
        cerr << "Error: Failed to read machine code from file!" << endl;
        return 1;
    }

    initializing_inst();

    // Main control loop for user input
    int in = 1;
    while (in > 0 && in < 5)
    {
        cout << "1: for trace\n";
        cout << "2: show memory before execution\n";
        cout << "3: show memory after execution\n";
        cout << "4: show register values\n";
        cout << "Type the number for instruction now: ";
        cin >> in;

        if (!(in > 0 && in < 5))
        {
            cout << "Invalid input, process terminated.\n";
            break;
        }

        if (in == 1)
        {
            for_trace(machinecodes.size(), tracef);
        }
        if (in == 2)
        {
            showbefore(machinecodes.size());
        }
        if (in == 3)
        {
            showafter();
        }
        if (in == 4)
        {
            cout << "A: " << inttohex(A) << " B: " << inttohex(B) << " SP: " << inttohex(SP) << " PC: " << inttohex(pc) << endl;
        }
    }

    return 0;
}
