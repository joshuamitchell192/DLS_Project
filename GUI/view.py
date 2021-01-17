from PyQt5.QtWidgets import (QMainWindow, QSlider,
    QLabel, QApplication, QSpinBox, QDoubleSpinBox, QPushButton, QComboBox, QFrame, QMenu, QWidget, QFileDialog, QLineEdit, QFormLayout)
from PyQt5.QtCore import Qt, QSize, QRect, QTimer
from PyQt5 import QtWidgets
from PyQt5.QtGui import QPixmap, QDoubleValidator, QIcon
import sys
import csv
import math
from matplotlib import animation
# from animatedCanvas4 import AnimatedMplCanvasWidget

from dynamicCanvas import DynamicMplCanvas
from dynamicCanvasPerformance import DynamicMplCanvasPerformance

class View(QMainWindow):

    def __init__(self, serialConnection, controller):
        super(View, self).__init__()
        self.serialConnection = serialConnection
        self.controller = controller
        self.currentStepsPerMM = self.controller.stepsPerMM

        self.setAttribute(Qt.WA_DeleteOnClose)

        self.y = 50

        self.Calibrate_Button = QPushButton(self)
        self.Calibrate_Button.setGeometry(40, self.y, 100, 35)
        self.Calibrate_Button.setText("Calibrate")
        self.Calibrate_Button.clicked.connect(lambda: self.controller.handleCalibrate())

        self.y += 80

        self.P1_Label = QLabel(self)
        self.P1_Label.setGeometry(40, self.y, 100, 30)
        self.P1_Label.setText("Start Position")

        self.y += 30

        self.P1_Slider = QSlider(Qt.Horizontal, self)
        self.P1_Slider.setFocusPolicy(Qt.NoFocus)
        self.P1_Slider.setGeometry(40, self.y, 400, 15)
        self.P1_Slider.setRange(0, 65)
        self.P1_Slider.setTickInterval(5)
        self.P1_Slider.TickPosition(self.P1_Slider.TicksBelow)
        self.P1_Slider.valueChanged[int].connect(self.__updateP1SpinBox)

        self.y -= 5

        self.P1_SpinBox = QSpinBox(self)
        self.P1_SpinBox.setGeometry(460, self.y, 60, 25)
        self.P1_SpinBox.setRange(0, 65)
        self.P1_SpinBox.valueChanged[int].connect(self.__updateP1Slider)

        self.y += 50

        self.P2_Label = QLabel(self)
        self.P2_Label.setGeometry(40, self.y, 100, 30)
        self.P2_Label.setText("End Position")

        self.y += 30

        self.P2_Slider = QSlider(Qt.Horizontal, self)
        self.P2_Slider.setFocusPolicy(Qt.NoFocus)
        self.P2_Slider.setRange(0, 65)
        self.P2_Slider.setGeometry(40, self.y, 400, 15)
        self.P2_Slider.setValue(10)
        self.P2_Slider.valueChanged[int].connect(self.__updateP2SpinBox)

        self.y -= 5

        self.P2_SpinBox = QSpinBox(self)
        self.P2_SpinBox.setGeometry(460, self.y, 60, 25)
        self.P2_SpinBox.setRange(0, 65)
        self.P2_SpinBox.setValue(10)
        self.P2_SpinBox.valueChanged[int].connect(self.__updateP2Slider)

        self.y += 80

        self.SmpDuration_Label = QLabel(self)
        self.SmpDuration_Label.setGeometry(40, self.y, 250, 30)
        self.SmpDuration_Label.setText("Sample Duration (ms)")
        
        self.StepMode_Combo = QLabel(self)
        self.StepMode_Combo.setGeometry(250, self.y, 150, 30)
        self.StepMode_Combo.setText("Step Mode")

        self.y += 30

        self.SmpDuration_LineEdit = QLineEdit(self)
        self.SmpDuration_LineEdit.setValidator(QDoubleValidator(0.018,5.0, 3))
        self.SmpDuration_LineEdit.setGeometry(40, self.y, 150, 25)
        self.SmpDuration_LineEdit.setText("0.001")
        self.SmpDuration_LineEdit.editingFinished.connect(self.__updateSmpDurationEditLine)
        self.SmpDuration_LineEdit.textChanged.connect(self.SampleDurationUpdate)
        
        self.StepMode_ComboBox_Widget = QWidget(self)
        self.StepMode_ComboBox_Widget.setGeometry(250, self.y, 120, 35)
        # self.setCentralWidget(StepMode_ComboBox_Widget)

        self.StepMode_ComboBox = QComboBox(self.StepMode_ComboBox_Widget)
        self.StepMode_ComboBox.setObjectName(("StepMode_ComboBox"))
        self.StepMode_ComboBox.addItem("Quarter")
        self.StepMode_ComboBox.addItem("Half")
        self.StepMode_ComboBox.addItem("Full")
        self.StepMode_ComboBox.setCurrentIndex(2)
        self.StepMode_ComboBox.currentIndexChanged.connect(self.__updateStepModeComboBox)

        self.y += 40

        self.StepLength_Label = QLabel(self)
        self.StepLength_Label.setGeometry(40, self.y, 150, 30)
        # rounded to nearest motor step on enter
        self.StepLength_Label.setText("Step Length (mm)")


        self.y += 30

        self.StepLength_LineEdit = QLineEdit(self)
        #self.StepLength_LineEdit.setValidator(QDoubleValidator(self.controller.stepsPerMM,5.0, 7))
        self.StepLength_LineEdit.setGeometry(40, self.y, 150, 25)
        self.StepLength_LineEdit.setText("0.1")
        self.StepLength_LineEdit.editingFinished.connect(self.__updateStepLengthEditLine)
        self.StepLength_LineEdit.textChanged.connect(self.StepLengthUpdate)

        self.ScanBetween_Button = QPushButton(self)
        self.ScanBetween_Button.setGeometry(250, self.y - 10, 120, 35)
        self.ScanBetween_Button.setText("Scan Between")
        self.ScanBetween_Button.clicked.connect(lambda: self.handleScanBetween())

        self.y += 60

        self.seperator = QFrame(self)
        self.seperator.setObjectName("Seperator")
        self.seperator.setGeometry(QRect(30, self.y, 550, 5))
        self.seperator.setFrameShape(QFrame.HLine)
        self.seperator.setFrameShadow(QFrame.Raised)
        
        self.y += 25
        
        self.AvgInterval_Label = QLabel(self)
        self.AvgInterval_Label.setGeometry(40, self.y, 200, 30)
        self.AvgInterval_Label.setText("Averaging Interval")

        self.y += 30

        self.AvgInterval_SpinBox = QDoubleSpinBox(self)
        self.AvgInterval_SpinBox.setGeometry(40, self.y, 120, 25)
        self.AvgInterval_SpinBox.setRange(1, 1000)
        self.AvgInterval_SpinBox.setValue(5.0)
        self.AvgInterval_SpinBox.valueChanged[float].connect(self.__updateAvgIntervalSpinBox)

        
        self.ScanAtPoint_Button = QPushButton(self)
        self.ScanAtPoint_Button.setGeometry(250, self.y - 10, 120, 35)
        self.ScanAtPoint_Button.setText("Start Sampling")
        self.ScanAtPoint_Button.clicked.connect(lambda: self.controller.handleStartSample(self.AvgInterval_SpinBox.value()))

        self.GoTo_Button = QPushButton(self)
        self.GoTo_Button.setGeometry(390, self.y - 10, 120, 35)
        self.GoTo_Button.setText("Move to Start")
        self.GoTo_Button.clicked.connect(lambda: self.controller.handleGoToPoint(self.P1_Slider.value()))

        self.y += 5

        self.TimeElapsed_Label = QLabel(self)
        self.TimeElapsed_Label.setGeometry(650, self.y, 150, 30)
        self.TimeElapsed_Label.setText("Time Elapsed: ")

        self.CurrentPosition_Label = QLabel(self)
        self.CurrentPosition_Label.setGeometry(650, self.y + 30, 150, 30)
        self.CurrentPosition_Label.setText("Current Position:")

        self.ExpectedDuration_Label = QLabel(self)
        self.ExpectedDuration_Label.setGeometry(650, self.y + 60, 150, 30)
        self.ExpectedDuration_Label.setText("Expected Duration:")


        self.ClearGraph_Button = QPushButton(self)
        self.ClearGraph_Button.setGeometry(1075, self.y, 120, 35)
        self.ClearGraph_Button.setText("Clear Samples")
        # self.ClearGraph_Button.clicked.connect(lambda: self.controller.handleClearSamples())
        self.ClearGraph_Button.clicked.connect(lambda: self.clearGraph())

        self.y += 120

        self.Stop_Button = QPushButton(self)
        self.Stop_Button.setGeometry(40, self.y, 100, 35)
        self.Stop_Button.setText("Stop")
        self.Stop_Button.setToolTip("Stops executing the current instruction")
        self.Stop_Button.clicked.connect(self.toggleStop)

        self.ClearQueue_Button = QPushButton(self)
        self.ClearQueue_Button.setGeometry(150, self.y, 100, 35)
        self.ClearQueue_Button.setText("Clear Queue")
        self.ClearQueue_Button.setToolTip("Clears all instructions yet to be executed")
        self.ClearQueue_Button.clicked.connect(lambda: self.controller.handleClearQueue())

        self.graph_widget = QtWidgets.QWidget(self)

        l = QtWidgets.QVBoxLayout(self.graph_widget)

        self.dc = DynamicMplCanvas(self.graph_widget, self.controller.samples, self.controller.times, self.controller.positions, width=5, height=4, dpi=100)
        l.addWidget(self.dc)
        self.dc.show()

        self.graph_widget.setGeometry(575, 30, 700, 500)


        self.file_menu = QMenu('&File', self)
        self.file_menu.addAction('&Quit', self.fileQuit,
                                 Qt.CTRL + Qt.Key_Q)
        self.file_menu.addAction('&Save Data', self.saveDataToCSV,
                                 Qt.CTRL + Qt.Key_S)
        self.file_menu.addAction('&Load Program', self.loadProgram, Qt.CTRL + Qt.Key_L)
        self.menuBar().addMenu(self.file_menu)

        self.timer = QTimer()
        self.timer.timeout.connect(self.updateLabels)
        self.timer.start(32)

        #self.setBaseSize(QSize(1280, 720))
        self.setMinimumSize(QSize(704, 480))
        self.setMaximumSize(QSize(1920, 1080))
        self.setGeometry(300, 300, 1280, 720)
        self.setWindowFlags(Qt.WindowCloseButtonHint | Qt.WindowMinimizeButtonHint)
        self.setWindowTitle('Linear Stage Controller')
        self.setWindowIcon(QIcon())

    # def __updateSmpDurationSpinBox(self, value):
    #     self.SmpDuration_SpinBox.setValue(value)

    def updateLabels(self):
        if (len(self.controller.times[self.controller.currentRow]) > 0):
            self.TimeElapsed_Label.setText("Time Elapsed: " + str( self.controller.secondsToRTC(self.controller.times[self.controller.currentRow][-1])))
        else:
            self.TimeElapsed_Label.setText("Time Elapsed:")

        if (len(self.controller.positions[self.controller.currentRow]) > 0):
            self.CurrentPosition_Label.setText("Current Position: " + str(round(self.controller.positions[self.controller.currentRow][-1], 3)))
        else:
            self.CurrentPosition_Label.setText("Current Position:")

    def __updateSmpDurationEditLine(self):
        value = self.SmpDuration_LineEdit.text()
        if (value == ""):
            return

    def __updateStepLengthEditLine(self):
        value = self.StepLength_LineEdit.text()
        if (value == ""):
            return
        #self.StepLength_LineEdit.setValidator(QDoubleValidator(self.controller.stepsPerMM,5.0, 7))

        value = self.roundNearest(value, self.currentStepsPerMM)
        self.StepLength_LineEdit.setText(value)

    def __updateStepModeComboBox(self, value):
        # print(value)
        if value == 0:
            self.currentStepsPerMM = self.controller.stepsPerMM / 4
        elif value == 1:
            self.currentStepsPerMM = self.controller.stepsPerMM / 2
        elif value == 2:
            self.currentStepsPerMM = self.controller.stepsPerMM
        self.__updateStepLengthEditLine()

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

    def handleScanBetween(self):
        self.dc.addDataRow()
        self.controller.addDataRow()

        self.controller.handleScanBetween(self.P1_Slider.value(), self.P2_Slider.value(), self.SmpDuration_LineEdit.text(), self.StepLength_LineEdit.text(), self.StepMode_ComboBox.currentText())

    def toggleStop(self):
        self.controller.handlePause()
        if (self.controller.pause):
            self.Stop_Button.setText("Resume")
        else:
            self.Stop_Button.setText("Stop")

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
                newFile.write("Sample, Time, Position\n")
                for i in range(len(self.controller.samples)):
                    newFile.write("Line " + str(i + 1) + "\n")
                    for j in range(len(self.controller.samples[i])):
                        newFile.write(str(self.controller.samples[i][j]) + ", ")
                        newFile.write(str(self.controller.times[i][j])+ ", ")
                        newFile.write(str(self.controller.positions[i][j])+ ", ")
                        newFile.write('\n')
    
    def loadProgram(self):
        pass

    def clearGraph(self):
        self.controller.samples = [[]]
        self.controller.times = [[]]
        self.controller.positions = [[]]
        self.dc.currentRow = 0
        self.controller.currentRow = 0
        self.dc.resetSamples(self.controller.samples, self.controller.times, self.controller.positions)
        # self.dc.compute_initial_figure()
        # self.dc.update_figure()

    def closeEvent(self, ce):
        self.fileQuit()

    def roundNearest(self, value, a, prec=5):
        print(round(a * round(float(value)/a),prec))
        return str(max(round(a * round(float(value)/a),prec),round(a,5)))

    def SampleDurationUpdate(self):
        self.calculateExpectedDuration()
    
    def StepLengthUpdate(self):
        self.calculateExpectedDuration()

    def calculateExpectedDuration(self):
        stepLength = self.StepLength_LineEdit.text()
        if (stepLength == ""):
            return
        stepLength = float(stepLength)
        sampleDuration = float(self.SmpDuration_LineEdit.text())
        if (stepLength == "" or stepLength == 0 or sampleDuration == "" or sampleDuration == 0):
            return

        #sampleDuration = sampleDuration
        #stepLength = float(stepLength)
        startPosition = float(self.P1_SpinBox.value())
        endPosition = float(self.P2_SpinBox.value())

        distance = abs(startPosition - endPosition)

        expectedDuration = ((distance / stepLength) * sampleDuration * 1.65 ) + 0.22 * distance#* self.currentStepsPerMM) + (distance * 0.001 * self.currentStepsPerMM)

        self.ExpectedDuration_Label.setText("Expected Duration: " + str(expectedDuration))

