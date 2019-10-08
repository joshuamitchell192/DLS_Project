# Make sure that we are using QT5
import sys
import os
import random
import matplotlib
from PyQt5 import QtCore, QtWidgets
matplotlib.use('Qt5Agg')

from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure

class MplCanvas(FigureCanvas):

    """ Creates the matplotlib canvas to setup the basic figure base class so that it can be extended to be either static or dynamic.

    """

    def __init__(self, parent=None, samples=[], width=5, height=4, dpi=100):

        """ Creates a matplot lib figure, subplot and links the data samples list.
            
        """
        fig = Figure(figsize=(width, height), dpi=dpi)
        self.axes = fig.add_subplot(111)
        self.samples = samples

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

    def resetSamples(self, newSamples):

        """ Relinks the samples list for when the user clicks the clear samples button

            :param: newSamples - the samples list after it's been reinitialised to an empty list.

        """

        self.samples = newSamples

    def compute_initial_figure(self):

        """ Plots the initial list when the DynamicMPLCanvas object is created.

        """
        self.axes.plot(self.samples, 'r')

    def update_figure(self):

        """ Replots the samples list when the timer reaches zero, updating it on the GUI.

        """
        # Build a list of 4 random integers between 0 and 10 (both inclusive)
        # l = [random.randint(0, 10) for i in range(4)]
        self.axes.cla()
        self.axes.plot(self.samples, 'r')
        self.draw()