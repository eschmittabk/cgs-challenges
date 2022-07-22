// MaxOfArray.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>

using namespace std;

int GetArraySize();
void PopulateArray(float numArray[], int size);
float GetMaxArray(float numArray[], int size);
void DisplayArray(float numArray[], int size);

int main()
{
    cout << "This program will find the maximum value of an array." << endl;
    
    int size = GetArraySize();

    float* numArray = new float[size];

    PopulateArray(numArray, size);

    cout << "The maximum value of the array ";

    DisplayArray(numArray, size);
    
    cout << "is " << GetMaxArray(numArray, size) << endl;

    delete[] numArray;
    numArray = nullptr;
}

int GetArraySize() 
{
    int size;

    do 
    {
        cout << "How big is your array? Please enter a positive integer: ";
        cin >> size;

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
        else if (size <= 0)
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system("cls");
            cout << "That is not a POSITIVE integer. Please try again!" << endl;
        }
        else 
        {
            break;
        }

    } while (true);

    return size;
}

void PopulateArray(float numArray[], int size) 
{
    cout << "Enter " << size << " numbers for your array: " << endl;

    for (int i = 0; i < size; i++) 
    {
        cin >> numArray[i];

        if (cin.fail()) 
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number!" << endl;
            i--;
        }
        else if(cin.get() != '\n')
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number!" << endl;
            i--;
        }
    }
}

float GetMaxArray(float numArray[], int size) 
{
    float maxValue = numArray[0];

    for (int i = 1; i < size; i++) 
    {
        if (numArray[i] > maxValue) {
            maxValue = numArray[i];
        }
    }

    return maxValue;
}

void DisplayArray(float numArray[], int size) 
{
    for (int i = 0; i < size; i++) 
    {
        cout << numArray[i] << " ";
    }
}
