#include "Individual.h"
#include <assert.h>
#include <iostream>
#include <typeinfo>
#include <vector>
#include <algorithm>
#include <numeric>



Individual::Individual(IndividualParameters passedParameters,int idNumber) {
    //constructor

    passedParameters.nrPositions;
    indParameters = passedParameters;
    chromLen = passedParameters.nrDeltaTs;
    id = idNumber;
    fitness = 0;

}


void Individual::InitilizeConstant() {
    //intilize chromosome with constant deltas
    //float deltaT = 0.012;
    float deltaT = GenerateRandomFloat(0.0, 0.5);
    for (int i = 0; i < indParameters.nrDeltaTs; i++) {
        chromosome.push_back(deltaT);
    }
}

void Individual::InitilizeRandom() {
    //intilize chromosome with random delatas which sum up to endTime


    // Create a random array of size (nrVariables - 1) with uniform distribution between 0.001 and endTime
    std::vector<float> randomArray(indParameters.nrDeltaTs - 1);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.001, indParameters.endTime);
    for (int i = 0; i < indParameters.nrDeltaTs - 1; ++i) {
        randomArray[i] = dis(gen);
    }

    // Sort the random array in ascending order
    std::sort(randomArray.begin(), randomArray.end());

    // Initialize ind array with zeros
    std::vector<float> ind(indParameters.nrDeltaTs, 0.0);

    // Set the first and last values of ind
    ind[0] = randomArray[0];
    ind[indParameters.nrDeltaTs - 1] = indParameters.endTime - randomArray[indParameters.nrDeltaTs - 2];

    // Compute the remaining values of ind
    for (int i = 1; i < indParameters.nrDeltaTs - 1; ++i) {
        ind[i] = randomArray[i] - randomArray[i - 1];
    }
    std::vector<float> indVector(std::begin(ind), std::end(ind));
    chromosome = indVector;

}


void Individual::Derivative() {
    std::vector<std::vector<float>> velocities(indParameters.nrPositions, std::vector<float>(indParameters.nrJoints));
    std::vector<std::vector<float>> accelerations(indParameters.nrPositions, std::vector<float>(indParameters.nrJoints));

    for (int i = 1, j = 0; i < indParameters.nrDeltaTs; i++, j++) {
        for (int k = 0; k < indParameters.nrJoints; k++) {
            velocities[i - 1][k] = (indParameters.posArray[i][k] - indParameters.posArray[i-1][k]) / chromosome[j];
        }
    }
    for (int i = 1, j = 0; i < indParameters.nrDeltaTs; i++, j++) {
        for (int k = 0; k < indParameters.nrJoints; k++) {
            accelerations[i - 1][k] = (velocities[i][k] - velocities[i-1][k]) / chromosome[j];
        }
        
    }
    mechUnits.velocities = velocities;
    mechUnits.accelerations = accelerations;
}



void Individual::EvaluateIndividual() {
    Derivative();
    float sum_psy = 0.0;
    for (int i = 0; i < chromLen; i++) {
        float psi_squared_norm = 0.0;
        for (int j = 0; j < indParameters.nrJoints; j++) {
            psi_squared_norm += pow(mechUnits.accelerations[i][j], 2);
        }
        sum_psy += psi_squared_norm * chromosome[i];
    }
    auto result = std::accumulate(chromosome.begin(), chromosome.end(), 0.0);
    fitness = 1.0 / sum_psy;
    if (result > indParameters.endTime) {
        fitness = fitness - 1;
    }
}


bool Individual::operator<(const Individual& other) const {
    return fitness < other.fitness;
}

bool Individual::operator==(const Individual& other) const {
    return fitness == other.fitness;
}
bool Individual::operator>(const Individual& other) const {
    return fitness > other.fitness;
}

/*
float Individual::GenerateRandomFloat(float lowerBound, float upperBound) {
    
    static std::random_device rand_dev;
    static std::mt19937 generator(rand_dev());
    std::uniform_real_distribution<float> distr(lowerBound, upperBound);
    float randomNumber = distr(generator);
    
    return randomNumber;
    
    return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (upperBound - lowerBound) + lowerBound;
}
*/

/*
Individual::~Individual() {
    delete[] chromosome;
}
*/

