from PyQt5.QtWidgets import (QMainWindow, QSlider,
    QLabel, QApplication, QSpinBox, QPushButton)
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QPixmap
import sys

class View(QMainWindow):

    def __init__(self, serialConnection, controller):
        super(View, self).__init__()
        self.serialConnection = serialConnection
        self.controller = controller

        self.P1_Label = QLabel(self)
        self.P1_Label.setGeometry(30, 20, 100, 30)
        self.P1_Label.setText("Position 1")

        self.P1_Slider = QSlider(Qt.Horizontal, self)
        self.P1_Slider.setFocusPolicy(Qt.NoFocus)
        self.P1_Slider.setGeometry(30, 50, 400, 15)
        self.P1_Slider.valueChanged[int].connect(self.__updateP1SpinBox)

        self.P1_SpinBox = QSpinBox(self)
        self.P1_SpinBox.setGeometry(160, 80, 60, 25)
        self.P1_SpinBox.valueChanged[int].connect(self.__updateP1Slider)
        
        self.Calibrate_Button = QPushButton(self)
        self.Calibrate_Button.setGeometry(30, 120, 90, 35)
        self.Calibrate_Button.setText("Calibrate")
        self.Calibrate_Button.clicked.connect(lambda: self.controller.handleCalibrate())

        self.Stop_Button = QPushButton(self)
        self.Stop_Button.setGeometry(140, 120, 70, 35)
        self.Stop_Button.setText("Stop")


        self.setGeometry(300, 300, 1280, 720)
        self.setWindowTitle('Linear Stage Controller')

    def __updateP1SpinBox(self, value):
        self.P1_SpinBox.setValue(value)

    def __updateP1Slider(self, value):
        self.P1_Slider.setValue(value)