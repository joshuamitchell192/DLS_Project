# Make sure that we are using QT5
import sys
import os
import random
import matplotlib
from PyQt5 import QtCore, QtWidgets
import numpy as np
matplotlib.use('Qt5Agg')

from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure

class MplCanvas(FigureCanvas):

    """ Creates the matplotlib canvas to setup the basic figure base class so that it can be extended to be either static or dynamic.

    """

    def __init__(self, parent=None, samples=[], times=[], positions=[], width=5, height=4, dpi=100):

        """ Creates a matplot lib figure, subplot and links the data samples list.
            
        """
        fig = Figure(figsize=(width, height), dpi=dpi)
        self.axes = fig.add_subplot(111)
        self.axes2 = self.axes.twiny()
        self.samples = samples
        self.times = times
        self.positions = positions

        self.compute_initial_figure()

        FigureCanvas.__init__(self, fig)
        self.setParent(parent)

        FigureCanvas.setSizePolicy(self,
                                   QtWidgets.QSizePolicy.Expanding,
                                   QtWidgets.QSizePolicy.Expanding)
        FigureCanvas.updateGeometry(self)

    def compute_initial_figure(self):
        pass

class DynamicMplCanvas(MplCanvas):

    """ Creates a dynamic canvas that updates based on the timers' frequency, changing if there's new data within the samples list.

    """

    def __init__(self, *args, **kwargs):

        """ Initialises the basic canvas from the super class and setups of the timer for updating the graph

        """

        MplCanvas.__init__(self, *args, **kwargs)
        timer = QtCore.QTimer(self)
        timer.timeout.connect(self.update_figure)
        timer.start(100)

    def resetSamples(self, newSamples, newTimes, newPositions):

        """ Relinks the samples list for when the user clicks the clear samples button

            :param: newSamples - the samples list after it's been reinitialised to an empty list.

        """
        self.times = newTimes
        self.samples = newSamples
        self.positions = newPositions

    def compute_initial_figure(self):

        """ Plots the initial list when the DynamicMPLCanvas object is created.

        """
        self.axes.plot(self.samples, 'r')

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


    def update_figure(self):

        """ Replots the samples list when the timer reaches zero, updating it on the GUI.

        """
        # Build a list of 4 random integers between 0 and 10 (both inclusive)
        # l = [random.randint(0, 10) for i in range(4)]
        time_tick_locations = np.array([.1, .2, .3, .4, .5, .6, .7, .8, .9])
        position_tick_locations = np.array([0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9])

        self.axes.cla()
        # if (len(self.positions) > 0):
        #     self.axes.set_xlim([0,max(self.positions[-1], 10)])
        # else:
        #     self.axes.set_xlim([0,10])
        # self.axes.set_xticks(time_tick_locations)
        #print(self.positionIntervals(self.positions))
        self.axes.set_xticklabels(self.positionIntervals(self.positions))
        self.axes.plot(self.samples, 'r')
        self.axes.set_xlabel("Position")
        self.axes.set_ylabel("Sensor Value")

        self.draw()

class data:
    
    def __init__(self, x, y):
        self.x = x
        self.y = y
