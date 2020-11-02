from pycrc.models import CrcModels
from instructions import Instructions
import serial
import serial.tools.list_ports as port_list
import time
import sys
import struct
import binascii
import pycrc.algorithms


class SerialConnection:

    def __init__(self, port):

        """ Sets up the serial connection with the specified configuration.

        """

        if (self.connectionTest(port)):
            self.establishConnection(port)

        else:
            while (not self.connectionTest(port)):
                # error_dialog = QErrorMessage()
                # error_dialog.setWindowTitle("Connection Failed")
                # error_dialog.showMessage('Failed to connect to the given port')
                port = self.openPortModal()

            self.establishConnection(port)

    def establishConnection(self, port):
        self.ser = serial.Serial(port=port.upper(), baudrate=57600, bytesize=serial.EIGHTBITS, parity=serial.PARITY_ODD, stopbits=serial.STOPBITS_TWO, xonxoff=False, timeout=200)

    def connectionTest(self, port):

        """ Attempts to connect to the given port and returns if the connection was successful.

        """

        try:
            serial.Serial(port=port.upper(), baudrate=57600, bytesize=serial.EIGHTBITS, parity=serial.PARITY_ODD, stopbits=serial.STOPBITS_TWO, xonxoff=False, timeout=200)
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
        time.sleep(0.03)

    def readInt(self):
        data1 = self.ser.read(1)
        data2 = self.ser.read(1)

        return data1 + data2

    def readFloat(self):
        ser_bytes_lower = self.ser.read(1)
        ser_bytes_mid = self.ser.read(1)
        ser_bytes_upper = self.ser.read(1)
        ser_bytes_upper_upper = self.ser.read(1)

        ser_bytes_total = ser_bytes_lower + ser_bytes_mid + ser_bytes_upper + ser_bytes_upper_upper
        return ser_bytes_total
