from PyQt5.QtWidgets import QApplication
import serial.tools.list_ports as port_list
import sys

from view import View
from serialConnection import SerialConnection
from controller import Controller
from instructions import Instructions

class App (QApplication):
    
    def __init__(self, sys_argv):
        super(App, self).__init__(sys_argv)
        
        ports = list(port_list.comports())
        for p in ports:
            print (f'{p} is visible')

        port = input("Enter the COM port you're connected to: ")
        self.serialConnection = SerialConnection(port)
        self.controller = Controller(self.serialConnection, Instructions)
        self.view = View(self.serialConnection, self.controller)
        self.view.show()

if __name__ == '__main__':
    app = App(sys.argv)
    sys.exit(app.exec_())