#include "Instruction.h"

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
