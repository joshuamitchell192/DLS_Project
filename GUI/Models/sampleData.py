
class LinePlotData:

    def __init__(self):
        self.samples = []
        self.times = []
        self.positions = []

    def getLineIndex(self):
        return len(self.samples) - 1

    def addLine(self):
        self.positions.append([])
        self.times.append([])
        self.samples.append([])

    def clearData(self):
        self.positions = []
        self.times = []
        self.samples = []

class ScatterPlotData:

    def __init__(self):
        self.samples = []
        self.times = []
        self.positions = []
    
    def getSampleCount(self):
        return len(self.samples)
    
    def clearData(self):
        self.samples = []
        self.times = []
        self.positions = []

    def __repr__(self):
        return f'{self.samples}, {self.times}, {self.positions}'

class SampleData:

    def __init__(self):
        self.linePlotData = LinePlotData()
        self.scatterPlotData = ScatterPlotData()

    def clearAllData(self):
        self.linePlotData.clearData()
        self.scatterPlotData.clearData()
