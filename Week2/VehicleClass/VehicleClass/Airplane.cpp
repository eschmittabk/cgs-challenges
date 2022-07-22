//Airplane.cpp

#include "Airplane.h"
#include <iostream>

using namespace std;

//Constructor:
Airplane::Airplane()
    : Vehicle(2022, 0, "Unspecified")
{

}

Airplane::Airplane(int year, int miles, string brand, string model)
    : Vehicle(year, miles, brand)
    , m_airplaneModel(model)
{

}

//Destructor
Airplane::~Airplane()
{

}


//Member functions
void Airplane::Display()
{
    cout << "Airplane: " << m_year << " " << m_brand << " " << m_airplaneModel << " with " << m_miles << " miles." << endl;
}

void Airplane::Drive(int miles)
{
    cout << "Driving " << miles << " miles." << endl;
    m_miles += miles;
}
