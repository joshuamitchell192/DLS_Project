from view import View
from serialConnection import SerialConnection


class Controller:
    def __init__(self, serialConnection, Instructions):
        self.serialConnection = serialConnection
        self.Instructions = Instructions

    def handleCalibrate(self):
        self.serialConnection.sendInstruction(self.Instructions.CALIBRATE)

    def handleScanBetween(self, P1, P2, sampleDuration, stepSize):
        self.serialConnection.sendInstruction(self.Instructions.TWOPOS_SCAN)
        self.serialConnection(P1)
        self.serialConnection(P2)
        self.serialConnection(sampleDuration)
        if (stepSize == "Full"):
            self.serialConnection(1)
        if (stepSize == "Half"):
            self.serialConnection(2)
        if (stepSize == "Quarter"):
            self.serialConnection(4)

    def handleStop(self):
        self.serialConnection.sendInstruction(self.Instructions.STOP)
