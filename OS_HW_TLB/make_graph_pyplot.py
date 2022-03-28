import matplotlib.pyplot as plt
import matplotlib.legend as legend
import numpy as np
import sys



x=[]
y=[]
cnt=0

with open('./time.data') as file:
    for line in file.readlines():
        if cnt != 0:
            x.append(int(line.split()[0]))
            y.append(float(line.split()[1]))
        cnt = cnt + 1


plt.xlabel('Number Of Pages')
plt.ylabel('Time Per Access (ns)')


plt.plot(x.astype('str'), y)
# NUMPAGES =[1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192]

plt.margins(0)
plt.scatter(x,y,c='g',s=100)
plt.title("TLB Size Measurement",
        fontdict={ 'family': 'serif',
                    'color' : 'black' })
plt.grid(True)
plt.savefig('TLB_Measurement_graph')
