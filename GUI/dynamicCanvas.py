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

    def __init__(self, parent=None, sampleData=SampleData(), width=5, height=4, dpi=90):

        """ Creates a matplot lib figure, subplot and links the data samples list.

        """
        #self.fig = Figure(figsize=(width, height), dpi=dpi)
        self.fig = figure.Figure(figsize=(width, height), dpi=dpi)
        self.ax = self.fig.add_subplot(111)
        self.setCanvasAxes()
        # self.background = self.fig.canvas.copy_from_bbox(self.ax.bbox)
        self.currentRow = 0
        self.lines = [self.ax.plot(0,0)[0]]

        # self.samples = samples
        # self.times = times
        # self.positions = positions
        self.sampleData = sampleData
        self.noColours = 40
        self.maxLineCount = self.noColours * 3
        cm_subsection = linspace(0.0, 1.0, self.noColours)
        tabbMap = [ cm.tab20b(x) for x in cm_subsection ]
        tabcMap = [ cm.tab20c(x) for x in cm_subsection ]
        random.shuffle(tabbMap)
        random.shuffle(tabcMap)
        m1 = lambda m: tabbMap[m]
        m2 = lambda m: tabcMap[m]
        self.lineLabels = ["Line " + str(i) for i in range(self.maxLineCount)]
        self.colourMap = [m(i) for i in range(self.noColours) for m in (m1, m2)]
        print(self.colourMap)


        self.compute_initial_figure()

        FigureCanvas.__init__(self, self.fig)
        self.setParent(parent)

        FigureCanvas.setSizePolicy(self,
                                   QtWidgets.QSizePolicy.Expanding,
                                   QtWidgets.QSizePolicy.Expanding)
        FigureCanvas.updateGeometry(self)

    def setCanvasAxes(self):

        self.ax.set_xlim((0, 65))
        
        self.ax.grid(color='darkgrey', linestyle='dashed', axis='both', linewidth=1)
        self.ax.set_xlabel("Position")
        self.ax.set_ylabel ("Sensor Value")

        self.fig.canvas.draw()


    def compute_initial_figure(self):
        pass



class DynamicMplCanvas(QtWidgets.QWidget):

    """ Creates a dynamic canvas that updates based on the timers' frequency, changing if there's new data within the samples list.

    """

    def __init__(self, parent=None, samples=[], times=[], positions=[], width=5, height=4, dpi=90):

        """ Initialises the basic canvas from the super class and setups of the timer for updating the graph

        """
        QtWidgets.QMainWindow.__init__(self)
        vbox = QVBoxLayout()
        self.canvas = MplCanvas(parent=parent, samples=samples, times=times, positions=positions, width=width, height=height, dpi=dpi)
        vbox.addWidget(self.canvas)
        self.setLayout(vbox)
        self.currentRow = 0
        self.lines = [self.canvas.ax.plot([], [])[0]]

        timer = QtCore.QTimer(self)
        timer.timeout. connect(self.update_figure)
        timer.start(100)



    def resetSamples(self, newSamples, newTimes, newPositions):

        """ Relinks the samples list for when the user clicks the clear samples button

            :param: newSamples - the samples list after it's been reinitialised to an empty list.

        """
        self.canvas.times = newTimes
        self.canvas.samples = newSamples
        self.canvas.positions = newPositions
        
        self.canvas.ax.clear()
        self.canvas.setCanvasAxes()

    def compute_initial_figure(self):

        """ Plots the initial list when the DynamicMPLCanvas object is created.

        """
        self.lines.append(self.ax.plot(self.positions[self.currentRow], self.samples[self.currentRow]))
        # self.axes.append(plt.plot(self.positions[self.currentRow], self.samples[self.currentRow], color=self.colourMap[self.currentRow % self.noColours], label=f'Scan {self.currentRow + 1}', animated=True))

    def timeIntervals(self, time_tick_ints):
        timeInts = []
        if len(self.times) < 9:
            intNum = len(self.times)
        else:
            intNum = 9
        for t in range(intNum):
            timeInts.append(self.times[int(t*len(self.times)/intNum)])
        return timeInts


    def positionIntervals(self, position_tick_ints):
        positionInts = []
        if len(self.positions) < 9:
            intNum = len(self.positions)
        else:
            intNum = 9

        for pos in range(intNum):
            positionInts.append(self.positions[int(pos * len(self.positions) / intNum)])

        return positionInts

    def addDataRow(self):
        self.canvas.positions.append([])
        self.canvas.times.append([])
        self.canvas.samples.append([])
        # if (len(self.canvas.samples) > 1):
        self.currentRow = self.currentRow + 1
        self.lines.append(self.canvas.ax.plot([], [], animated=True, color=self.canvas.colourMap[self.currentRow % self.canvas.noColours])[0])

    def update_figure(self):
        """ Replots the samples list when the timer reaches zero, updating it on the GUI.

        """

        self.lines[self.currentRow] = self.canvas.ax.plot(self.canvas.positions[self.currentRow ], self.canvas.samples[self.currentRow], color=self.canvas.colourMap[self.currentRow % self.canvas.noColours])
        self.canvas.draw()
        return self.canvas.lines

