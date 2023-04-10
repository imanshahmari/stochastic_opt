from helper_functions_checkpoint import *

from deap import base
from deap import creator
from deap import tools
from deap import algorithms
import random
import pickle
import shutil

IND_SIZE = len(positions[0]) - 1
ENDTIME = 11.5


creator.create("FitnessMax", base.Fitness, weights=(1.0,))
creator.create("Individual", list, fitness=creator.FitnessMax)

toolbox = base.Toolbox()

toolbox.register("individual", generate_individual, creator.Individual, IND_SIZE, ENDTIME)
toolbox.register("population", tools.initRepeat, list, toolbox.individual)
toolbox.register("evaluate", evaluate)



toolbox.decorate("evaluate", tools.DeltaPenalty(feasible, -1.0,distance_feasible))
#toolbox.decorate("evaluate", tools.DeltaPenalty(feasible, -0.2))
#toolbox.decorate("evaluate", tools.DeltaPenalty(check_positive, 0.0))
#toolbox.decorate("evaluate", tools.DeltaPenalty(check_acceleration, -0.1,distance_acceleration))
#toolbox.decorate("evaluate", tools.DeltaPenalty(check_acceleration, -0.1))


toolbox.register("mate", tools.cxTwoPoint)
#toolbox.register("mate", tools.cxOnePoint)
#toolbox.register("mate", tools.cxPartialyMatched)


toolbox.register("mutate", tools.mutGaussian,  mu=0.024, sigma=0.036, indpb=20 / 219)
toolbox.register("select", tools.selTournament,tournsize=3)
#toolbox.register("select", tools.selRoulette,fit_attr = 'fitness')



hof = tools.HallOfFame(5, similar=np.array_equal)

stats = tools.Statistics(lambda ind: ind.fitness.values)
stats.register("avg", np.mean)
stats.register("std", np.std)
stats.register("min", np.min)
stats.register("max", np.max)

population = toolbox.population(n=10000)             #A list of individuals.
mu = 10000                              #The number of individuals to select for the next generation.
lambda_ = 8000                            #The number of children to produce at each generation.
cxpb = 0.9                                           #The probability that an offspring is produced by crossover.
mutpb = 0.1                                        #The probability that an offspring is produced by mutation.
ngen = 100                                      #The number of generation.
#verbose = True#Whether or not to log the statistics.



#finPop,logBook =  algorithms.eaMuPlusLambda(population, toolbox, mu , lambda_, cxpb, mutpb, ngen, stats = stats, halloffame= hof)


algorithms.eaSimple(population, toolbox, cxpb=0.5, mutpb=0.2, ngen=50)


ind = np.array(hof.items[0])
draw_graphs(ind, 0.024)

print("hello")

post_process("vals.csv", 0.024, hof.items[0],positions,ENDTIME,True)

shutil.copyfile(r"C:\Users\SEIMSHA\Documents\MasterThesis\genetic-clean\vals.csv",
                "C:\\Users\\SEIMSHA\\Documents\\MasterThesis\\Exjobb2023\\SmartComponent\\JointToRapidProgram\\vals.csv")

#z = np.ones(100)

#z_mut = tools.mutGaussian(z,0.012,0.012,1.0)

"""
VIKTIGA PARAMETRAR: 
1)ratio mellan föräldrar/barn(antal barn viktigt)
2)tournmentsize mindre desto mer diverse
3)mer crossover desto bättre 

"""