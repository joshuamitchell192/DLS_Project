from PyQt5.QtWidgets import (QApplication)
# from dialog import Ui_Dialog as Form
import serial.tools.list_ports as port_list
import sys

from view import View
from serialConnection import SerialConnection
from controller import Controller
from instructions import Instructions

class App (QApplication):
    
    def __init__(self, sys_argv):
        super(App, self).__init__(sys_argv)
        
        # dialog = QDialog()
        # dialog.ui = Form()
        # dialog.ui.setupUi(dialog)
        # dialog.exec_()
        # dialog.show()

        ports = list(port_list.comports())
        for p in ports:
            print (f'{p} is visible')

        port = input("Enter the COM port you're connected to: ")

        self.serialConnection = SerialConnection(port)
        self.controller = Controller(self.serialConnection, Instructions)
        self.view = View(self.serialConnection, self.controller)
        self.view.show()

        #self.serialConnection.connectionTest()

if __name__ == '__main__':
    app = App(sys.argv)
    sys.exit(app.exec_())