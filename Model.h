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
    
    Model(ModelParameters passedParameters);
    
    ModelParameters parameters;
    std::string modelType;

    std::vector<float> bestChromosomes;


    float maximumFitnessInCurrentGeneration;
    int bestIndividualId;

    std::vector<Individual> population;

    void InitilizePopulation();

    void Cross(Individual& ind1, Individual& ind2);
    void Mutate(Individual& ind);
    int TournementSelect();
    void Elitisim();



    //~Individual();
};


#endif