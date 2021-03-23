# Make sure that we are using QT5
import sys
import os
import random
import matplotlib
from PyQt5.QtWidgets import QVBoxLayout
from PyQt5 import QtCore, QtWidgets
import numpy as np
matplotlib.use('Qt5Agg')

from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
# from matplotlib.figure import Figure
from matplotlib import cm, animation, figure
import matplotlib.pyplot as plt
from numpy import linspace
import time
import matplotlib.colors as mcolors
from threading import Timer

from Models.sampleData import SampleData

class MplCanvas(FigureCanvas):

    """ Creates the matplotlib canvas to setup the basic figure base class so that it can be extended to be either static or dynamic.

    """

    def __init__(self, parent=None, width=5, height=4, dpi=90):

        """ Creates a matplot lib figure, subplot and links the data samples list.

        """
        self.fig = figure.Figure(figsize=(width, height), dpi=dpi)
        self.ax = self.fig.add_subplot(111)
        self.setCanvasAxes()

        self.noColours = 40
        self.noColourRepitions = 3
        self.maxLineCount = self.noColours * self.noColourRepitions
        
        FigureCanvas.__init__(self, self.fig)

        self.createColourMap()
        self.setParent(parent)

        FigureCanvas.setSizePolicy(self,
                                   QtWidgets.QSizePolicy.Expanding,
                                   QtWidgets.QSizePolicy.Expanding)
        FigureCanvas.updateGeometry(self)

    def setCanvasAxes(self):

        self.ax.set_xlim((0, 10))
        self.ax.set_ylim(0, 10)
        
        self.ax.grid(color='darkgrey', linestyle='dashed', axis='both', linewidth=1)
        self.ax.set_xlabel("Position")
        self.ax.set_ylabel ("Sensor Value")

        self.fig.canvas.draw()

    def createColourMap(self):

        cm_subsection = linspace(0.0, 1.0, self.noColours)

        tabbMap = [ cm.tab20b(x) for x in cm_subsection ]
        tabcMap = [ cm.tab20c(x) for x in cm_subsection ]

        colours = (tabbMap + tabcMap) 
        random.shuffle(colours)

        self.colourMap = colours

        for i in range(self.noColourRepitions):
            random.shuffle(colours)
            self.colourMap += colours



class DynamicMplCanvas(QtWidgets.QWidget):

    """ Creates a dynamic canvas that updates based on the timers' frequency, changing if there's new data within the samples list.

    """

    def __init__(self, sampleData, parent=None, width=5, height=4, dpi=90):

        """ Initialises the basic canvas from the super class and setups of the timer for updating the graph

        """
        QtWidgets.QMainWindow.__init__(self)
        vbox = QVBoxLayout()
        self.canvas = MplCanvas(parent=parent, width=width, height=height, dpi=dpi)
        self.sampleData = sampleData

        self.xAxisRange = AxisRange()
        self.yAxisRange = AxisRange()

        vbox.addWidget(self.canvas)
        self.setLayout(vbox)
        self.lines = [self.canvas.ax.plot(0, 0, color=self.canvas.colourMap[i % self.canvas.noColours], label="Line")[0] for i in range(self.canvas.maxLineCount)]
        self.scatterPlot = self.canvas.ax.plot(-10, -10, color='black', linestyle = 'None', marker='o', markersize=5)[0]

        self.startTimer()

        self.dataExists = False

    def startTimer(self):
        timer = QtCore.QTimer(self)
        timer.timeout. connect(self.callback)
        timer.start(64)

    def callback(self):

        try:
            if (self.sampleData.linePlotData.getLineIndex() >= 0):
                self.dataExists = True
                self.lines[self.sampleData.linePlotData.getLineIndex()].set_data(self.sampleData.linePlotData.positions[self.sampleData.linePlotData.getLineIndex()], self.sampleData.linePlotData.samples[self.sampleData.linePlotData.getLineIndex()])

            if (self.sampleData.scatterPlotData.getSampleCount() > 0):
                self.dataExists = True
                self.scatterPlot.set_data(self.sampleData.scatterPlotData.positions, self.sampleData.scatterPlotData.samples)


            if (self.dataExists):

                self.canvas.draw()
                self.computeXAxisLimits()
                self.computeYAxisLimits()

        except:
            pass

    def computeXAxisLimits(self):

        if (self.sampleData.scatterPlotData.getSampleCount() > 0):
            latestScatterPosition = self.sampleData.scatterPlotData.positions[-1]
            if (latestScatterPosition < self.xAxisRange.minValue):
                self.xAxisRange.setMinValue(latestScatterPosition - 2)

        lastRowMin = np.amin(self.sampleData.linePlotData.positions[-1])
        if (lastRowMin < self.xAxisRange.minValue):
            self.xAxisRange.setMinValue(lastRowMin - 2)

        if (self.sampleData.scatterPlotData.getSampleCount() > 0):
            latestScatterPosition = self.sampleData.scatterPlotData.positions[-1]
            if (latestScatterPosition > self.xAxisRange.maxValue):
                self.xAxisRange.maxValue = latestScatterPosition

        lastRowMax = np.amax(self.sampleData.linePlotData.positions[-1])
        if (lastRowMax > self.xAxisRange.maxValue):
            self.xAxisRange.maxValue = lastRowMax

        self.canvas.ax.set_xlim(self.xAxisRange.minValue, self.xAxisRange.maxValue + 2)

    def computeYAxisLimits(self):

        if (self.sampleData.scatterPlotData.getSampleCount() > 0):
            latestScatterSample = self.sampleData.scatterPlotData.samples[-1]
            if (latestScatterSample < self.yAxisRange.minValue):
                self.yAxisRange.setMinValue(latestScatterSample - 10)

        lastRowMin = np.amin(self.sampleData.linePlotData.samples[-1])
        if (lastRowMin < self.yAxisRange.minValue):
            self.yAxisRange.setMinValue(lastRowMin - 10)

        if (self.sampleData.scatterPlotData.getSampleCount() > 0):
            latestScatterSample = self.sampleData.scatterPlotData.samples[-1]
            if (latestScatterSample > self.yAxisRange.maxValue):
                self.yAxisRange.maxValue = latestScatterSample

        lastRowMax = np.amax(self.sampleData.linePlotData.samples[-1])
        if (lastRowMax > self.yAxisRange.maxValue):
            self.yAxisRange.maxValue = lastRowMax

        self.canvas.ax.set_ylim(self.yAxisRange.minValue, self.yAxisRange.maxValue + 10)

    def resetCanvas(self):

        """ Relinks the samples list for when the user clicks the clear samples button

            :param: newSamples - the samples list after it's been reinitialised to an empty list.

        """
        
        self.canvas.ax.clear()
        self.canvas.setCanvasAxes()
        self.lines = [self.canvas.ax.plot(0,0, color=self.canvas.colourMap[i % self.canvas.noColours], label="Line")[0] for i in range(self.canvas.maxLineCount)]
        self.scatterPlot = self.canvas.ax.plot(0, 0, color='black', linestyle = 'None', marker='o', markersize=5)[0]
        
        self.canvas.createColourMap()

class AxisRange:

    def __init__(self):
        self.minValue = 5000
        self.maxValue = 0
    
    def setMinValue(self, value):
        if (value < 0):
            self.minValue = 0
        else:
            self.minValue = value