// VehicleClass.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include "Airplane.h"
#include "Boat.h"
#include "Car.h"

using namespace std;

int main()
{
    //Airplane initialize and drive
    Airplane myPlane(2022, 3000, "Boeing", "777");
    myPlane.Display();
    myPlane.Drive(6000);
    myPlane.Display();

    cout << endl;

    //Boat initialize and drive
    Boat myBoat(2022, 50, "MasterCraft", "Prostar");
    myBoat.Display();
    myBoat.Drive(33);
    myBoat.Display();

    cout << endl;

    //Car initialize and drive
    Car myCar(2022, 300, "Chevrolet", "Corvette");
    myCar.Display();
    myCar.Drive(333);
    myCar.Display();
}
