// Factors.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>

using namespace std;

int GetNumber();
void FindFactors(int number);

int main()
{
    cout << "This program will find all of the factors of a number." << endl;
 
    int number; 

    number = GetNumber();

    FindFactors(number);
}

int GetNumber()
{
    int number;
    do 
    {
        cout << "Please input a positive number: ";
        cin >> number;

        
        if (cin.fail()) 
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system("cls");
            cout << "Invalid input! Try again!" << endl;
        }
        else if (cin.get() != '\n')
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system("cls");
            cout << "Invalid input! Try again!" << endl;
        }
        else if (number <= 0)
        {
            cin.clear();
            system("cls");
            cout << "Invalid input! Try again!" << endl;
        }
        else 
        {
            break;
        }
    } while (true);

    return number;
}

void FindFactors(int number)
{
    cout << "The following numbers are factors of " << number << ":";

    for (int i = 1; i <= number; i++) 
    {
        if (number % i == 0) {
            cout << " " << i;
        }
    }
}