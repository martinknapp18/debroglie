from matplotlib import pyplot as plt

def values():
   with open('super_nice_spectroscopy.txt') as f:
       for l in f:
           if l.startswith("rd"):
               _, x = l.split(":")
           elif l.startswith("fr"):
               _, y = l.split(":")
               yield (float(x), float(y))

xs = []
ys = []
data = values()
while True:
    try:
        x, y = data.next()
        if x not in xs:
            xs.append(x)
            ys.append([y])
        else:
            ys[xs.index(x)].append(y)
    except:
        break
for xe, ye in zip(xs, ys):
    plt.scatter([xe] * len(ye), ye)
plt.show()
