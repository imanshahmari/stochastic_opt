#ifndef MODEL
#define MODEL

#include "Individual.h"
#include <array>
#include <vector>
#include <stdexcept>

struct ModelParameters {
    IndividualParameters indParameters;
    int populationSize;
    int nrGenerations;
    float crossOverProb;
    float creepProba;
    float creepRate;
    float tournamentProb;
    int tournamentSize;
    float constantDeltasRatio;

    float mutationProb;

};



//Individual class
class Model
{
public:

    ModelParameters parameters;
    std::string modelType;
    std::vector <std::string> hallOfFame;
    float maximumFitnessInCurrentGeneration;
    int bestIndividualId;

    std::vector<Individual> population;
    Model(std::string, ModelParameters passedParameters);

    void InitilizePopulation();

    void Cross(Individual& ind1, Individual& ind2);
    void Mutate(Individual& ind);
    int TournementSelect();



    //~Individual();
};


#endif