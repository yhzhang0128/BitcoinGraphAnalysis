import subprocess
from time import perf_counter as pc

#days = 3
days = 30

f = open('result.log', 'w')
for i in range(3):
    for j in range(days):
        t0 = pc()
        result = subprocess.run(['./stat1', str(i + 1), str(j + 1)], stdout = subprocess.PIPE)
        t1 = pc()
        print("@data " + str(i + 1) + ":" + str(j + 1))
        print("@time " + ("%0.2f" % (t1 - t0)))
        print(result.stdout.decode('utf-8'))
        print()
        print()

        f.write("@data " + str(i + 1) + ":" + str(j + 1) + "\n")
        f.write("@time " + ("%0.2f" % (t1 - t0)) + "\n")
        f.write(result.stdout.decode('utf-8') + "\n")
        f.write("\n")
        f.write("\n")

f.close()
