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
        self.lines = [self.ax.plot(0,0, marker='o')[0]]
        self.scatterPlot = self.ax.plot([0],[0], color='black', linestyle = 'None', marker='o', markersize=5)[0]

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

        self.compute_initial_figure()

        FigureCanvas.__init__(self, self.fig)
        self.setParent(parent)

        FigureCanvas.setSizePolicy(self,
                                   QtWidgets.QSizePolicy.Expanding,
                                   QtWidgets.QSizePolicy.Expanding)
        FigureCanvas.updateGeometry(self)

    def setCanvasAxes(self):

        self.ax.set_xlim((0, 65))
        self.ax.set_ylim(100,4000)
        
        self.ax.grid(color='darkgrey', linestyle='dashed', axis='both', linewidth=1)
        self.ax.set_xlabel("Position")
        self.ax.set_ylabel ("Sensor Value")

        self.fig.canvas.draw()

    def compute_initial_figure(self):
        pass



class DynamicMplCanvasPerformance(QtWidgets.QWidget):

    """ Creates a dynamic canvas that updates based on the timers' frequency, changing if there's new data within the samples list.

    """

    def __init__(self, sampleData, parent=None, width=5, height=4, dpi=90):

        """ Initialises the basic canvas from the super class and setups of the timer for updating the graph

        """
        QtWidgets.QMainWindow.__init__(self)
        vbox = QVBoxLayout()
        self.canvas = MplCanvas(parent=parent, width=width, height=height, dpi=dpi)
        self.sampleData = sampleData

        vbox.addWidget(self.canvas)
        self.setLayout(vbox)
        self.lines = [self.canvas.ax.plot(0,0, color=self.canvas.colourMap[i % self.canvas.noColours], label="Line")[0] for i in range(self.canvas.maxLineCount)]
        self.scatterPlot = self.canvas.ax.plot(0, 0, color='black', linestyle = 'None', marker='o', markersize=5)[0]

        self.startTimer()

    def startTimer(self):
        timer = QtCore.QTimer(self)
        timer.timeout. connect(self.callback)
        timer.start(32)

    def callback(self):

        try:
            if (self.sampleData.linePlotData.getLineIndex() > 0):

                self.lines[self.sampleData.linePlotData.getLineIndex()].set_data(self.sampleData.linePlotData.positions[self.sampleData.linePlotData.getLineIndex()], self.sampleData.linePlotData.samples[self.sampleData.linePlotData.getLineIndex()])
                self.canvas.ax.draw_artist(self.lines[self.sampleData.linePlotData.getLineIndex()])

            if (self.sampleData.scatterPlotData.getSampleCount() > 0):

                self.scatterPlot.set_data(self.sampleData.scatterPlotData.positions, self.sampleData.scatterPlotData.samples)
                self.canvas.ax.draw_artist(self.scatterPlot)

            # blit the axes
            self.canvas.fig.canvas.blit(self.canvas.ax.bbox)

        except:
            pass

    def resetCanvas(self):

        """ Relinks the samples list for when the user clicks the clear samples button

            :param: newSamples - the samples list after it's been reinitialised to an empty list.

        """
        
        self.canvas.ax.clear()
        self.canvas.setCanvasAxes()

