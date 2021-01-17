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

class MplCanvas(FigureCanvas):

    """ Creates the matplotlib canvas to setup the basic figure base class so that it can be extended to be either static or dynamic.

    """

    def __init__(self, parent=None, samples=[], times=[], positions=[], width=5, height=4, dpi=90):

        """ Creates a matplot lib figure, subplot and links the data samples list.

        """
        #self.fig = Figure(figsize=(width, height), dpi=dpi)
        self.fig = figure.Figure(figsize=(width, height), dpi=dpi)
        self.ax = self.fig.add_subplot(111)
        self.setCanvasAxes()
        # self.background = self.fig.canvas.copy_from_bbox(self.ax.bbox)
        self.currentRow = 0
        self.lines = [self.ax.plot(0,0)[0]]

        self.samples = samples
        self.times = times
        self.positions = positions
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
        self.ax.set_ylim(100,4000)
        
        self.ax.grid(color='darkgrey', linestyle='dashed', axis='both', linewidth=1)
        self.ax.set_xlabel("Position")
        self.ax.set_ylabel ("Sensor Value")
        self.initialDrawEvent =  False

        self.fig.canvas.draw()
        # self.fig.canvas.mpl_connect('draw_event', self.sentinel)

    def sentinel(self, event):
        self.initialDrawEvent = True


    def compute_initial_figure(self):
        pass



class DynamicMplCanvasPerformance(QtWidgets.QWidget):

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
        self.lines = [self.canvas.ax.plot(0,0, color=self.canvas.colourMap[i % self.canvas.noColours], label="Line")[0] for i in range(self.canvas.maxLineCount)]
        anim = animation.FuncAnimation(self.canvas.fig, self.animate, init_func=self.init,
                                interval=16, blit=True)
        # while (not self.canvas.initialDrawEvent):
        #     pass
        # self.startTimer()
        
        # Timer required to delay between drawing the graph and updating to finish drawing
        # self.delayDrawTimer = QtCore.QTimer(self)
        # self.delayDrawTimer.timeout. connect(self.startTimer)
        # self.delayDrawTimer.start(300)
        
    def startTimer(self):
        self.delayDrawTimer.stop()
        timer = QtCore.QTimer(self)
        timer.timeout. connect(self.callback)
        timer.start(32)

    def addLine(self):
        
        # self.canvas.positions.append([])
        # self.canvas.times.append([])
        # self.canvas.samples.append([])

        self.currentRow = self.currentRow + 1


    def callback(self):


        if (len(self.canvas.positions) > 1):
            self.lines[self.currentRow].set_data(self.canvas.positions[self.currentRow ], self.canvas.samples[self.currentRow])
            
        # Restore the background
        # self.fig.canvas.restore_region(self.background)
        # draw the point on the screen
        self.canvas.ax.draw_artist(self.lines[self.currentRow])
        # blit the axes
        self.canvas.fig.canvas.blit(self.canvas.ax.bbox)

        self.xdata = self.xdata + 1
        self.ydata = self.ydata + 0.1

    def resetSamples(self, newSamples, newTimes, newPositions):

        """ Relinks the samples list for when the user clicks the clear samples button

            :param: newSamples - the samples list after it's been reinitialised to an empty list.

        """
        self.canvas.times = newTimes
        self.canvas.samples = newSamples
        self.canvas.positions = newPositions
        
        self.canvas.ax.clear()
        self.canvas.setCanvasAxes()

    # def compute_initial_figure(self):

    #     """ Plots the initial list when the DynamicMPLCanvas object is created.

    #     """
    #     self.lines.append(self.ax.plot(self.positions[self.currentRow], self.samples[self.currentRow]))
    #     # self.axes.append(plt.plot(self.positions[self.currentRow], self.samples[self.currentRow], color=self.colourMap[self.currentRow % self.noColours], label=f'Scan {self.currentRow + 1}', animated=True))

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
        # self.positions.append([])
        # self.times.append([])
        # self.samples.append([])

        self.canvas.currentRow = self.canvas.currentRow + 1
        self.canvas.lines.append(self.canvas.ax.plot([], [], animated=True)[0])
        # self.canvas.lines[self.canvas.currentRow].set_data([], [])
        # print(self.colourMap[self.currentRow % self.noColours])
        # print(self.currentRow % self.noColours)
        # print("adding data row")
        # print(self.positions, self.samples)
        # print(self.currentRow)
        # print(self.lines)
        #self.lines.append(plt.plot(self.positions[self.currentRow], self.samples[self.currentRow], animated=True)[0])
        # self.axes.append(plt.plot(self.positions[self.currentRow], self.samples[self.currentRow], color=self.colourMap[self.currentRow % self.noColours], label=f'Scan {self.currentRow + 1}', animated=True))


    def update_figure(self):

        """ Replots the samples list when the timer reaches zero, updating it on the GUI.

        """
        # Build a list of 4 random integers between 0 and 10 (both inclusive)
        self.axes[self.currentRow].clear()
        # self.axes[self.currentRow] = self.axes[self.currentRow].ydata(self.samples[self.currentRow])
        # self.axes[self.currentRow] = self.axes[self.currentRow].xdata(self.positions[self.currentRow])
        self.axes[self.currentRow] = plt.plot(self.positions[self.currentRow], self.samples[self.currentRow], color=self.colourMap[self.currentRow % self.noColours], label=f'Scan {self.currentRow + 1}')
        self.draw()
        # plt.cla()
        # plt.clf()
        # plt.close("all")

    def init_func(self):
        print("running init")

        for line in self.canvas.lines:
            line.set_data([], [])

        return self.canvas.lines

    def animate(self, i):
        #self.ax.clear()
        # print("running animate")

        # print(f'Samples : {self.samples} \n Positions : {self.positions}')
        # print(self.currentRow)
        # print(self.lines)

        # self.ax.clear()
        # plt.cla()
        # self.ax.clear()
        # plt.cla()
        # self.lines[self.currentRow] = plt.plot(self.positions[self.currentRow], self.samples[self.currentRow], color=self.colourMap[self.currentRow % self.noColours])[0]
        # self.draw()



        # plt.grid(color='darkgrey', linestyle='dashed', linewidth=1)
        # plt.xlim((0, 65))
        # plt.xlabel("Position")
        # plt.ylabel("Sensor Value")
        # plt.pause(1)
        # plt.legend(loc='upper right')
        # self.lines[self.currentRow].clear()
        # print(self.positions[self.currentRow])
        print(f'Lines : {self.canvas.lines}')
        print(f'Lines Data : {self.canvas.lines[self.canvas.currentRow].get_ydata()}')
        print(f'Samples : {self.canvas.currentRow} - {self.canvas.samples}')
        print(f'Positions : {self.canvas.currentRow} - {self.canvas.positions}')
        self.canvas.lines[self.canvas.currentRow].set_data(self.canvas.positions[self.canvas.currentRow], self.canvas.samples[self.canvas.currentRow])
        # plt.plot(self.positions[self.currentRow], self.samples[self.currentRow])
        # self.ax.draw
        # self.canvas.draw()
        # self.lines[self.currentRow].set_ydata(self.samples[self.currentRow])
        return self.canvas.lines

