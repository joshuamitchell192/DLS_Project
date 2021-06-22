import os
from pathlib import Path

import serial.tools.list_ports as port_list
from PyQt5.QtWidgets import QComboBox, QDialog, QDialogButtonBox, QVBoxLayout


class PortDialog(QDialog):

    def __init__(self, *args, **kwargs):
        super(PortDialog, self).__init__(*args, **kwargs)
        
        self.setWindowTitle("Serial Port")
        self.setFixedSize(250, 110)
        dirname = Path(os.path.dirname(os.path.abspath(__file__))).parent.absolute()
        ssFile = os.path.join(dirname, "stylesheet.qss")
        dropDownPath = os.path.join(dirname, "Assets/baseline_arrow_drop_down_black_18dp.png").replace('\\', '/')
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
            }
            QComboBox::down-arrow {
                image: url(""" + dropDownPath + """);
            }
            """

            
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
