//Car.cpp

#include "Car.h"
#include <iostream>

using namespace std;

//Constructor:
Car::Car()
    : Vehicle(2022, 0, "Unspecified")
{

}

Car::Car(int year, int miles, string brand, string model)
    : Vehicle(year, miles, brand)
    , m_carModel(model)
{

}

//Destructor
Car::~Car() 
{
   
}


//Member functions
void Car::Display()
{
    cout << "Car: " << m_year << " " << m_brand << " " << m_carModel << " with " << m_miles << " miles." << endl;
}

void Car::Drive(int miles)
{
    cout << "Driving " << miles << " miles." << endl;
    m_miles += miles;
}
