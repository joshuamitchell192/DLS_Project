import sys
import time

import serial
from PyQt5.QtWidgets import QDialog

from Views.portDialog import PortDialog


class SerialConnection:

    def __init__(self, port):

        """ Sets up the serial connection with the specified configuration.

        """

        if (self.connectionTest(port)):
            self.establishConnection(port)

        else:
            while (not self.connectionTest(port)):
                port = self.openPortModal()

            self.establishConnection(port)

    def establishConnection(self, port):
        self.ser = serial.Serial(port=port.upper(), baudrate=115200, bytesize=serial.EIGHTBITS, parity=serial.PARITY_ODD, stopbits=serial.STOPBITS_TWO, xonxoff=False, timeout=200)

    def connectionTest(self, port):

        """ Attempts to connect to the given port and returns if the connection was successful.

        """

        try:
            serial.Serial(port=port.upper(), baudrate=115200, bytesize=serial.EIGHTBITS, parity=serial.PARITY_ODD, stopbits=serial.STOPBITS_TWO, xonxoff=False, timeout=200)
            return True
        except(Exception):
            return False

    def sendInstruction(self, instruction, parameters = None):

        """ Sends an instruction over the serial connection after appending
            parameters in a space separated format with a new line ending the frame.

        """
        if parameters == None:
             wholeInstruction = instruction + '\n'
        else:
            wholeInstruction = instruction + " ".join(" {0}".format(n) for n in parameters) + "\n"
        self.ser.write(wholeInstruction.encode("ascii"))

        print(f'Sending: {repr(wholeInstruction)}')
        self.ser.reset_output_buffer()
        
        # Add a time delay to allow the microcontroller to keep up
        # between sending multiple instructions
        time.sleep(0.01)

    def readInt(self):
        return self.ser.read(2)

    def readFloat(self):
        return self.ser.read(4)
    
    def openPortModal(self):
        self.getPortDialog = PortDialog()
        self.getPortDialog.setModal(True)

        if (self.getPortDialog.exec_() == QDialog.Accepted):
            return self.getPortDialog.PortList.currentText()
        else:
            sys.exit()
