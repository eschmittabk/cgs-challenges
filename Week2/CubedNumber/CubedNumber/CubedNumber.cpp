//Use a function to return the value of a number cubed
#include <iostream>

using namespace std;

//function declarations
double GetNumber();
template < typename T>
T FindCubed(T n);
void PrintCubed(double cubed);

int main()
{
	double num = GetNumber();

	double cubed = FindCubed(num);

	PrintCubed(cubed);
}

//function definitions
double GetNumber() 
{
	//Prompt the user for a number
	cout << "Welcome! This program will cube a number." << endl;

	//Get the number
	double num;

    do
    {
        cout << "Please provide a number: ";
        cin >> num;

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
        else
        {
            break;
        }

    } while (true);
	

	return num;
}

template < typename T>
T FindCubed(T n) 
{
	//Run the function to calculate the number cubed
	return n * n * n;
}

void PrintCubed(double cubed) 
{
	//Print the value back to the user
	cout << "Your number cubed is: " << cubed << endl;
}