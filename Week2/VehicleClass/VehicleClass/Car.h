#pragma once
#include <string>
#include "Vehicle.h"

using std::string;

class Car : public Vehicle
{
private:
    //Member variables
    string m_carModel;
public:
    //Constructors:
    Car();
    Car(int year, int miles, string brand, string model);

    //Destructor
    ~Car();

    //Member functions
    void Display() override;
    void Drive(int miles) override;
};
