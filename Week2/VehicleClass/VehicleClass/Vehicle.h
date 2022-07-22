#pragma once
#include <string>

using std::string;

class Vehicle 
{
protected:
    //Member variables
    int m_year;
    int m_miles;
    string m_brand;

public:
    //Constructors:
    Vehicle();
    Vehicle(int year, int miles, string brand);

    //Destructor
    ~Vehicle();

    //Member functions
    //Setter
    void SetYear(int newYear);
    //Getter
    int GetYear();

    virtual void Display() = 0;
    virtual void Drive(int miles) = 0;
};
