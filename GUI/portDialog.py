from PyQt5.QtWidgets import (QDialog, QDialogButtonBox, QVBoxLayout, QLineEdit, QComboBox)
import serial.tools.list_ports as port_list

class PortDialog(QDialog):

    def __init__(self, *args, **kwargs):
        super(PortDialog, self).__init__(*args, **kwargs)

        self.setWindowTitle("Serial Port Dialog")
        self.setFixedSize(250, 100)

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



