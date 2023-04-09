#ifndef INDIVIDUAL
#define INDIVIDUAL


#include <array>
#include <vector>
#include <stdexcept>
#include <random>
#include <iostream>

//Structs and template classes:

struct MechanicalUnits {
    std::vector<std::vector<float>> velocities;
    std::vector<std::vector<float>> accelerations;
};

struct IndividualParameters {
    int nrDeltaTs;
    float** posArray;
    int nrJoints;
    int nrPositions;
    float endTime;
};



//template classes
template<typename T>
class MyVector : public std::vector<T>
{
    int _maxSize;

public:
    MyVector() : _maxSize(0) {} // Default constructor

    MyVector(const std::vector<T>& vec, int maxSize) : std::vector<T>(vec), _maxSize(maxSize) {}

    void setMaxSize(int maxSize) { _maxSize = maxSize; }

    void push_back(const T& element)
    {
        if (std::vector<T>::size() < _maxSize)
            std::vector<T>::push_back(element);
        else
        {
            throw std::overflow_error("MAXIMUM ELEMENT EXCEEDED");
        }
    }
};


//Individual class
class Individual
{
    public:
        int id;
        int chromLen;
        MechanicalUnits mechUnits;
        IndividualParameters indParameters;
        float fitness;

        MyVector<float> chromosome;
        Individual(IndividualParameters passedParameters, int id);

        void InitilizeConstant();
        void InitilizeRandom();

        void Derivative();
        void EvaluateIndividual();

        //float GenerateRandomFloat(float lowerBound, float upperBound);

        bool operator<(const Individual& other) const;

        bool operator==(const Individual& other) const;
        bool operator>(const Individual& other) const;



        int getLen() { return chromLen; }
        int getId() { return id; }
        static inline float GenerateRandomFloat(float lowerBound, float upperBound) {
            /*
            static std::random_device rand_dev;
            static std::mt19937 generator(rand_dev());
            std::uniform_real_distribution<float> distr(lowerBound, upperBound);
            float randomNumber = distr(generator);
            
            return randomNumber;
            */
            //std::cout << "randomNumber:" << (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (upperBound - lowerBound) + lowerBound << std::endl;
            return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (upperBound - lowerBound) + lowerBound;
        }

    //~Individual();
};

#endif