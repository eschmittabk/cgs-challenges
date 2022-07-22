// LeapYear.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>

using namespace std;

int GetYear();
void IsLeapYear(int year);

int main()
{
    cout << "This program will check to see if a year is a leap year." << endl;
    
    int year = GetYear();
    
    IsLeapYear(year);
}

int GetYear() 
{
    int year;

    do 
    {
        cout << "Please enter a year after 1581: ";
        
        cin >> year;

        if (cin.fail()) 
        {
            cin.clear();
            cin.ignore(100, '\n');
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
        else if (year < 1582) 
        {
            system("cls");
            cout << year << " is not part of the Gregorian Calendar, so it is not a leap year." << endl;
        }
        else {
            break;
        }

    } while (true);
    
    return year;
}

void IsLeapYear(int year) 
{
    if (year % 400 == 0) 
    {
        cout << year << " is a leap year." << endl;
    }
    else if ((year % 4 == 0) && (year % 100 != 0)) 
    {
        cout << year << " is a leap year." << endl;
    }
    else 
    {
        cout << year << " is not a leap year." << endl;
    }
}