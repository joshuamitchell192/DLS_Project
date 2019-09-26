from view import View
from serialConnection import SerialConnection


class Controller:
    def __init__(self, serialConnection, Instructions):
        self.serialConnection = serialConnection
        self.Instructions = Instructions

    def handleCalibrate(self):
        self.serialConnection.sendInstruction(self.Instructions.CALIBRATE)
