<<<<<<< HEAD
from PyQt5.QtWidgets import (QApplication)
# from dialog import Ui_Dialog as Form
import serial.tools.list_ports as port_list
import sys
=======
from PyQt5.QtWidgets import (QApplication, QDialog, QErrorMessage)
from PyQt5.QtCore import Qt, QTimer
import serial.tools.list_ports as port_list
import sys, threading, configparser
from pycrc.models import CrcModels
>>>>>>> tivaCpp

from view import View
from serialConnection import SerialConnection
from controller import Controller
from instructions import Instructions

<<<<<<< HEAD
=======

>>>>>>> tivaCpp
class App (QApplication):
    
    def __init__(self, sys_argv):
        super(App, self).__init__(sys_argv)
<<<<<<< HEAD
        
        # dialog = QDialog()
        # dialog.ui = Form()
        # dialog.ui.setupUi(dialog)
        # dialog.exec_()
        # dialog.show()

        ports = list(port_list.comports())
        for p in ports:
            print (f'{p} is visible')

        port = input("Enter the COM port you're connected to: ")
=======

        port = self.loadConfigComPort()
>>>>>>> tivaCpp

        self.serialConnection = SerialConnection(port)
        self.controller = Controller(self.serialConnection, Instructions)
        self.view = View(self.serialConnection, self.controller)
        self.view.show()

<<<<<<< HEAD
        #self.serialConnection.connectionTest()

if __name__ == '__main__':
    app = App(sys.argv)
=======
        thread = threading.Thread(target=self.controller.readLoop, args=())
        # probably should use an signalling mechanism like an Event to stop gracefully
        thread.daemon = True
        thread.start()
        self.loadConfig()

        self.view.calculateExpectedDuration()

    def loadConfigComPort(self):
        config = configparser.ConfigParser()
        config.read('./GUI/settings.ini')

        if ('Default' in config):
            if ('Port' in config['Default']):
                return config['Default']['Port']
        return ""

    def loadConfig(self):
        config = configparser.ConfigParser()
        config.read('./GUI/settings.ini')

        if ('Default' in config):

            defaultSettings = config['Default']

            if ("SampleDuration" in defaultSettings):
                self.view.SmpDuration_LineEdit.setText(defaultSettings['SampleDuration'])
            else:
                print("Failed to load 'SampleDuration' from 'Default' section from settings")

            if ("mmBetweenSamples" in defaultSettings):
                self.view.StepLength_LineEdit.setText(defaultSettings['mmBetweenSamples'])
            
            if ("StartPosition" in defaultSettings):
                self.view.P1_Slider.setValue(int(defaultSettings["StartPosition"]))
                self.view.P1_SpinBox.setValue(int(defaultSettings["StartPosition"]))

            if ("EndPosition" in defaultSettings):
                self.view.P2_Slider.setValue(int(defaultSettings["EndPosition"]))
                self.view.P2_SpinBox.setValue(int(defaultSettings["EndPosition"]))

            if ("AverageInterval" in defaultSettings):
                self.view.AvgInterval_SpinBox.setValue(int(defaultSettings["AverageInterval"]))
            

if __name__ == '__main__':

    if hasattr(Qt, 'AA_EnableHighDpiScaling'):
        QApplication.setAttribute(Qt.AA_EnableHighDpiScaling, True)
    app = App(sys.argv)

    stylesheet = """
            View {
                background-color: white;
            }
        """

    app.setStyleSheet(stylesheet)
>>>>>>> tivaCpp
    sys.exit(app.exec_())