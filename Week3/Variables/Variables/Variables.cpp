// Variables.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

using namespace std;

int GetSize(); //asks the user how many variables they would like to create
void NameVariables(char* varNames, int numVar); //asks the users to name each variable with a lowercase letter
void PopulateVariables(int* var, char* varNames, int numVar); //asks the user for the value of each variable
void DisplayPointer(int* var, char* varNames, int numVar); //displays the name and value of each variable

int main()
{
   
    int numVar = GetSize();

    char* varNames = new char[numVar]; //creates a pointer to an array of chars

    NameVariables(varNames, numVar);

    int* var = new int[numVar]; //creates a pointer to an array of ints

    PopulateVariables(var, varNames, numVar);
    DisplayPointer(var, varNames, numVar);

    //deletes pointers and sets them to null
    delete[] varNames;
    varNames = nullptr;

    delete[] var;
    varNames = nullptr;
}

int GetSize() //asks the user how many variables they would like to create
{
    int numVar;
    do
    {
        cout << "How many variables would you like to assign? ";
        cin >> numVar;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system("cls");
            cout << "Invalid input! Please enter a positive integer!" << endl;
        }
        else if (cin.get() != '\n')
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system("cls");
            cout << "Invalid input! Please enter a positive integer!" << endl;
        }
        else if (numVar <= 0)
        {
            cin.clear();
            system("cls");
            cout << "Invalid input! Please enter a positive integer!" << endl;
        }
        else
        {
            break;
        }

    } while (true);

    return numVar;
}

void NameVariables(char* varNames, int numVar) //asks the users to name each variable with a lowercase letter
{
    for (int i = 0; i < numVar; i++) 
    {
        cout << "What is the name of variable " << i + 1 << ": ";
        cin >> varNames[i];

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system("cls");
            cout << "Invalid input! Please enter a lowercase character!" << endl;
            i--;
        }
        else if (cin.get() != '\n')
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system("cls");
            cout << "Invalid input! Please enter a lowercase character!" << endl;
            i--;
        }
        else if (varNames[i] < 'a')
        {
            cin.clear();
            system("cls");
            cout << "Invalid input! Please enter a lowercase character!" << endl;
            i--;
        }
    }

}

void PopulateVariables(int* var, char* varNames, int numVar) //asks the user for the value of each variable
{
    for(int i = 0; i < numVar; i++)
    {
        cout << "Please enter an integer for " << varNames[i] << ": ";
        cin >> var[i];

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system("cls");
            cout << "Invalid input! Try again!" << endl;
            i--;
        }
        else if (cin.get() != '\n')
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system("cls");
            cout << "Invalid input! Try again!" << endl;
            i--;
        }
    }
}

void DisplayPointer(int* var, char* varNames, int numVar) //displays the name and value of each variable
{
    for (int i = 0; i < numVar; i++)
    {
        cout << "*ptr" << (char)toupper(varNames[i]) << ": " << var[i] << endl;
    }
}
