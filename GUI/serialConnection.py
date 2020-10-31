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

    def __init__(self):

        """ Sets up the serial connection with the specified configuration.

            :param: port - the name of the port to be used within the serial configuration.

        """
        self.crc = pycrc.algorithms.Crc(width=16, poly=0x8005, reflect_in=True, xor_in= 0x0000, reflect_out=True, xor_out = 0x0000)
        #self.ser = serial.Serial(port=port.upper(), baudrate=57600, bytesize=serial.EIGHTBITS, parity=serial.PARITY_ODD, stopbits=serial.STOPBITS_TWO, xonxoff=False, timeout=200)

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

    

    # def sendInstruction(self, instruction):

    #     """
    #         Send a single ascii character to inform the tiva of what task is should execute.

    #         :param: instruction - the instruction character to be sent to the tiva.
    #     """
    #     print(f'Sending: {instruction}\n')
    #     self.ser.write(instruction.encode("ascii"))

    #     self.ser.reset_input_buffer()

    #     ser_bytes = self.ser.read(1)
    #     print(f'Receiving\nraw data: {ser_bytes}')

        # decoded_bytes = (ser_bytes.decode("ascii"))
        # print(f'Ascii Value: {decoded_bytes}', flush=True)

    #def readCalibrationData():


    def sendInstruction(self, instruction, parameters = None):

        if parameters == None:
             wholeInstruction = instruction + '\n'
        else:
            wholeInstruction = instruction + " ".join(" {0}".format(n) for n in parameters) + "\n"
        self.ser.write(wholeInstruction.encode("ascii"))
        # for i in wholeInstruction:
        #     self.ser.write(i.encode("ascii"))

        print(f'Sending: {repr(wholeInstruction)}')
        self.ser.reset_output_buffer()
        time.sleep(0.018)

    # def sendValue(self, value):

    #     """ Sends a value associated with an instruction.

    #         :param: value - The raw value to be sent to the tiva.
    #     """

    #     print(f'Sending: {value}\n')
    #     self.ser.write(bytes([value]))

    #     self.ser.reset_input_buffer()
    #     ser_bytes = self.ser.read(1)
    #     print(f'Receiving\nraw data: {ser_bytes}')


        #decoded_bytes = (ser_bytes.decode("ascii"))

        #print(f'Ascii Value: {decoded_bytes}', flush=True)

    def readAdcSample(self):

        """ Waits to receive samples from the Tiva and combines bytes from separated writes

            :return: combined bytes for whole sample value.
        """
        ser_bytes_sample1 = self.ser.read(1)
        ser_bytes_sample2 = self.ser.read(1)

        crc = self.ser.read(2)

        ser_bytes_total = int.from_bytes(ser_bytes_sample1, byteorder='little', signed=False) + (int.from_bytes(ser_bytes_sample2, byteorder='little', signed=False) << 8)

        crcData = ser_bytes_total + crc
        remainder = self.crc.table_driven(crcData)
        
        if (remainder != 0):
            print("---------- TRANMISSION ERROR OCCURED ----------")
        print(f'Remainder: {remainder}')

        return ser_bytes_total

    def readInt(self):
        data1 = self.ser.read(1)
        data2 = self.ser.read(1)
        
        return int.from_bytes(data1, byteorder='little', signed=False) + (int.from_bytes(data2, byteorder='little', signed=False) << 8)
        # return data1 + data2

    def readFloat(self):
        ser_bytes_lower = self.ser.read(1)
        ser_bytes_mid = self.ser.read(1)
        ser_bytes_upper = self.ser.read(1)
        ser_bytes_upper_upper = self.ser.read(1)

        print(f'{ser_bytes_upper_upper} - {ser_bytes_upper} - {ser_bytes_mid} - {ser_bytes_lower}')
        print(f'{int.from_bytes(ser_bytes_upper_upper,"little")} - {int.from_bytes(ser_bytes_upper,"little")} - {int.from_bytes(ser_bytes_mid,"little")} - {int.from_bytes(ser_bytes_lower,"little")}')

        # crc = self.ser.read(1)
        # crc2 = self.ser.read(1)

        ser_bytes_total = ser_bytes_lower + ser_bytes_mid + ser_bytes_upper + ser_bytes_upper_upper 
        # ser_bytes_total = ser_bytes_upper_upper + ser_bytes_upper + ser_bytes_mid + ser_bytes_lower
        # crcData = ser_bytes_lower + ser_bytes_mid + ser_bytes_upper + ser_bytes_upper_upper + crc + crc2

        # remainder = self.crc.table_driven(crcData)
        # if (remainder != 0):
        #     print("---------- TRANMISSION ERROR OCCURED ----------")
        # print(f'Remainder: {remainder}')

        return struct.unpack('f', ser_bytes_total)[0]
        # return ser_bytes_total
