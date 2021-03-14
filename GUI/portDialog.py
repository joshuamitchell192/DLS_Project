from PyQt5.QtWidgets import (QDialog, QDialogButtonBox, QVBoxLayout, QLineEdit, QComboBox, QPushButton)
from PyQt5 import QtWidgets

import serial.tools.list_ports as port_list

class PortDialog(QDialog):

    def __init__(self, *args, **kwargs):
        super(PortDialog, self).__init__(*args, **kwargs)
        
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

        self.buttonBox = QDialogButtonBox(QDialogButtonBox.Save)
        self.buttonBox.accepted.connect(self.accept)

        self.layout = QVBoxLayout()
        self.layout.addWidget(self.PortList)
        self.layout.addWidget(self.buttonBox)
        self.setLayout(self.layout)



