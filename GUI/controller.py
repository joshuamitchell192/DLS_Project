from view import View
from serialConnection import SerialConnection
from PyQt5.QtWidgets import QApplication
import time, configparser
import struct
import pycrc.algorithms
import tracemalloc
import  os
from settings import Settings
from enum import Enum

class State(Enum):
    Idle = 0
    ScanBetween = 1
    StationarySample = 2
    Calibration = 3

class Controller:
    def __init__(self):
        pass
    def __init__(self, serialConnection, Instructions, sampleData):
        self.serialConnection = serialConnection
        self.Instructions = Instructions
        self.pause = False
        self.stepsPerMM = 0.018
        self.isSampling = False
        self.crc = pycrc.algorithms.Crc(width=16, poly=0x8005, reflect_in=True, xor_in= 0x0000, reflect_out=True, xor_out = 0x0000)
        self.state = State.Idle
        self.sampleData = sampleData
        

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

    def handleGoToPoint(self, position):

        """ sends the Rapid Position instruction to move to the given position

        """
        self.isSampling = False
        self.serialConnection.sendInstruction(self.Instructions.Pause)
        self.serialConnection.sendInstruction(self.Instructions.TurnOffAdc)
        self.serialConnection.sendInstruction(self.Instructions.RapidPosition, [position])
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
            messageType = self.serialConnection.ser.read(2)
            if messageType == b'\xff\xff':
                if (self.state != State.ScanBetween):
                    self.sampleData.linePlotData.addLine()
                    self.state = State.ScanBetween
                self.readSampleData()

            if messageType == b'\xff\xfe':
                self.state = State.Idle

            if messageType == b'\xff\xfd':
                self.state = State.StationarySample
                self.readSampleData()

            elif(messageType == b'\xff\xfc'):
                self.state = State.Calibration
                stepsPerMMBytes = self.serialConnection.readInt()
                stageCalibrationStepsPerMM = struct.unpack('h', stepsPerMMBytes)[0]

                Settings.saveSetting("Calibration", "stepspermm", str(stageCalibrationStepsPerMM))


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

        #time = self.secondsToRTC(time)
        if (self.state == State.ScanBetween):

            self.sampleData.linePlotData.samples[self.sampleData.linePlotData.getLineIndex()].append(round(sample, 4))
            self.sampleData.linePlotData.times[self.sampleData.linePlotData.getLineIndex()].append(time)
            self.sampleData.linePlotData.positions[self.sampleData.linePlotData.getLineIndex()].append(round(position, 4))

        elif (self.state == State.StationarySample):
            
            self.sampleData.scatterPlotData.samples.append(round(sample, 4))
            self.sampleData.scatterPlotData.times.append(round(time, 4))
            self.sampleData.scatterPlotData.positions.append(round(position, 4))

    def secondsToRTC(self, time):
        minutes = time // 60
        seconds = round(time - (minutes * 60), 4)
        #milliSeconds = round(seconds // )
        #hours = minutes // 60

        return f'{int(minutes)} m : {seconds} s'#:{milliSeconds} ms'

    def readCrc(self, data):
        crc = self.serialConnection.ser.read(2)
        crcData = data + crc

        return self.crc.table_driven(crcData)

    def handleClearQueue(self):

        self.isSampling = False
        self.serialConnection.sendInstruction(self.Instructions.Pause)
        self.serialConnection.sendInstruction(self.Instructions.Clear)
        self.serialConnection.sendInstruction(self.Instructions.Resume)