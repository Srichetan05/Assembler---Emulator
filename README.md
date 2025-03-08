# Assembler---Emulator
This project involves implementing an assembler for an extended SIMPLE instruction set, writing and testing programs in SIMPLE assembly, and developing an emulator for the SIMPLE machine.

## Features

### Assembler
- Takes an input filename of format filename.asm
- Reads assembly language from the .asm file
- Assigns label values and instruction opcodes
- Diagnoses common assembly errors (e.g., unknown instruction, missing label, duplicate label)
- Produces object file with machine code starting at address zero with extension .o
- Produces a log file containing all the errors
- Generates a listing file showing instructions, Machine codes and Program Counter.

### Assembler Testing
- Sample programs are provided for validation
- Additional programs were tested

### Emulator
- The emulator is interactive and can run all instructions according to user's input.
- Assumes that all the data and set instructions are segregated from other parts of  assembly code (after the HALT) in .asm file
- Runs For all the Instuctions mentioned in the pdf file

## Instructions

### Assembler Usage
Compile the assembler using:
```sh
g++ assembler.cpp -o assembler
```
Run the assembler:
```sh
./assembler 
```
-Asks for input of assembler file after executing the .exe file

### Emulator Usage
Compile the emulator using:
```sh
g++ emulater.cpp -o emulater
```
Run the emulator:
```sh
./emulator
```
-Asks for input of machine code(assembler generated) file after executing the .exe file

## File Structure
- `asm.cpp` - Source code for the assembler
- `emu.cpp` - Source code for the emulator
- `test1.asm`, `test2.asm`, ... - Sample assembly test files
- `test1.o`, `test2.o`, ... - Generated object files
- `test1.l`, `test2.l`, ... - Assembler listing files
- `test1.log`, `test2.log`, ... - Assembler Log files
