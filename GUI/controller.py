from view import View
from serialConnection import SerialConnection
from PyQt5.QtWidgets import QApplication
import time

class Controller:
    def __init__(self, serialConnection, Instructions):
        self.serialConnection = serialConnection
        self.Instructions = Instructions
        self.samples = []
        self.times = []
        self.stop = False
        self.stepsPerMM = 0.018

    def handleCalibrate(self):
        self.serialConnection.sendInstruction(self.Instructions.Calibrate, "")
        
       #self.stepsPerMM = (self.serialConnection.readSample() )


        print("NUMSTEPS ",self.stepsPerMM)

    def handleScanBetween(self, P1, P2, sampleDuration, mmBetweenSamples, stepMode):
        self.serialConnection.sendInstruction(self.Instructions.Stop, [])
        #self.serialConnection.readLine()
        self.serialConnection.sendInstruction(self.Instructions.StartProgram, [])
        #self.serialConnection.readLine()
        self.serialConnection.sendInstruction(self.Instructions.RapidPosition, [P1])
        #self.serialConnection.readLine()
        self.serialConnection.sendInstruction(self.Instructions.SampleDuration, [sampleDuration])
        #self.serialConnection.readLine()
        self.serialConnection.sendInstruction(self.Instructions.TurnOnAdc, [])
        #self.serialConnection.readLine()
        self.serialConnection.sendInstruction(self.Instructions.mmBetweenSamples, [mmBetweenSamples])
        #self.serialConnection.readLine()
        self.serialConnection.sendInstruction(self.Instructions.StepMode, [stepMode])
        #self.serialConnection.readLine()
        self.serialConnection.sendInstruction(self.Instructions.LinearPosition, [P2])
        #self.serialConnection.readLine()
        self.serialConnection.sendInstruction(self.Instructions.EndProgram, [])
        #self.serialConnection.readLine()
        self.serialConnection.sendInstruction(self.Instructions.Resume, [])
        #self.serialConnection.readLine()

    # def handleScanBetween(self, P1, P2, sampleDuration, stepLength, stepSize):

    #     """ Sends the TWOPOS_SCAN instruction along with its associated values.

    #         :param: P1 - the first position from the top slider widget
    #         :param: P2 - the second position from the bottom slider widget
    #         :param: sampleDuration - the value from sampleDuration spinbox indicating how low to sample for at each step.
    #         :param: stepLength - the value selected in the stepLength_combobox (Full, Half, or Quarter)

    #     """
    #     self.handleStop()
    #     self.stop = False
    #     self.serialConnection.sendInstruction(self.Instructions.TWOPOS_SCAN)
    #     self.serialConnection.sendValue(P1)
    #     self.serialConnection.sendValue(P2)

    #     sampleDurationWhole = int(float(sampleDuration) * 1000)
    #     sampleDurationLower = sampleDurationWhole & 0xFF
    #     sampleDurationMiddle = (sampleDurationWhole & 0xFF00) >> 8
    #     sampleDurationUpper = (sampleDurationWhole & 0xFF0000) >> 16

    #     self.serialConnection.sendValue(sampleDurationLower)
    #     self.serialConnection.sendValue(sampleDurationMiddle)
    #     self.serialConnection.sendValue(sampleDurationUpper)

    #     stepLengthWhole = int(float(stepLength) * 1000)
    #     stepLengthLower = stepLengthWhole & 0xFF
    #     stepLengthUpper = (stepLengthWhole & 0xFF00) >> 8

    #     self.serialConnection.sendValue(stepLengthLower)
    #     self.serialConnection.sendValue(stepLengthUpper)

    #     if (stepSize == "Full"):
    #         self.serialConnection.sendValue(2)
    #     elif (stepSize == "Half"):
    #         self.serialConnection.sendValue(1)
    #     elif (stepSize == "Quarter"):
    #         self.serialConnection.sendValue(0)
    #     else:
    #         self.serialConnection.sendValue(2)

    #     while(not self.stop):
    #         currentSample = self.serialConnection.readSample()
    #         if currentSample == 0xFFFF:
    #             break
    #         currentTime = self.serialConnection.readTime()
    #         print(currentTime)
    #         self.samples.append(currentSample)
            
    #         self.times.append(currentTime)
    #         QApplication.processEvents()

    def handleStop(self):

        """ Sets the stop boolean to true so that we cease reading samples.

        """
        self.stop = True
        self.serialConnection.sendStopInstruction(self.Instructions.STOP)


    def handleGoToPoint(self, P1):

        """ sends the GOTO instruction to move to position 1

        """
        self.handleStop()
        self.serialConnection.sendInstruction(self.Instructions.GOTO)
        self.serialConnection.sendValue(P1)

    def handleStartSample(self, averageInterval):

        """ Sends the START_SAMPLE instruction to turn on the ADC clock and wait to receive samples from the sensor through the tiva UART connection.
            Samples until the stop button is pressed.

            :param: sampleDuration -  [Deprecated]
            :param: averageInterval  - the amount of samples to take to average on.

        """
        self.handleStop()
        self.stop = False
        self.serialConnection.sendInstruction(self.Instructions.START_SAMPLE)
        #self.serialConnection.sendValue(sampleDuration)
        #self.serialConnection.sendValue(averageInterval)

        while(not self.stop):
            currentSample = self.serialConnection.readSample()
            # print("Current sample", currentSample)
            if currentSample == 0xFFFF:
                break
            self.samples.append(currentSample)
            self.times.append(20)
            # print(self.samples)
            QApplication.processEvents()



    def handleClearSamples(self):
        """
            Clear the samples list for the controller. [ Need to relink the list on the view. ]
        """
        self.samples = []


