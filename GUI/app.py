import configparser
import os
import sys
import threading

from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QApplication

from Controllers.controller import Controller
from Models.instructions import Instructions
from Models.sampleData import SampleData
from Services.serialConnection import SerialConnection
from Views.mainView import View


class App (QApplication):
    
    def __init__(self, sys_argv):
        super(App, self).__init__(sys_argv)
        self.absoluteInstallationPath = os.path.dirname(os.path.abspath(__file__))
        self.sampleData = SampleData()

        port = self.loadConfigComPort()

        self.serialConnection = SerialConnection(port)
        self.controller = Controller(self.serialConnection, Instructions, self.sampleData)
        self.view = View(self.serialConnection, self.controller, self.sampleData)
        self.view.show()

        thread = threading.Thread(target=self.controller.readLoop, args=())
        # probably should use an signalling mechanism like an Event to stop gracefully
        thread.daemon = True
        thread.start()
        self.loadConfig()

        self.view.calculateExpectedDuration()

    def loadConfigComPort(self):
        config = configparser.ConfigParser()
        config.read(self.absoluteInstallationPath + '/settings.ini')

        if ('Default' in config):
            if ('Port' in config['Default']):
                return config['Default']['Port']
        return ""

    def loadConfig(self):
        config = configparser.ConfigParser()
        config.read(self.absoluteInstallationPath + '/settings.ini')

        if ('Default' in config):

            defaultSettings = config['Default']

            if ("SampleDuration" in defaultSettings):
                self.view.SmpDuration_SpinBox.setValue(float(defaultSettings['SampleDuration']))
            else:
                print("Failed to load 'SampleDuration' from 'Default' section from settings")

            if ("mmBetweenSamples" in defaultSettings):
                self.view.StepLength_LineEdit.setValue(float(defaultSettings['mmBetweenSamples']))
            
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
    QApplication.setAttribute(Qt.AA_UseStyleSheetPropagationInWidgetStyles, True)

    app = App(sys.argv)

    dirname = os.path.dirname(os.path.abspath(__file__))
    ssFile = os.path.join(dirname, "stylesheet.qss")
    dropDownPath = os.path.join(dirname, "Assets/baseline_arrow_drop_down_black_18dp.png").replace('\\', '/')
    with open(ssFile) as fh:
        styleSheet = fh.read()
        styleSheet += """
            QComboBox::down-arrow {
                image: url(""" + dropDownPath + """);
            }
        """
        app.setStyleSheet(styleSheet)

    sys.exit(app.exec_())
