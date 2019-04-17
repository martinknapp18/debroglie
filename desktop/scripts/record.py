import serial
import dataset
import datetime

db = dataset.connect('sqlite:///t_130.db')

table = db['16_fringes']

stream = serial.Serial('COM5', 112500)

# num_pts_fringe = 16
#
# from matplotlib import pyplot as plt
# plt.xlim(22.993603 - 0.0000001, 22.993760 + 0.0000001)
# plt.ion()
# plt.show()

def values():
    x = 0
    y = 0
    while True:
        l = stream.readline()
        l = l.strip()
        if l.startswith("rd"):
            _, x = l.split(":")
        elif l.startswith("fr"):
            _, y = l.split(":")
            yield (float(x), float(y))
        print l

if __name__ == '__main__':
    data = values()
    while True:
        x, y = data.next()
        table.insert(dict(chirp=x, pd=y, timestamp=datetime.datetime.utcnow()))
        db.commit()
        # plt.scatter(x, y, color='r', marker='.')
        #
        # plt.draw()
        # plt.pause(0.001)
        # xs, ys = [], []

