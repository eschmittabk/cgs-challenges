//Boat.cpp

#include "Boat.h"
#include <iostream>

using namespace std;

//Constructor:
Boat::Boat()
    : Vehicle(2022, 0, "Unspecified")
{

}

Boat::Boat(int year, int miles, string brand, string model)
    : Vehicle(year, miles, brand)
    , m_boatModel(model)
{

}

//Destructor
Boat::~Boat()
{

}


//Member functions
void Boat::Display()
{
    cout << "Boat: " << m_year << " " << m_brand << " " << m_boatModel << " with " << m_miles << " miles." << endl;
}

void Boat::Drive(int miles)
{
    cout << "Driving " << miles << " miles." << endl;
    m_miles += miles;
}
