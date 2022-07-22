#include "Vehicle.h"
#include <iostream>

using namespace std;



//Constructors:
Vehicle::Vehicle()
    : m_year(2022)
    , m_miles(0)
    , m_brand("Unspecified")
{

}

Vehicle::Vehicle(int year, int miles, string brand)
    : m_year(year)
    , m_miles(miles)
    , m_brand(brand)
{

}

//Destructor
Vehicle::~Vehicle()
{

}

//Member functions
//Setter
void Vehicle::SetYear(int newYear)
{
    m_year = newYear;
}

//Getter
int Vehicle::GetYear()
{
    return m_year;
}