
#include "DLS.h"


DLS::DLS(){

}

void DLS::ReadSerial(){
    char inputString[10] = "";
    char c = Helpers::ReadChar();
    Helpers::WriteChar(c);
    
    if (c == '\n'){
        strcpy(inputString, "");
        //inputString = ;
        return;
    }
    strcat(inputString, &c);
}

class DLS {
private:
    char CI[4] = "";
    String inputString = "";
    char inputChar[30] = "";

    Queue *queue;
    Instruction instruction;

    const int directionPin = 4;
    const int stepPin = 5;

    // MAX LENGTH - 6600
    const int StepsPerRev = 6600;

public:

    DLS::DLS (Queue *_queue) {
        queue = _queue;
    }

    void DLS::eventLoop(){
        Serial.println("in");
        for (;;) {
            String currentInstruction = queue->peek();
            if (currentInstruction == "G0") {
                Serial.println(currentInstruction);
                spinMotor();
            }
        }
    }

    
    void spinMotor() {
        for (int x = 0; x < StepsPerRev; x++) {
            digitalWrite(stepPin, HIGH);
            
            delayMicroseconds(800);
            digitalWrite(stepPin, LOW);
            
            delayMicroseconds(800);

        }
    }
    
    void DLS::readSerial(){
      
        while (Serial.available() > 0 ) {

            char inChar = (char)Serial.read();
            
            if (inChar == '\n') {
              
              queue->enqueue(inputString);
              Serial.print(queue->peek());
              Serial.println(queue->size());
            //   while(1);
              
              inputString = "";
              break;
            }
            
            inputString += inChar;
        }
    }
};