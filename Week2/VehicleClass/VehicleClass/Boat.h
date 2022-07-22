#pragma once
#include <string>
#include "Vehicle.h"

using std::string;

class Boat : public Vehicle
{
private:
    //Member variables
    string m_boatModel;
public:
    //Constructors:
    Boat();
    Boat(int year, int miles, string brand, string model);

    //Destructor
    ~Boat();

    //Member functions
    void Display() override;
    void Drive(int miles) override;
};
