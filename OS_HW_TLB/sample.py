import matplotlib.pyplot as plt
import matplotlib.legend as legend
index=[0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0]
 
GABDR=[0, 0.51, 0.37, 0.87, 0.41, 0.33, 0.56, 0.65]
gab,=plt.plot(index, GABDR, 'ro-')
 
RABDR=[0.06, 0.39, 0.53, 0.43, 0.29, 0.42, 0.41, 0.34]
rab,=plt.plot(index, RABDR, 'g^-')
 
DABDR=[0.39, 0.84, 0.82, 0.84, 0.90, 0.54, 0.62, 0.66]
dab,=plt.plot(index, DABDR, 'bs-')
 
plt.legend([dab, rab, gab],['DAB','RAB','RAB'], loc=3)
plt.xlabel('FPR of Stage')
plt.ylabel('False Positive Rate')
plt.axis([-0.05,0.75, 0.0, 1.05])
plt.grid(True)
plt.show()
plt.savefig('wow.png')

