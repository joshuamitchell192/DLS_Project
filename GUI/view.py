from PyQt5.QtWidgets import (QMainWindow, QSlider,
    QLabel, QApplication, QSpinBox, QPushButton, QComboBox)
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QPixmap
import sys

class View(QMainWindow):

    def __init__(self, serialConnection, controller):
        super(View, self).__init__()
        self.serialConnection = serialConnection
        self.controller = controller
        
        self.y = 20

        self.SmpDuration_Label = QLabel(self)
        self.SmpDuration_Label.setGeometry(30, self.y, 200, 30)
        self.SmpDuration_Label.setText("Sample Duration (mm)")

        self.y += 30

        self.SmpDuration_SpinBox = QSpinBox(self)
        self.SmpDuration_SpinBox.setGeometry(50, self.y, 120, 25)
        self.SmpDuration_SpinBox.valueChanged[int].connect(self.__updateSmpDurationSpinBox)
        
        self.y += 30

        self.SmpDuration_Label = QLabel(self)
        self.SmpDuration_Label.setGeometry(30, self.y, 150, 30)
        self.SmpDuration_Label.setText("Step Size (mm)")

        self.y += 30

        self.StepSize_ComboBox = QComboBox()
        self.StepSize_ComboBox.addItem("Full")
        self.StepSize_ComboBox.addItem("Half")
        self.StepSize_ComboBox.addItem("Quarter")
        self.StepSize_ComboBox.currentIndexChanged.connect(self.__updateStepSizeComboBox)
        self.StepSize_ComboBox.setGeometry(50, self.y, 120, 25)

        self.y += 30

        self.SmpDuration_Label = QLabel(self)
        self.SmpDuration_Label.setGeometry(30, self.y, 200, 30)
        self.SmpDuration_Label.setText("Averaging Interval")

        self.y += 30

        self.AvgInterval_SpinBox = QSpinBox(self)
        self.AvgInterval_SpinBox.setGeometry(50, self.y, 120, 25)
        self.AvgInterval_SpinBox.valueChanged[int].connect(self.__updateAvgIntervalSpinBox)

        self.y += 100

        self.P1_Label = QLabel(self)
        self.P1_Label.setGeometry(30, self.y, 100, 30)
        self.P1_Label.setText("Position 1")

        self.y += 30

        self.P1_Slider = QSlider(Qt.Horizontal, self)
        self.P1_Slider.setFocusPolicy(Qt.NoFocus)
        self.P1_Slider.setGeometry(30, self.y, 400, 15)
        self.P1_Slider.valueChanged[int].connect(self.__updateP1SpinBox)

        self.y += 30

        self.P1_SpinBox = QSpinBox(self)
        self.P1_SpinBox.setGeometry(160, self.y, 60, 25)
        self.P1_SpinBox.setRange(0, 55)
        self.P1_SpinBox.valueChanged[int].connect(self.__updateP1Slider)

        self.y += 60

        self.P2_Label = QLabel(self)
        self.P2_Label.setGeometry(30, self.y, 100, 30)
        self.P2_Label.setText("Position 2")

        self.y += 30

        self.P2_Slider = QSlider(Qt.Horizontal, self)
        self.P2_Slider.setFocusPolicy(Qt.NoFocus)
        self.P2_Slider.setGeometry(30, self.y, 400, 15)
        self.P2_Slider.valueChanged[int].connect(self.__updateP2SpinBox)

        self.y += 30

        self.P2_SpinBox = QSpinBox(self)
        self.P2_SpinBox.setGeometry(160, self.y, 60, 25)
        self.P2_SpinBox.setRange(0, 55)
        self.P2_SpinBox.valueChanged[int].connect(self.__updateP2Slider)

        self.y += 190
        
        self.Calibrate_Button = QPushButton(self)
        self.Calibrate_Button.setGeometry(30, self.y, 90, 35)
        self.Calibrate_Button.setText("Calibrate")
        self.Calibrate_Button.clicked.connect(lambda: self.controller.handleCalibrate())

        self.ScanBetween_Button = QPushButton(self)
        self.ScanBetween_Button.setGeometry(140, self.y, 70, 35)
        self.ScanBetween_Button.setText("Scan Between")
        self.ScanBetween_Button.clicked.connect(lambda: self.controller.handleScanBetween(self.P1_Slider.value, self.P1_Slider.value, self.SmpDuration_SpinBox.value, self.StepSize_ComboBox.currentText()))

        self.Stop_Button = QPushButton(self)
        self.Stop_Button.setGeometry(240, self.y, 70, 35)
        self.Stop_Button.setText("Stop")
        self.Stop_Button.clicked.connect(lambda: self.controller.handleStop())




        self.setGeometry(300, 300, 1280, 720)
        self.setWindowTitle('Linear Stage Controller')

    def __updateSmpDurationSpinBox(self, value):
        self.SmpDuration_SpinBox.setValue(value)

    def __updateStepSizeComboBox(self, value):
        self.StepSize_SpinBox.setValue(value)

    def __updateAvgIntervalSpinBox(self, value):
        self.AvgInterval_SpinBox.setValue(value)

    def __updateP1SpinBox(self, value):
        self.P1_SpinBox.setValue(value)

    def __updateP1Slider(self, value):
        self.P1_Slider.setValue(value)

    def __updateP2SpinBox(self, value):
        self.P2_SpinBox.setValue(value)

    def __updateP2Slider(self, value):
        self.P2_Slider.setValue(value)