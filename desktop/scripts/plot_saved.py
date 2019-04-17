import dataset
import datafreeze
from matplotlib import pyplot as plt

plot_db = True
plot_txt = False
output_csv = True

xs = []
ys = []



if plot_db:
    db = dataset.connect('sqlite:///day_run_9_18.db')
    if output_csv:
        result = db['T_115_fringes'].all()
        dataset.freeze(result, format='csv', filename='for_zack.csv')

    table = db['T_115_fringes']
    # plt.ion()
    query1 = "SELECT DISTINCT chirp FROM T_115_fringes"

    for row in db.query(query1):
        xs.append(row['chirp'])
        query2 = 'SELECT pd FROM T_115_fringes WHERE chirp = %f' % (row['chirp'])
        for pd in db.query(query2):
            try:
                ys[xs.index(row['chirp'])].append(pd['pd'])
            except IndexError:
                ys.append([pd['pd']])



if plot_txt:
    file = "T_115.txt"
    x, y = 0, 0
    for line in open(file, 'r'):
        l = line.strip()
        if l.startswith("rd"):
            _, x = l.split(":")
            x = float(x)
            xs.append(x)
        elif l.startswith("fr"):
            _, y = l.split(":")
            y = float(y)
            try:
                ys[xs.index(x)].append(y)
            except IndexError:
                ys.append([y])

for xe, ye in zip(xs, ys):
    plt.scatter([xe] * len(ye), ye)

plt.xlim(min(xs) - 0.00001, max(xs) + 0.00001)
plt.show()

while True:
    continue


if __name__ == '__main__':
    pass