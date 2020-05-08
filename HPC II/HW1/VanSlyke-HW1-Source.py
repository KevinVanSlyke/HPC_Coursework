# -*- coding: utf-8 -*-
"""
Created on Sun Jan 31 14:48:30 2016

@author: Kevin
"""
import numpy
import time
import math
import random
import matplotlib.pyplot as plt
from lmfit import Model

def createF(N):
    F = numpy.asarray([[0. for x in range(N)] for x in range(N)])
    for i in range(N):
         for j in range(N):
             F[i][j] = random.random()
    return F
def createT(N):
    T = numpy.asarray([[0. for x in range(N)] for x in range(N)])
    for i in range(N):
         for j in range(N):
             T[i][j] = random.random()
    return T

def simultaneous_mult(A,B):
    N = len(A)
    C = numpy.asarray([[0. for x in range(N)] for x in range(N)])
    for i in range(N):
        for j in range(N):
            part = 0.
            for p in range(N):
                for q in range(N):
                    part = part + A[p][q]*B[p][i]*B[q][j]
            C[i][j]= part
    return C

def sequential_mult(A,B):
    N = len(A)
    C = numpy.asarray([[0. for x in range(N)] for x in range(N)])
    D = numpy.asarray([[0. for x in range(N)] for x in range(N)])
    for j in range(N):
        for p in range(N):
            part = 0.
            for q in range(N):
                part = part + A[p][q]*B[q][j]
            D[p][j]= part
    for j in range(N):
        for i in range(N):
            part = 0.
            for p in range(N):
                part = part + B[p][i]*D[p][j]
            C[i][j]=part
    return C
               
def exp_func(x, a, b):
    return a+b*x

def line_ratios(c,d):
    s = numpy.asarray([0. for x in range(len(c))])
    for i in range(len(c)):
        s[i]=c[i]/d[i]
    return s
    
def conv_ln(vec):
    N = len(vec)
    ln_vec = numpy.asarray([0. for x in range(N)])
    for i in range(N):
        ln_vec[i] = math.log(vec[i])
    return ln_vec
    
random.seed()
sizes = [x for x in range(64,129, 4)]
N = len(sizes)
bTimes = [0. for x in range(64,129, 4)]
sTimes = [0. for x in range(64,129, 4)]
bTimes = numpy.asarray(bTimes)
sTimes = numpy.asarray(sTimes)
sizes=numpy.asarray(sizes)

count = 0
for i in sizes:
    F = createF(i)
    T = createT(i)
    sizes[count] = i
    
    #Product with simultaneous summation
    bstart = time.time()
    E = simultaneous_mult(F,T)
    bTime = time.time() - bstart
    if (bTime > 0.001):
        bTimes[count] = bTime
    else:
        bTimes[count] = .001
    #End simultaneous summation
        
    #Product with sequential summation
    sstart = time.time()
    G = sequential_mult(F,T)
    sTime = time.time() - sstart
    if (sTime > 0.001):
        sTimes[count] = sTime
    else:
        sTimes[count] = .001
    #End sequential summation
    count = count + 1
    
ratio_times = line_ratios(bTimes,sTimes)
ratio_times = conv_ln(ratio_times)
bTimes = conv_ln(bTimes)
sTimes = conv_ln(sTimes)
sizes = conv_ln(sizes)

model = Model(exp_func)

sim_pars = model.make_params(a = 0, b = 4)
sim_model_fit = model.fit(bTimes, sim_pars, x=sizes)
print(sim_model_fit.fit_report())
plt.figure()
sim_model_fit.plot_fit()
plt.xlabel('ln(N)')
plt.ylabel('ln(Time)')
plt.title('Matrix Transformation with simultaneous summation')

seq_pars = model.make_params(a = 0, b = 3)
seq_model_fit = model.fit(sTimes, seq_pars, x=sizes)
print(seq_model_fit.fit_report())
plt.figure()
seq_model_fit.plot_fit()
plt.xlabel('ln(N)')
plt.ylabel('ln(Time)')
plt.title('Matrix Transformation with sequential summation')

ratio_pars = model.make_params(a = 0, b = 1)
ratio_model_fit = model.fit(ratio_times, ratio_pars, x=sizes)
print(ratio_model_fit.fit_report())
plt.figure()
seq_model_fit.plot_fit()
plt.xlabel('ln(N)')
plt.ylabel('ln(Time Simultaneous / Time Sequential)')
plt.title('Scaling Comparison')