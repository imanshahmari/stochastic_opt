// genetic.cpp : Defines the entry point for the application.
//

#include "Interface.h"
#include "Individual.h"
#include "Model.h"

#include <algorithm>
#include <cmath>
#include <omp.h>


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
	modelParameters.constantDeltasRatio = 0.0;
	modelParameters.populationSize = 1000;
	modelParameters.nrGenerations = 100;
	modelParameters.crossOverProb = 0.85;
	modelParameters.mutationProb = 0.2;
	modelParameters.creepProba = 0.5;
	modelParameters.creepRate = 0.0125;
	modelParameters.tournamentProb = 0.6;
	modelParameters.tournamentSize = 1;


	Individual ind1(indParameters,500);
	ind1.InitilizeConstant();
	ind1.EvaluateIndividual();

	Individual ind2(indParameters,502);
	ind2.InitilizeConstant();
	ind2.EvaluateIndividual();
	

	//Initilize population

	Model model = Model("lamdaMu", modelParameters);
	model.InitilizePopulation();
	/*
	auto test = model.TournementSelect();
	model.Cross(ind1,ind2);
	model.Mutate(ind1);
	std::cout << "hello" << std::endl;
	*/



	
	//Start Optimiziation loop:
	model.maximumFitnessInCurrentGeneration = 0;
	float bestFitness;

	for (int i = 0; i < modelParameters.nrGenerations; i++) {
		//Evaluate individuals:

		#pragma omp parallel for num_threads(4)
		for (auto& ind : model.population) {
			std::cout << 
			ind.EvaluateIndividual();
			if (ind.fitness > model.maximumFitnessInCurrentGeneration) {
				model.maximumFitnessInCurrentGeneration = ind.fitness;
				model.bestIndividualId = ind.id;
			}
		}

		//Tournament selection
		int ind1Id = model.TournementSelect();
		int ind2Id = model.TournementSelect();

		std::cout << "indexes are:" <<ind1Id << "," <<ind2Id << std::endl;

		float randomNumber = Individual::GenerateRandomFloat(0.0, 1.0);

		
		//Crossover
		if (randomNumber <= modelParameters.crossOverProb) {
			model.Cross(model.population[ind1Id], model.population[ind2Id]);
		}
		

		//Elitisim
		model.population[model.bestIndividualId] = model.population[model.bestIndividualId];



		for (auto& ind : model.population) {
			model.Mutate(ind);
		}

		std::cout << "Genration Number:" << i << "     Best Fitness:" << model.maximumFitnessInCurrentGeneration << std::endl;

		/*
		for (auto& ind : model.population) {

			std::cout << "Fitness:" << ind.fitness << std::endl;

		}
		*/
		
	}
	

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