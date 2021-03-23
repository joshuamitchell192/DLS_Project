import configparser, os

class Settings:

    def __init__(self):
        self.sampleDuration = 0.01
        self.mmBetweenSamples = 0.1
        self.startPosition = 0
        self.endPosition = 10
        self.averageInterval = 5
        self.port = ""
        self.terminalMode = False

        self.stepsPerMm = 99
        self.usePerformanceGraph = False
        self.xAxisRange = (0, 65)
        self.yAxisRange = (0, 4000)

        self.absoluteInstallationPath = os.path.dirname(os.path.abspath(__file__))

    def loadSettings(self):
        config = configparser.ConfigParser()
        config.read(self.absoluteInstallationPath + '/settings.ini')

        if ('Default' in config):

            defaultSettings = config['Default']
            if ("SampleDuration" in defaultSettings):
                self.sampleDuration = float(defaultSettings['SampleDuration'])
            if ("mmBetweenSamples" in defaultSettings):
                self.mmBetweenSamples = float(defaultSettings['mmBetweenSamples'])
            if ('startPosition' in defaultSettings):
                self.startPosition = int(defaultSettings['startPosition'])
            if ("endPosition" in defaultSettings):
                self.endPosition = defaultSettings['endPosition']
            if ("averageInterval" in defaultSettings):
                self.averageInterval = defaultSettings
            if ("port" in defaultSettings):
                self.port = defaultSettings["port"]
            if ("terminalMode" in defaultSettings):
                self.terminalMode = defaultSettings["terminalMode"]

            calibrationSettings = config['Calibration']
            if ("stepsPerMm" in calibrationSettings):
                self.stepsPerMm = calibrationSettings['stepsPerMm']

            canvasSettings = config["Canvas"]
            if ("usePerformanceGraph" in canvasSettings):
                self.usePerformanceGraph = canvasSettings["usePerformanceGraph"]
            if ("xaxisrange" in canvasSettings):
                self.xAxisRange = canvasSettings["xaxisrange"]
            if ("yaxisrange" in canvasSettings):
                self.yAxisRange = canvasSettings["yaxisrange"]

            # if ("SampleDuration" in defaultControlSettings):
            #     self.sampleDuration = defaultSettings['SampleDuration'])
            # else:
            #     print("Failed to load 'SampleDuration' from 'Default' section from settings")

            # if ("mmBetweenSamples" in defaultSettings):
            #     self.view.StepLength_LineEdit.setText(defaultSettings['mmBetweenSamples'])
            
            # if ("StartPosition" in defaultSettings):
            #     self.view.P1_Slider.setValue(int(defaultSettings["StartPosition"]))
            #     self.view.P1_SpinBox.setValue(int(defaultSettings["StartPosition"]))

            # if ("EndPosition" in defaultSettings):
            #     self.view.P2_Slider.setValue(int(defaultSettings["EndPosition"]))
            #     self.view.P2_SpinBox.setValue(int(defaultSettings["EndPosition"]))

            # if ("AverageInterval" in defaultSettings):
            #     self.view.AvgInterval_SpinBox.setValue(int(defaultSettings["AverageInterval"]))

    @staticmethod
    def saveSetting(section, field, value):
        config = configparser.ConfigParser()
        config.read(os.getcwd() + '/GUI/settings.ini')

        config[section][field] = value

        with open(os.getcwd() + '/GUI/settings.ini', 'w') as configFile:
            config.write(configFile)