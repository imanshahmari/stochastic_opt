import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import random
import math


SWITCH = "ABB DATA"
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


def generate_individual(icls,nrVariables, endTime):
    chans = random.uniform(0, 1)
    ind = np.zeros(nrVariables)
    if chans < 0.01:
        randomArray = np.random.uniform(0.001, endTime, nrVariables - 1)
        randomArray = np.sort(randomArray)

        ind[0] = randomArray[0]
        ind[-1] = endTime - randomArray[-1]
        for i in range(1, nrVariables-1):
            ind[i] = randomArray[i] - randomArray[i-1]

    else:
        randomNumber = random.uniform(0, 1) / 20.0
        ind = [randomNumber for _ in range(0,len(ind))]


    indWrapped = icls(ind)

    return indWrapped



def evaluate(individual):

    # Do some hard computing on the individual
    joint_angles = positions
    velocities = np.zeros((joint_angles.shape[0], joint_angles.shape[1]))
    accelerations = np.zeros((joint_angles.shape[0], joint_angles.shape[1]))
    for i in range(1, len(individual)):
        velocity = (joint_angles[:, i] - joint_angles[:, i - 1]) / individual[i]
        velocities[:, i-1] = velocity
    for i in range(1, len(individual)):
        acceleration = (velocities[:, i] - velocities[:, i - 1]) / individual[i]
        accelerations[:, i-1] = acceleration


    sum_psy = 0
    for i in range(0, len(individual)):
        #psi = np.multiply(accelerations[:,i], velocities[:,i])
        psi = accelerations[:, i]
        sum_psy = sum_psy + np.matmul(psi, psi) * individual[i]

    # validList = []
    # for i in range(0,len(individual)):
    #     diff = abs(accelerations[:,i]) - MAX_ACCLERATIONS
    #     validIter = all(value < 0 for value in diff)
    #     validList.append(validIter)
    # valid = all(value == True for value in validList)
    # if valid == False:
    #     sum_psy = sum_psy * 10000
        #print("oops\n")

    return 1/sum_psy,


def mutCustom(individual,mu, sigma, indpb):
    """This function applies a gaussian mutation of mean *mu* and standard
    deviation *sigma* on the input individual. This mutation expects a
    :term:`sequence` individual composed of real valued attributes.
    The *indpb* argument is the probability of each attribute to be mutated.

    :param individual: Individual to be mutated.
    :param mu: Mean or :term:`python:sequence` of means for the
               gaussian addition mutation.
    :param sigma: Standard deviation or :term:`python:sequence` of
                  standard deviations for the gaussian addition mutation.
    :param indpb: Independent probability for each attribute to be mutated.
    :returns: A tuple of one individual.

    This function uses the :func:`~random.random` and :func:`~random.gauss`
    functions from the python base :mod:`random` module.
    """
    size = len(individual)
    for i, m, s in zip(range(size), mu, sigma):
        if random.random() < indpb:
            individual[i] += random.gauss(m, s)

    return individual,



def feasible(individual):
    """Feasibility function for the individual. Returns True if feasible False
    otherwise."""
    if sum(individual) <= ENDTIME:
        return True
    return False

def distance_feasible(individual):
    """Feasibility function for the individual. Returns True if feasible False
    otherwise."""
    distanceDiff = sum(individual) - ENDTIME
    return distanceDiff




def check_positive(individual):
    """Feasibility function for the individual. Returns True if feasible False
    otherwise."""
    is_positive = all(value > 0 for value in individual)

    if is_positive == True:
        return True
    return False




def check_acceleration(individual):
    """Feasibility function for the individual. Returns True if feasible False
    otherwise."""
    joint_angles = positions
    velocities = np.zeros((joint_angles.shape[0], joint_angles.shape[1]))
    accelerations = np.zeros((joint_angles.shape[0], joint_angles.shape[1]))
    for i in range(1, len(individual)):
        velocity = (joint_angles[:, i] - joint_angles[:, i - 1]) / individual[i]
        velocities[:, i-1] = velocity
    for i in range(1, len(individual)):
        acceleration = (velocities[:, i] - velocities[:, i - 1]) / individual[i]
        accelerations[:, i-1] = acceleration

    validList = []
    for i in range(0,len(individual)):
        diff = abs(accelerations[:,i]) - MAX_ACCLERATIONS
        validIter = all(value < 0 for value in diff)
        validList.append(validIter)
    valid = all(value == True for value in validList)

    #print("done")
    return valid



