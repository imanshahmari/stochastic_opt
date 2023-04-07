#ifndef INDIVIDUAL
#define INDIVIDUAL


#include <array>
#include <vector>
#include <stdexcept>

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
    float mu;
    float sigma;
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

        static float GenerateRandomFloat(float lowerBound, float upperBound);

        bool operator<(const Individual& other) const;

        bool operator==(const Individual& other) const;
        bool operator>(const Individual& other) const;



        int getLen() { return chromLen; }
        int getId() { return id; }

    //~Individual();
};

#endif