#include "Instruction.h"
#include "Helpers.h"

const char* Instruction::G00 = "G00";
const char* Instruction::G01 = "G01";
const char* Instruction::M00 = "M00";
const char* Instruction::M01 = "M01";
const char* Instruction::M02 = "M02";
const char* Instruction::M03 = "M03";
const char* Instruction::M04 = "M04";
const char* Instruction::T1 = "T1";
const char* Instruction::T2 = "T2";
const char* Instruction::T3 = "T3";
const char* Instruction::S1 = "S1";
const char* Instruction::S2 = "S2";
const char* Instruction::S3 = "S3";
const char* Instruction::S4 = "S4";


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




instruction Instruction::splitInstruction(char *instruction, char *delimiters) {

    char *parameters[10];
    char *token;

    struct instruction newInstruction;

    token = strtok(instruction, delimiters);
    newInstruction.instruction = token;
    int instructionIndex = 0;
    
    while (token != NULL){
      token = strtok(NULL, delimiters);
      if (token != '\0'){
        if (token[0] == 'X' || token[0] == 'Y' || token[0] == 'Z') token = &token[1];
        parameters[instructionIndex] = token; 
        instructionIndex++;
      }
    }
    newInstruction.parameterCount = instructionIndex;
    newInstruction.parameters = parameters;

    return newInstruction;
}
