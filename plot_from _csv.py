import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import random
import math


SWITCH = "SARMAD"
POSITIONS = "NO STANDSTILL"
ENDTIME = 12.384
MAX_ACCLERATIONS = np.array([4485.0,2222.0,3752.0,140.6,4222.1,119.2])

JOINT_VALUE = []
ALL_JOINT_LIST = [JOINT_VALUE for i in range(7)]
if SWITCH == "ABB DATA":
    DELTA = 0.024
    my_data = pd.read_excel('Recording-reference_1_no_payload.xlsx', index_col=0)
    time_list = pd.Series.tolist(my_data.index)
    ALL_JOINT_LIST[0] = time_list
    for i in range(1,7):
        ALL_JOINT_LIST[i] = pd.Series.tolist(my_data["J%d" % i])


elif SWITCH == "SARMAD":
    DELTA = 0.012
    sarmad_data = pd.read_csv("sarmad.csv")
    for i in range(7):
        ALL_JOINT_LIST[i] = pd.Series.tolist(sarmad_data[str(i)])

ALL_PHIS_STANDSTILL = np.array(ALL_JOINT_LIST[1::])
samples  = np.array([(ALL_PHIS_STANDSTILL[:, i] - ALL_PHIS_STANDSTILL[:, i - 1]) for i in range(1, ALL_PHIS_STANDSTILL.shape[1])])
NO_STANDSTILL = np.array([ALL_PHIS_STANDSTILL[:, i] for i,element in enumerate(samples) if np.linalg.norm(element) > 0.1]).T


if POSITIONS == "NO STANDSTILL":
    positions = NO_STANDSTILL
elif POSITIONS == "STANDSTILL":
    positions = ALL_PHIS_STANDSTILL

def draw_graphs(individual, delta):


    xOriginal = [i * delta for i in range(0, len(individual)+1)]
    xOptimized = [0]
    for element in individual:
        xOptimized.append(element + xOptimized[-1])

    velocsNo, accsNo = derivative(0.012,positions)
    velocsOpt, accsOpt = derivative(individual,positions)
    #my_x = [max(abs(accsNo[i,:])) for i in range(0,accsNo.shape[0])]
    #print("done")


    plt.figure(1)
    for i in range(0, 6):
        plt.subplot(230 + i + 1)
        plt.plot(xOriginal, positions[i,:], label="no optimized")
        plt.legend(loc="upper left")
        plt.plot(xOptimized, positions[i,:], label="optimized")
        plt.legend(loc="upper left")

    plt.figure(2)
    for i in range(0, 6):
        plt.subplot(230 + i + 1)
        plt.plot(xOriginal, velocsNo[i,:], label="no optimized")
        plt.legend(loc="upper left")
        plt.plot(xOptimized, velocsOpt[i,:], label="optimized")
        plt.legend(loc="upper left")

    plt.figure(3)
    for i in range(0, 6):
        plt.subplot(230 + i + 1)
        plt.plot(xOriginal, accsNo[i,:], label="no optimized")
        plt.legend(loc="upper left")
        plt.plot(xOptimized, accsOpt[i,:], label="optimized")
        plt.legend(loc="upper left")



