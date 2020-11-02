from view import View
from serialConnection import SerialConnection
from PyQt5.QtWidgets import QApplication
import time, configparser
import struct
import pycrc.algorithms

class Controller:
    def __init__(self, serialConnection, Instructions):
        self.serialConnection = serialConnection
        self.Instructions = Instructions
        self.samples = []
        self.times = []
        self.positions = []
        self.pause = False
        self.stepsPerMM = 0.018
        self.isSampling = False
        self.crc = pycrc.algorithms.Crc(width=16, poly=0x8005, reflect_in=True, xor_in= 0x0000, reflect_out=True, xor_out = 0x0000)

    def handleCalibrate(self):
        self.isSampling = False
        self.serialConnection.sendInstruction(self.Instructions.Calibrate)

    def handleScanBetween(self, P1, P2, sampleDuration, mmBetweenSamples, stepMode):
        self.isSampling = False
        self.serialConnection.sendInstruction(self.Instructions.Pause)
        self.serialConnection.sendInstruction(self.Instructions.StartProgram)
        self.serialConnection.sendInstruction(self.Instructions.RapidPosition, [P1])
        self.serialConnection.sendInstruction(self.Instructions.SampleDuration, [sampleDuration])
        self.serialConnection.sendInstruction(self.Instructions.TurnOnAdc)
        self.serialConnection.sendInstruction(self.Instructions.mmBetweenSamples, [mmBetweenSamples])
        self.serialConnection.sendInstruction(self.Instructions.StepMode, [stepMode])
        self.serialConnection.sendInstruction(self.Instructions.LinearPosition, [P2])
        self.serialConnection.sendInstruction(self.Instructions.TurnOffAdc)
        self.serialConnection.sendInstruction(self.Instructions.EndProgram)
        self.serialConnection.sendInstruction(self.Instructions.Resume)

    def handlePause(self):

        """ Sets the pause boolean to true so that we cease reading samples.

        """
        
        if (self.pause):
            self.pause = False
            self.serialConnection.sendInstruction(self.Instructions.Resume)
        else:
            self.pause = True
            # Write to tiva to pause sampling on GUI
            self.isSampling = False
            self.serialConnection.sendInstruction(self.Instructions.Pause)

    def handleGoToPoint(self, P1):

        """ sends the Rapid Position instruction to move to position 1

        """
        self.isSampling = False
        self.serialConnection.sendInstruction(self.Instructions.Pause)
        self.serialConnection.sendInstruction(self.Instructions.TurnOffAdc)
        self.serialConnection.sendInstruction(self.Instructions.RapidPosition, [P1])
        self.serialConnection.sendInstruction(self.Instructions.Resume)

    def handleStartSample(self, averageInterval):
        self.isSampling = False
        self.serialConnection.sendInstruction(self.Instructions.Pause)
        self.serialConnection.sendInstruction(self.Instructions.StartProgram)
        self.serialConnection.sendInstruction(self.Instructions.AverageInterval, [averageInterval])
        self.serialConnection.sendInstruction(self.Instructions.TurnOnAdc)
        self.serialConnection.sendInstruction(self.Instructions.SampleAtPosition)
        self.serialConnection.sendInstruction(self.Instructions.TurnOffAdc)
        self.serialConnection.sendInstruction(self.Instructions.EndProgram)
        self.serialConnection.sendInstruction(self.Instructions.Resume)

    def readLoop(self):

        while True:
            #bytesToRead = self.serialConnection.ser.in_waiting

            #if (bytesToRead > 0):
                messageType = self.serialConnection.ser.read(2)
                if messageType == b'\xff\xff':

                    self.readSampleData()

                    #endFrame = self.serialConnection.ser.read(2)
                    #print(f'End Frame: {endFrame}')

                elif(messageType == b'\xff\xfc'):
                    pass
                    #Read Calibration

    def saveSetting(self, section, field, value):
        config = configparser.ConfigParser()
        config.read('./GUI/settings.ini')

        config[section][field] = value

        with open('./GUI/settings.ini', 'w') as configFile:
            config.write(configFile)

    def readSampleData(self):
        sampleBytes = self.serialConnection.readInt()
        remainder = self.readCrc(sampleBytes)
        if (remainder != 0):
            print("---------- TRANMISSION ERROR OCCURED ----------")


        timeBytes = self.serialConnection.readFloat()
        remainder = self.readCrc(timeBytes)
        if (remainder != 0):
            print("---------- TRANMISSION ERROR OCCURED ----------")

        positionBytes = self.serialConnection.readFloat()
        remainder = self.readCrc(positionBytes)
        if (remainder != 0):
            print("---------- TRANMISSION ERROR OCCURED ----------")

        sample = struct.unpack('h', sampleBytes)[0]
        time = struct.unpack('f', timeBytes)[0]
        position = struct.unpack('f', positionBytes)[0]

        # print(f'Sample: {sample}')
        # print(f'Time: {time}')
        # print(f'Position: {position}')

        self.samples.append(round(sample, 4))
        self.times.append(round(time, 4))
        self.positions.append(round(position, 4))

    def readCrc(self, data):
        crc = self.serialConnection.ser.read(2)
        crcData = data + crc

        return self.crc.table_driven(crcData)

    def handleClearSamples(self):
        """
            Clear the samples list for the controller. [ Need to relink the list on the view. ]
        """
        self.samples = []
        self.times = []
        self.positions = []

    def handleClearQueue(self):

        self.isSampling = False
        self.serialConnection.sendInstruction(self.Instructions.Pause)
        self.serialConnection.sendInstruction(self.Instructions.Clear)
        self.serialConnection.sendInstruction(self.Instructions.Resume)