from instructions import Instructions
import serial
import serial.tools.list_ports as port_list
import time
import sys

class SerialConnection:


    def __init__(self, port):

        self.ser = serial.Serial(port=port.upper(), baudrate=56700, bytesize=serial.EIGHTBITS, parity=serial.PARITY_ODD, stopbits=serial.STOPBITS_TWO, xonxoff=False, timeout=200)
        

        # self.sendInstruction(Instructions.CALIBRATE)

    def connectionTest(self):
        print("Turn on the Tiva")
        g_byte = self.ser.read(1)
        o_byte = self.ser.read(1)

        if (g_byte.decode("ascii") != "G") | (o_byte.decode("ascii") != "O"):
            print("Unsuccessful Serial Connection to Tiva, Try Again")
        else:
            print("Successful Serial Connection to Tiva")

    def sendInstruction(self, instruction):

        print(f'Sending: {instruction}\n')
        self.ser.write(instruction.encode("ascii"))
        
        self.ser.reset_input_buffer()

        ser_bytes = self.ser.read(1)
        print(f'Receiving\nraw data: {ser_bytes}')

        decoded_bytes = (ser_bytes.decode("ascii"))
        print(f'Ascii Value: {decoded_bytes}', flush=True)

    def sendValue(self, value):

        print(f'Sending: {value}\n')
        self.ser.write(bytes([value]))


        self.ser.reset_input_buffer()
        ser_bytes = self.ser.read(1)
        print(f'Receiving\nraw data: {ser_bytes}')

        
        decoded_bytes = (ser_bytes.decode("ascii"))
           
        print(f'Ascii Value: {decoded_bytes}', flush=True)

    def readSample(self):

        #self.ser.reset_input_buffer()
        # print("Waiting", self.ser.in_waiting)
        ser_bytes_sample1 = self.ser.read(1)


        ser_bytes_sample2 = self.ser.read(1)

        # print("BYTES RAW  ",ser_bytes_sample1,ser_bytes_sample2)
        # print("BYTE INTS ", int.from_bytes(ser_bytes_sample1, byteorder='little') ,(int.from_bytes(ser_bytes_sample2, byteorder='little')))

        ser_bytes_total = int.from_bytes(ser_bytes_sample1, byteorder='little', signed=False) + (int.from_bytes(ser_bytes_sample2, byteorder='little', signed=False) << 8)






        # decoded_bytes = (ser_bytes.decode("ascii"))
        # print(f'Ascii Value: {decoded_bytes}', flush=True)

        # print(f'Sending Ackt: {value}\n')
        # self.ser.write(bytes([value]))

        return ser_bytes_total

    def sendStopInstruction(self, instruction):

        print(f'Sending: {instruction}\n')
        self.ser.write(instruction.encode("ascii"))

        #while (self.ser.in_waiting > 1):
        self.ser.reset_input_buffer()
        ser_bytes = self.ser.read(1)
        #print(f'Receiving\nraw data: {ser_bytes}')

        #decoded_bytes = (ser_bytes.decode("ascii"))
        #print(f'Ascii Value: {decoded_bytes}', flush=True)


