#include "Instruction.h"

Instruction::Instruction(char * _instruction) {}

static const char *G00 = "G00";
static const char *G01 = "G01";
static const char *M00 = "M00";
static const char *M01 = "M01";
static const char *M02 = "M02";
static const char *T1 = "T1";
static const char *T2 = "T2";

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