def distance_acceleration(individual):
    """Feasibility function for the individual. Returns True if feasible False
    otherwise."""
    joint_angles = positions
    velocities = np.zeros((joint_angles.shape[0], joint_angles.shape[1]))
    accelerations = np.zeros((joint_angles.shape[0], joint_angles.shape[1]))
    for i in range(1, len(individual)):
        velocity = (joint_angles[:, i] - joint_angles[:, i - 1]) / individual[i]
        velocities[:, i] = velocity
    for i in range(1, len(individual)):
        acceleration = (velocities[:, i] - velocities[:, i - 1]) / individual[i]
        accelerations[:, i] = acceleration


    delta = 0.012
    velocitiesValid = np.zeros((joint_angles.shape[0], joint_angles.shape[1]))
    accelerationsValid = np.zeros((joint_angles.shape[0], joint_angles.shape[1]))
    for i in range(1, len(individual)):
        velocityValid = (joint_angles[:, i] - joint_angles[:, i - 1]) / delta
        velocitiesValid[:, i-1] = velocityValid
    for i in range(1, len(individual)):
        accelerationvalid = (velocitiesValid[:, i] - velocitiesValid[:, i - 1]) / delta
        accelerationsValid[:, i-1] = accelerationvalid

    diffs = accelerationsValid - accelerations
    allDiffs = []

    for i in range(0,diffs.shape[1]):
        diffNormalized = np.linalg.norm(diffs[:,i])
        allDiffs.append(diffNormalized)

    distance = sum(allDiffs)

    return -1*(1/distance)



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



def derivative(deltaTList, jointAngles):
    velocities = np.zeros((jointAngles.shape[0], jointAngles.shape[1]))
    accelerations = np.zeros((jointAngles.shape[0], jointAngles.shape[1]))
    if isfloat(deltaTList):
        for i,j in zip(range(1, len(deltaTList)) , range(0,len(deltaTList))):
            velocity = (jointAngles[:, i] - jointAngles[:, i - 1]) / deltaTList[j]
            velocities[:, i-1] = velocity
        for i,j in zip(range(1, len(deltaTList)) , range(0,len(deltaTList))):
            acceleration = (velocities[:, i] - velocities[:, i - 1]) / deltaTList[j]
            accelerations[:, i-1] = acceleration
    else:
        for i in range(1, jointAngles.shape[1]):
            velocity = (jointAngles[:, i] - jointAngles[:, i - 1]) / deltaTList
            velocities[:, i-1] = velocity
        for i in range(1, jointAngles.shape[1]):
            acceleration = (velocities[:, i] - velocities[:, i - 1]) / deltaTList
            accelerations[:, i-1] = acceleration
    return velocities,accelerations



def post_process(fileName:str, delta:float, xInput,phis,endTime,checkGraphics):
    xPost = [i * delta for i in range(0, math.ceil(endTime/delta))]
    xPost.append(endTime)
    xOptimized = [0]
    for element in xInput:
        xOptimized.append(element + xOptimized[-1])

    jointValuesInterpolated = []
    for i in range(0, 6):
        jointValue = interpolate_linear(xPost, xOptimized, phis[i])
        jointValuesInterpolated.append(jointValue)


    if checkGraphics:
        pass

    jointValuesInterpolatedDegrees = np.array(jointValuesInterpolated).T.astype(float)
    jointValuesInterpolatedRadians = np.deg2rad(jointValuesInterpolatedDegrees)
    joint_vals = pd.DataFrame(jointValuesInterpolatedRadians)
    joint_vals.to_csv("./" + fileName, sep=',', index=False, float_format='%.5f', header=False)

def interpolate_linear(points_vector,x,y):
    result = []
    for element in points_vector:
        value = np.interp(element,x,y)
        result.append(value)

    result = np.array(result)
    return result



def isfloat(num):
    try:
        len(num)
        return True
    except:
        print("im here")
        return False