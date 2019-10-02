from view import View
from serialConnection import SerialConnection
from PyQt5.QtWidgets import QApplication
import time

class Controller:

    

    def __init__(self, serialConnection, Instructions):
        self.serialConnection = serialConnection
        self.Instructions = Instructions
        self.samples = []
        self.stop = False

    def handleCalibrate(self):
        #self.handleStop()
        self.serialConnection.sendInstruction(self.Instructions.CALIBRATE)



    def handleScanBetween(self, P1, P2, sampleDuration, stepSize):
        self.stop = False
        self.serialConnection.sendInstruction(self.Instructions.TWOPOS_SCAN)
        self.serialConnection.sendValue(P1)
        self.serialConnection.sendValue(P2)
        self.serialConnection.sendValue(sampleDuration)
        if (stepSize == "Full"):
            self.serialConnection.sendValue(1)
        elif (stepSize == "Half"):
            self.serialConnection.sendValue(2)
        elif (stepSize == "Quarter"):
            self.serialConnection.sendValue(4)
        else:
            self.serialConnection.sendValue(1)

        while(not self.stop):
            if (len(self.samples) != 0):
                print(self.samples[-1])
            currentSample = self.serialConnection.readSample()
            if currentSample == 0xFFFF:
                break
            self.samples.append(currentSample)
            # print(self.samples)
            QApplication.processEvents()

    def handleStop(self):
        self.stop = True 
        self.serialConnection.sendStopInstruction(self.Instructions.STOP)

        


    def handleGoToPoint(self, P1):
        #self.handleStop()
        self.serialConnection.sendInstruction(self.Instructions.GOTO)
        self.serialConnection.sendValue(P1)

    def handleStartSample(self, sampleDuration, averageInterval):
        #self.handleStop()
        self.serialConnection.sendInstruction(self.Instructions.START_SAMPLE)
        self.serialConnection.sendValue(sampleDuration)
        self.serialConnection.sendValue(averageInterval)

    def clearSamples(self):
        self.samples = []

