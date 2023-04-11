// genetic.cpp : Defines the entry point for the application.
//

#include "Individual.h"
#include "Model.h"

#include <algorithm>
#include <cmath>
#include <omp.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>


#define FILENAME "sarmad.csv"



int read_record(std::string, float**);





int main()
{


	float** positions;
	int rows = 1000;
	int cols = 6;

	// Allocate memory for positions array
	positions = new float* [rows];
	for (int i = 0; i < rows; i++) {
		positions[i] = new float[cols];
	}


	int nrPositions = read_record(FILENAME, positions);
	int nrDeltaTs = nrPositions - 1;

	std::cout << "number of DeltaTs:" << nrDeltaTs << std::endl;


	IndividualParameters indParameters;
	indParameters.nrDeltaTs = nrDeltaTs;
	indParameters.posArray = positions;
	indParameters.nrJoints = 6;
	indParameters.nrPositions = nrPositions;
	indParameters.endTime = 3.12;




	ModelParameters modelParameters;
	modelParameters.indParameters = indParameters;
	modelParameters.constantDeltasRatio = 1.0;
	modelParameters.populationSize = 50000;
	modelParameters.nrGenerations = 1000;
	modelParameters.crossOverProb = 0.9;
	modelParameters.mutationProb = 0.0001;
	modelParameters.creepProba = 0.8;
	modelParameters.creepRate = 0.008;
	modelParameters.tournamentProb = 0.75;
	modelParameters.tournamentSize = 3;
	

	//Initilize population

	Model model = Model(modelParameters);
	model.InitilizePopulation();

	srand(time(NULL));
	//Start Optimiziation loop:
	float maxFitness;

	for (int i = 0; i < modelParameters.nrGenerations; i++) {

		//std::cout << "Genration Number:" << i << "   First individual fitness:" << model.population[0].fitness << std::endl;
		model.maximumFitnessInCurrentGeneration = 0;

		if (i > 0){
			for(int j = 0; j < modelParameters.populationSize * 0.1; j++){
				model.population[j].chromosome = model.bestChromosomes;
			}
		}

		//std::cout << "BESTINDIVIDAL:" << bestIndividual.fitness << std::endl;
		//std::cout << "MODEL[0]:" << model.population[0].fitness << std::endl;
		//Evaluate individuals:
		#pragma omp parallel for
		for (auto& ind : model.population) {
			ind.EvaluateIndividual();
			if (ind.fitness > model.maximumFitnessInCurrentGeneration) {
				model.maximumFitnessInCurrentGeneration = ind.fitness;
				model.bestIndividualId = ind.id;
				if(ind.fitness > maxFitness){
					maxFitness = ind.fitness;
				}
			}
		}

		#pragma omp parallel for
		for(int i = 0; i < modelParameters.populationSize; i = i + 2){
					//Tournament selection
			int ind1Id = model.TournementSelect();
			int ind2Id = model.TournementSelect();

			//std::cout << "indexes are:" <<ind1Id << "," <<ind2Id << std::endl;

			float randomNumber = Individual::GenerateRandomFloat(0.0, 1.0);

			
			//Crossover
			if (randomNumber <= modelParameters.crossOverProb) {
				model.Cross(model.population[ind1Id], model.population[ind2Id]);
			}
		}

		//Elitisim
		model.bestChromosomes = model.population[model.bestIndividualId].chromosome;
		
		std::cout << "best Ind ID:" << model.bestIndividualId << std::endl;
		std::cout << "bestChromosome first element:" << model.bestChromosomes[0] << std::endl;


		for (auto& ind : model.population) {
			model.Mutate(ind);
		}

		model.parameters.mutationProb += 0.000001;


		std::cout << "Genration Number:" << i << "   Current Best Fitness:" << model.maximumFitnessInCurrentGeneration << std::endl;

		/*
		for(int i = 0;i < 20;i ++){
			for (auto& el : model.population[i].chromosome)
			std::cout << el;
			std:: cout << std::endl;
		;
		}
		
		for (auto& ind : model.population) {

			std::cout << "Fitness:" << ind.fitness << std::endl;

		}
		*/
	}

	std::cout << "check 1" << std::endl;
	std::cout << model.bestChromosomes[0] << std::endl;

	std::cout << "check 2" << std::endl;
	//Save the best Indvidual as csv file:
	std::ofstream myFile("output.csv");
	for(int i = 0; i < nrDeltaTs; i++){
		//std::cout << std::to_string(model.bestChromosomes[i]) << std::endl;
		myFile << std::to_string(model.bestChromosomes[i]);
		myFile << ",";
	}
	myFile.close();
	

	

	for (int i = 0; i < rows; i++) {
		delete[] positions[i];
	}
	delete[] positions;
	

	return 0;
}






//Functions 
int read_record(std::string fileName, float** positions)
{
	std::string fname = fileName;

	std::vector<std::vector<std::string>> content;
	std::vector<std::string> row;
	std::string line, word;

	std::fstream file(fname, std::ios::in);
	if (file.is_open())
	{
		while (getline(file, line))
		{
			row.clear();

			std::stringstream str(line);

			while (getline(str, word, ','))
				row.push_back(word);
			content.push_back(row);
		}
	}
	else
		std::cout << "Could not open the file\n";
	
	file.close();
	content.erase(content.begin());
	int index = 0;
	//for (auto& element : content) {
	for(int j = 1; j < content.size(); j++){
		float diff[6];
		float normSecond = 0;
		
		for (int i = 2; i < 8; i++) {
			diff[i-2] = std::stof(content[j][i]) - std::stof(content[j-1][i]);
			//std::cout << "element values are:" << std::stof(element[i]) << std::endl;
		}
		float sum = 0;
		for (int i = 0; i < 6; i++) {
			sum += diff[i] * diff[i];
		}
		if ( sqrt(sum) > 0.1){
			for (int i = 2; i < 8; i++) {
				positions[index][i-2] = std::stof(content[j][i]);

				//std::cout << "element values are:" << std::stof(element[i]) << std::endl;
			}
			index++;
		}
	}
	return index;
}