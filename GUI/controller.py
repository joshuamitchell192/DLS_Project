from view import View
from serialConnection import SerialConnection
from PyQt5.QtWidgets import QApplication


class Controller:
    def __init__(self, serialConnection, Instructions):
        self.serialConnection = serialConnection
        self.Instructions = Instructions

    def handleCalibrate(self):
        self.serialConnection.sendInstruction(self.Instructions.CALIBRATE)



    def handleScanBetween(self, P1, P2, sampleDuration, stepSize):
        self.serialConnection.sendInstruction(self.Instructions.TWOPOS_SCAN)
        self.serialConnection.sendValue(P1)
        self.serialConnection.sendValue(P2)
        self.serialConnection.sendValue(sampleDuration)
        if (stepSize == "Full"):
            self.serialConnection.sendValue(1)
        if (stepSize == "Half"):
            self.serialConnection.sendValue(2)
        if (stepSize == "Quarter"):
            self.serialConnection.sendValue(4)
        while(1):
            QApplication.processEvents()

    def handleStop(self):
        self.serialConnection.sendInstruction(self.Instructions.STOP)


    def handleGoToPoint(self, P1):
        self.serialConnection.sendInstruction(self.Instructions.ONEPOS_SCAN)
        self.serialConnection.sendValue(P1)

    def handleScanAtPoint(self, P1, averageInterval):
        self.serialConnection.sendInstruction(self.Instructions.ONEPOS_SCAN)
        self.serialConnection.sendValue(P1)
        self.serialConnection.sendValue(averageInterval)
