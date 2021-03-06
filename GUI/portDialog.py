from PyQt5.QtWidgets import (QDialog, QDialogButtonBox, QVBoxLayout, QLineEdit, QComboBox, QPushButton)
from PyQt5 import QtWidgets

import serial.tools.list_ports as port_list

class PortDialog(QDialog):

    def __init__(self, *args, **kwargs):
        super(PortDialog, self).__init__(*args, **kwargs)
        
        fontName = "Roboto"
        primaryColour = "#3f51b5"
        # primaryColour = "#ff5722"
        primaryLightColour = "#757de8"
        # primaryLightColour = "#ff8a50"
        self.setWindowTitle("Serial Port")
        self.setFixedSize(250, 110)
        ssFile = "./GUI/stylesheet.qss"
        with open(ssFile) as fh:
            styleSheet = fh.read()
            styleSheet += """
            QPushButton {
                font-family: "Roboto";
                font-weight: medium;
                color: white;
                border-radius: 3px;
                background-color: #3f51b5;
                font-size: 10pt;
                
                height: 33px;
                width: 85px;
            }"""
            self.setStyleSheet(styleSheet)

        self.PortList = QComboBox()
        ports = list(port_list.comports())
        for port in ports:
            self.PortList.addItem(str(port.device))


        # self.saveButton = QPushButton()
        
        # self.saveButton.setGeometry(50, 50 , 85, 33)
        # self.saveButton.setText("Select")
        # # self.saveButton.setDefault(True)
        # self.buttonBox = QDialogButtonBox()
        # self.buttonBox.setGeometry(10, 50, 200, 20)
        # self.buttonBox.addButton(self.saveButton, QDialogButtonBox.AcceptRole)
        # self.buttonBox.accepted.connect(self.accept)

        self.buttonBox = QDialogButtonBox(QDialogButtonBox.Save)
        self.buttonBox.accepted.connect(self.accept)

        self.layout = QVBoxLayout()
        self.layout.addWidget(self.PortList)
        self.layout.addWidget(self.buttonBox)
        self.setLayout(self.layout)



