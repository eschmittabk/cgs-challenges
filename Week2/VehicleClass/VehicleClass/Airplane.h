#pragma once
#include <string>
#include "Vehicle.h"

using std::string;

class Airplane : public Vehicle
{
private:
    //Member variables
    string m_airplaneModel;
public:
    //Constructors:
    Airplane();
    Airplane(int year, int miles, string brand, string model);

    //Destructor
    ~Airplane();

    //Member functions
    void Display() override;
    void Drive(int miles) override;
};
