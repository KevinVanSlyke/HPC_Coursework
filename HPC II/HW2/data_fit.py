import math
import matplotlib.pyplot as plt
import os
import numpy
from lmfit import Model

def read_times():
    try: 
        file = open( "output.txt", 'r')
        lines = file.readlines()
        #print lines
        count = 1
        x = []
        y = []
        N = []
        for line in lines :
            if (line != '\n' and line[0] != "#" and line[0] != 'P') :
                words = line.split()
                fast, node = [float(s) for s in words]
                x.append(fast)
                y.append(node)
                N.append(count)
                count = count + 1
        return x, y, N
    except:
        print 'Error in read_times. Returning None.'
        return None
                
def simple_func(x, b, a):
    return a+b*(x)

    
fast_times, slow_times, N = read_times()

time_model = Model(simple_func)
time_pars = time_model.make_params(a = 0, b = 0)
fast_model_fit = time_model.fit(fast_times, time_pars, x=N)
print(fast_model_fit.fit_report())
plt.figure()
fast_model_fit.plot_fit()
plt.xlabel('Message size as Vector < int > length N')
plt.ylabel('Time')
plt.title('Same node latency')


slow_model_fit = time_model.fit(slow_times, time_pars, x=N)
print(slow_model_fit.fit_report())
plt.figure()
slow_model_fit.plot_fit()
plt.xlabel('Message size as Vector < int > length N')
plt.ylabel('Time')
plt.title('Different node latency')