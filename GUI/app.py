from PyQt5.QtWidgets import (QApplication, QDialog, QErrorMessage)
from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtGui import QFont
import serial.tools.list_ports as port_list
import sys, threading, configparser
from pycrc.models import CrcModels
from pathlib import Path

from view import View
from serialConnection import SerialConnection
from controller import Controller
from instructions import Instructions
class App (QApplication):
    
    def __init__(self, sys_argv):
        super(App, self).__init__(sys_argv)

        port = self.loadConfigComPort()

        self.serialConnection = SerialConnection(port)
        self.controller = Controller(self.serialConnection, Instructions)
        self.view = View(self.serialConnection, self.controller)
        self.view.show()

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

    QApplication.setAttribute(Qt.AA_DisableWindowContextHelpButton)

    app = App(sys.argv)

    ssFile = "./GUI/stylesheet.qss"
    with open(ssFile) as fh:
        app.setStyleSheet(fh.read())

    sys.exit(app.exec_())