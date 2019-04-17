import serial
import signal
import sys
import time

# orig_stdout = sys.stdout
# filename = time.asctime(time.localtime(time.time())).replace(' ', '_').replace(':', '_') + '.txt'
# f = open(filename, 'w')
# sys.stdout = f

def signal_handler():
        print('You pressed Ctrl+C!')
        # TODO(bsm): Save data here


        # sys.stdout = orig_stdout
        # f.flush()
        # f.close()
        sys.exit(0)


stream = serial.Serial('COM5', 112500)

from matplotlib import pyplot as plt
plt.ion()
plt.show()

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

xs = set()

if __name__ == '__main__':
    try:
        xs = []
        ys = []
        data = values()
        while True:
            x, y = data.next()
            xs.append(x)
            plt.scatter(x, y)
            plt.xlim(min(xs) - 0.0000001, max(xs) + 0.0000001)
            plt.draw()
            plt.pause(0.005)
    except:
        signal_handler()


