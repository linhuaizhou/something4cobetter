import random
import socket
import sys
from PyQt5.QtWidgets import QWidget, QApplication, QLabel, QVBoxLayout
from PyQt5.QtGui import QPainter, QColor, QFont, QPen, QBrush
from PyQt5.QtCore import Qt, QThread, pyqtSignal


def drawRectangle(qp, x, y, width, height, name):
    # 设置矩形的边框样式
    pen = qp.pen()
    pen.setColor(QColor(Qt.blue))
    pen.setWidth(2)
    qp.setPen(pen)

    # 画矩形
    qp.drawRect(x, y, width, height)
    font = QFont('Arial', 10)
    qp.setFont(font)
    text = name + ":" + str(width) + "×" + str(height)
    text_width = qp.fontMetrics().width(text)
    text_height = qp.fontMetrics().height()
    qp.drawText(x + width - text_width, y - text_height, text)


def drawEllipse(qp, x, y, radius, name):
    # 设置椭圆的边框样式
    pen = qp.pen()
    pen.setColor(QColor(Qt.black))
    pen.setWidth(2)
    qp.setPen(pen)

    # 画椭圆
    qp.drawEllipse(x - radius, y - radius, 2 * radius, 2 * radius)
    font = QFont('Arial', 10)
    qp.setFont(font)
    text = name + ":radius= " + str(radius) + "mm"
    text_width = qp.fontMetrics().width(text)
    text_height = qp.fontMetrics().height()
    qp.drawText(x + radius - text_width, y - radius, text)


class SocketListener(QThread):
    data_received = pyqtSignal(str)

    def __init__(self, host, port):
        super().__init__()
        self.host = host
        self.port = port

    def run(self):
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_socket.bind((self.host, self.port))
        server_socket.listen(1)

        while True:
            client_socket, client_address = server_socket.accept()
            self.data_received.emit(f"New connection from {client_address}")
            self.handle_client(client_socket)

    def handle_client(self, client_socket):
        while True:
            data = client_socket.recv(1024).decode("utf-8")
            if not data:
                break
            self.data_received.emit(data)

        client_socket.close()


class Example(QWidget):
    def __init__(self):
        super().__init__()
        self.label = QLabel("Waiting for data...", self)
        self.listener = SocketListener("127.0.0.1", 3000)
        self.initUI()

        self.shapes = []

    def initUI(self):
        self.setGeometry(0, 0, 1000, 1000)
        self.setWindowTitle('simple2dServer')

        self.label.setAlignment(Qt.AlignCenter)

        layout = QVBoxLayout(self)
        # layout.addWidget(self.label)
        layout.addWidget(self.label, 0, Qt.AlignTop)
        self.listener.data_received.connect(self.update_plot)
        self.listener.start()
        self.show()

    def update_plot(self, data):
        self.label.setText(f"Received data: {data}")
        params = data.split()
        qp = QPainter()

        qp.begin(self)
        qp.translate(self.width() / 2, self.height() / 2)
        default_line_color = Qt.black
        if params[0] == 'rectangle':
            self.shapes.append({'type': 'rectangle', 'x': int(params[1]), 'y': int(params[2]), 'width': int(params[3]),
                                'height': int(params[4]), "name": params[5], 'line_color': default_line_color})
            # self.draw_rectangle(qp, int(params[1]), int(params[2]), int(params[3]), int(params[4]), params[5])
        elif params[0] == 'ellipse':
            self.draw_ellipse(qp, int(params[1]), int(params[2]), int(params[3]), params[4])
            self.shapes.append({'type': 'ellipse', 'x': int(params[1]), 'y': int(params[2]), 'radius': int(params[3]),
                                'name': params[4], 'line_color': default_line_color})
        elif params[0] == "move":
            name = params[1]
            new_x = int(params[2])
            new_y = int(params[3])
            for shape in self.shapes:
                if shape['name'] == name:
                    shape['x'] += new_x
                    shape['y'] += new_y
        elif params[0] == "paint":
            name = params[1]
            color = params[2]
            for shape in self.shapes:
                if shape['name'] == name:
                    if color == "black":
                        shape['line_color'] = Qt.black
                    if color == "red":
                        shape['line_color'] = Qt.red

        # drawEllipse(qp, 0, 0, 50, "Ellipse")
        # drawRectangle(qp, 100, 100, 100, 100, "123")
        self.update()  # 强制重绘界面

    def paintEvent(self, event):  # 重载⽅法
        # qp = QPainter()

        # qp.begin(self)
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)
        painter.translate(self.width() / 2, self.height() / 2)

        for shape in self.shapes:
            line_color = shape.get('line_color', Qt.black)  # 获取线条颜色，如果未指定则使用默认颜色
            painter.setPen(QPen(line_color))
            if shape['type'] == 'rectangle':
                # painter.setBrush(QBrush(shape['color']) if shape['color'] != Qt.NoBrush else Qt.NoBrush)
                painter.drawRect(shape['x'], shape['y'], shape['width'], shape['height'])
                painter.drawText(shape['x'], shape['y'], shape['width'], shape['height'], Qt.AlignCenter,
                                 shape['name'] + ":" + str(shape['width']) + "×" + str(shape['height']))
            elif shape['type'] == 'ellipse':
                # painter.setBrush(QBrush(shape['color']) if shape['color'] != Qt.NoBrush else Qt.NoBrush)
                painter.drawEllipse(shape['x'], shape['y'], 2 * shape['radius'], 2 * shape['radius'])
                painter.drawText(shape['x'], shape['y'], 2 * shape['radius'], 2 * shape['radius'], Qt.AlignCenter,
                                 shape['name'] + ":radius= " + str(shape['radius']) + "mm")

    def draw_rectangle(self, qp, x, y, width, height, name):
        # 设置矩形的边框样式
        pen = qp.pen()
        pen.setColor(QColor(Qt.blue))
        pen.setWidth(2)
        qp.setPen(pen)

        # 画矩形
        qp.drawRect(x, y, width, height)
        font = QFont('Arial', 10)
        qp.setFont(font)
        text = name + ":" + str(width) + "×" + str(height)
        text_width = qp.fontMetrics().width(text)
        text_height = qp.fontMetrics().height()
        qp.drawText(x + width - text_width, y - text_height, text)

    def draw_ellipse(self, qp, x, y, radius, name):
        # 设置椭圆的边框样式
        pen = qp.pen()
        pen.setColor(QColor(Qt.black))
        pen.setWidth(2)
        qp.setPen(pen)

        # 画椭圆
        qp.drawEllipse(x - radius, y - radius, 2 * radius, 2 * radius)
        font = QFont('Arial', 10)
        qp.setFont(font)
        text = name + ":radius= " + str(radius) + "mm"
        text_width = qp.fontMetrics().width(text)
        text_height = qp.fontMetrics().height()
        qp.drawText(x + radius - text_width, y - radius, text)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())
