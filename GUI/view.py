from PyQt5.QtWidgets import (QMainWindow, QSlider,
    QLabel, QApplication, QSpinBox, QPushButton, QComboBox, QMenu, QWidget, QFileDialog, QLineEdit, QFormLayout)
from PyQt5.QtCore import Qt, QSize
from PyQt5 import QtWidgets
from PyQt5.QtGui import QPixmap, QDoubleValidator
import sys
import csv
import math

from dynamicCanvas import DynamicMplCanvas

class View(QMainWindow):

    def __init__(self, serialConnection, controller):
        super(View, self).__init__()
        self.serialConnection = serialConnection
        self.controller = controller

        self.setAttribute(Qt.WA_DeleteOnClose)

        self.y = 100

        self.SmpDuration_Label = QLabel(self)
        self.SmpDuration_Label.setGeometry(30, self.y, 250, 30)
        self.SmpDuration_Label.setText("Sample Duration (0.1 ms)")

        self.y += 40

        self.SmpDuration_SpinBox = QSpinBox(self)
        self.SmpDuration_SpinBox.setGeometry(30, self.y, 120, 25)
        self.SmpDuration_SpinBox.setRange(7, 65536)
        self.SmpDuration_SpinBox.setValue(7)
        self.SmpDuration_SpinBox.valueChanged[int].connect(self.__updateSmpDurationSpinBox)

        self.y -= 40

        self.AvgInterval_Label = QLabel(self)
        self.AvgInterval_Label.setGeometry(300, self.y, 200, 30)
        self.AvgInterval_Label.setText("Averaging Interval")

        self.y += 40

        self.AvgInterval_SpinBox = QSpinBox(self)
        self.AvgInterval_SpinBox.setGeometry(300, self.y, 120, 25)
        self.AvgInterval_SpinBox.setRange(1, 1000)
        self.AvgInterval_SpinBox.setValue(10)
        self.AvgInterval_SpinBox.valueChanged[int].connect(self.__updateAvgIntervalSpinBox)

        self.y += 40

        self.StepSize_Label = QLabel(self)
        self.StepSize_Label.setGeometry(30, self.y, 150, 30)
        self.StepSize_Label.setText("Step Size (mm - rounded to nearest motor step on enter)")

        self.y += 40

        self.StepSize_LineEdit = QLineEdit(self)
        self.StepSize_LineEdit.setValidator(QDoubleValidator(0.018,5.0, 3))
        self.StepSize_LineEdit.setGeometry(30, self.y, 200, 25)
        self.StepSize_LineEdit.editingFinished.connect(self.__updateStepSizeEditLine)

        self.y += 100

        self.P1_Label = QLabel(self)
        self.P1_Label.setGeometry(30, self.y, 100, 30)
        self.P1_Label.setText("Position 1")

        self.y += 30

        self.P1_Slider = QSlider(Qt.Horizontal, self)
        self.P1_Slider.setFocusPolicy(Qt.NoFocus)
        self.P1_Slider.setGeometry(30, self.y, 400, 15)
        self.P1_Slider.setRange(0, 55)
        self.P1_Slider.valueChanged[int].connect(self.__updateP1SpinBox)

        self.y -= 5

        self.P1_SpinBox = QSpinBox(self)
        self.P1_SpinBox.setGeometry(460, self.y, 60, 25)
        self.P1_SpinBox.setRange(0, 55)
        self.P1_SpinBox.valueChanged[int].connect(self.__updateP1Slider)

        self.y += 60

        self.P2_Label = QLabel(self)
        self.P2_Label.setGeometry(30, self.y, 100, 30)
        self.P2_Label.setText("Position 2")

        self.y += 30

        self.P2_Slider = QSlider(Qt.Horizontal, self)
        self.P2_Slider.setFocusPolicy(Qt.NoFocus)
        self.P2_Slider.setRange(0, 55)
        self.P2_Slider.setGeometry(30, self.y, 400, 15)
        self.P2_Slider.valueChanged[int].connect(self.__updateP2SpinBox)

        self.y -= 5

        self.P2_SpinBox = QSpinBox(self)
        self.P2_SpinBox.setGeometry(460, self.y, 60, 25)
        self.P2_SpinBox.setRange(0, 55)
        self.P2_SpinBox.valueChanged[int].connect(self.__updateP2Slider)

        self.y += 200

        self.Calibrate_Button = QPushButton(self)
        self.Calibrate_Button.setGeometry(30, self.y, 100, 35)
        self.Calibrate_Button.setText("Calibrate")
        self.Calibrate_Button.clicked.connect(lambda: self.controller.handleCalibrate())

        self.ScanBetween_Button = QPushButton(self)
        self.ScanBetween_Button.setGeometry(150, self.y, 150, 35)
        self.ScanBetween_Button.setText("Scan Between")
        self.ScanBetween_Button.clicked.connect(lambda: self.controller.handleScanBetween(self.P1_Slider.value(), self.P2_Slider.value(), self.SmpDuration_SpinBox.value(), self.StepSize_LineEdit.text()))

        self.Stop_Button = QPushButton(self)
        self.Stop_Button.setGeometry(320, self.y, 70, 35)
        self.Stop_Button.setText("Stop")
        self.Stop_Button.clicked.connect(lambda: self.controller.handleStop())

        self.GoTo_Button = QPushButton(self)
        self.GoTo_Button.setGeometry(410, self.y, 180, 35)
        self.GoTo_Button.setText("Go To Position 1")
        self.GoTo_Button.clicked.connect(lambda: self.controller.handleGoToPoint(self.P1_Slider.value()))

        self.ScanAtPoint_Button = QPushButton(self)
        self.ScanAtPoint_Button.setGeometry(610, self.y, 180, 35)
        self.ScanAtPoint_Button.setText("Start Sampling")
        self.ScanAtPoint_Button.clicked.connect(lambda: self.controller.handleStartSample(self.SmpDuration_SpinBox.value(), self.AvgInterval_SpinBox.value()))

        self.ClearGraph_Button = QPushButton(self)
        self.ClearGraph_Button.setGeometry(810, self.y, 180, 35)
        self.ClearGraph_Button.setText("Clear Samples")
        # self.ClearGraph_Button.clicked.connect(lambda: self.controller.handleClearSamples())
        self.ClearGraph_Button.clicked.connect(lambda: self.clearGraph())

        self.graph_widget = QtWidgets.QWidget(self)

        l = QtWidgets.QVBoxLayout(self.graph_widget)
        self.dc = DynamicMplCanvas(self.graph_widget, self.controller.samples, width=5, height=4, dpi=100)
        l.addWidget(self.dc)

        self.graph_widget.setGeometry(600, 50, 650, 500)


        self.file_menu = QMenu('&File', self)
        self.file_menu.addAction('&Quit', self.fileQuit,
                                 Qt.CTRL + Qt.Key_Q)
        self.file_menu.addAction('&Save Data', self.saveDataToCSV,
                                 Qt.CTRL + Qt.Key_S)
        self.menuBar().addMenu(self.file_menu)


        self.setMinimumSize(QSize(1280, 720))
        self.setGeometry(300, 300, 1280, 720)
        self.setWindowTitle('Linear Stage Controller')

    def __updateSmpDurationSpinBox(self, value):
        self.SmpDuration_SpinBox.setValue(value)

    def __updateStepSizeEditLine(self):
        value = self.StepSize_LineEdit.text()
        if (value == ""):
            return

        value = self.roundNearest(value, 0.018)
        self.StepSize_LineEdit.setText(value)

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


    def fileQuit(self):
            self.close()

    def saveDataToCSV(self):
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        fileName, _ = QFileDialog.getSaveFileName(self,"QFileDialog.getSaveFileName()","","CSV Files (*.csv)", options=options)

        if (fileName != ""):
            if (fileName[-4:] != ".csv"):
                fileName = fileName + ".csv"
            with open(fileName, 'w+' ) as newFile:
                for s in self.controller.samples:
                    newFile.write(str(s))
                    newFile.write('\n')

    def clearGraph(self):
        self.controller.samples = []
        self.dc.resetSamples(self.controller.samples)
        self.dc.compute_initial_figure()
        self.dc.update_figure()

    def closeEvent(self, ce):
        self.fileQuit()

    def roundNearest(self, value, a):
        return str(round(round(float(value) / a) * a,  3))