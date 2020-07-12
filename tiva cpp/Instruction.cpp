#include "Instruction.h"

const char* Instruction::G00 = "G00";
const char* Instruction::G01 = "G01";
const char* Instruction::M00 = "M00";
const char* Instruction::M01 = "M01";
const char* Instruction::M02 = "M02";
const char* Instruction::M03 = "M03";
const char* Instruction::T1 = "T1";
const char* Instruction::T2 = "T2";


bool Instruction::exists(char *instruction) {

    switch (instruction[0]){
        case 'G':
            if (instruction[1] == '0') {
                switch (instruction[2]){
                    case '0':
                    case '1':
                        return true;
                    default:
                        return false;
                }
            }
            return false;

        case 'M':
            if (instruction[1] == '0') {
                switch (instruction[2]){
                    case '0':
                    case '1':
                    case '2':
                        return true;
                    default:
                        return false;
                }
            }
            return false;

        case 'T':
            switch(instruction[1]){
                case '0':
                case '1':
                    return true;
                default:
                    return false;
            }
            return false;
        default:
            return false;
    }
}

instruction Instruction::splitInstruction(char *instruction, char *delimiters){

    char * splitInstructions[10];
    char *token;

    splitInstructions[0] = strtok(instruction, delimiters);

    int instructionIndex = 1;
    while (token != NULL){

      token = strtok(NULL, delimiters);
      if (token != '\0'){
        splitInstructions[instructionIndex] = token;
        instructionIndex++;
      }
    }

    struct instruction _instruction = {
        instructionIndex,
        splitInstructions
    };

    return _instruction;
}
