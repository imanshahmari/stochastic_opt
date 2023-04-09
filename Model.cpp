#include "Model.h"
#include <assert.h>
#include <iostream>
#include <typeinfo>
#include <vector>
#include <random>
#include <algorithm>
#include <set>
#include <time.h>

Model::Model(ModelParameters passedParameters) {
    //constructor 
    parameters = passedParameters;
}


void Model::InitilizePopulation() {
    for (int i = 0; i < parameters.populationSize; i++) {
        Individual ind(parameters.indParameters,i);
        float randomNumber = Individual::GenerateRandomFloat(0.0, 1.0);
        if (randomNumber < parameters.constantDeltasRatio) {
            ind.InitilizeConstant();
        }else {
            ind.InitilizeRandom();
        }
        ind.EvaluateIndividual();
        population.push_back(ind);
    }
}


int Model::TournementSelect() {
    //ROOT OF THE PROBLEM !!!!!!!!!!!!!!!
     
    //int* indexes = new int[parameters.tournamentSize + 1];

    //std::vector<int> = indexes(parameters.tournamentSize + 1);
    std::vector<Individual> tournamentIndivids;
    
    std::vector<int> indexes;
    for (int i = 0; i < parameters.tournamentSize; i++) {
        int index = rand() % parameters.populationSize;
        indexes.push_back(index);
        tournamentIndivids.push_back(population[index]);
    }
    

    //std::cout << tournamentIndivids[0].chromLen << std::endl;
    float randomNumber = Individual::GenerateRandomFloat(0.0,1.0);
    int lastInd = parameters.tournamentSize -1;
    //std::cout << "lastInd is ___________:" << lastInd << std::endl;
    float prob = parameters.tournamentProb * pow((1.0 - parameters.tournamentProb), lastInd);
    if (randomNumber > parameters.tournamentProb) {
        return tournamentIndivids[0].id;
    }
    std::sort(tournamentIndivids.begin(), tournamentIndivids.end());
    while (randomNumber >= prob) {
        //std::cout << "randomNumber is:" << randomNumber << std::endl;
        //std::cout << "prob is:" << prob << std::endl;
        lastInd--;
        prob = parameters.tournamentProb * pow((1.0 - parameters.tournamentProb), lastInd);
        
    }

    //std::cout << "breakpoint 1 reached" << indexes[0] << indexes[1] << std::endl;
    //std::sort(tournamentIndivids.begin(), tournamentIndivids.end());
    //std::cout << "breakpoint 2 reached" << std::endl;

    //delete[] indexes;
    int test = tournamentIndivids[lastInd].id;

    //std::cout << "lastInd is:" << test << std::endl;

    return tournamentIndivids[lastInd].id;
}


void Model::Cross(Individual& ind1, Individual& ind2) {

    int lengden = ind1.getLen();
    int crossIndex = rand() % ind1.getLen();
    //std::cout << "getLen gives:" << ind1.getLen() << std::endl;    
    //std::cout << "crossIndex is:" << crossIndex << std::endl;

    for (int i = 0; i < crossIndex;i++) {
        float tempValueInd1 = ind1.chromosome[i];
        float tempValueInd2 = ind2.chromosome[i];
        ind1.chromosome[i] = tempValueInd2;
        ind2.chromosome[i] = tempValueInd1;
    }
    
}

void Model::Mutate(Individual& ind) {
    for (int i = 0; i < ind.getLen(); i++) {
        float checkMutate = Individual::GenerateRandomFloat(0.0, 1.0);
        float checkCreep = Individual::GenerateRandomFloat(0.0, 1.0);
        float mutationNumber = Individual::GenerateRandomFloat(0.0, 0.1);
        if (checkMutate <= parameters.mutationProb) {
            if (checkCreep <= parameters.creepProba) {
                ind.chromosome[i] = ind.chromosome[i] - (parameters.creepRate / 2) + mutationNumber * parameters.creepRate;
            }
            else {
                ind.chromosome[i] = mutationNumber;

            }

        }

    }
}
