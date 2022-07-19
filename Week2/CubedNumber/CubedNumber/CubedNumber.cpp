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
double GetNumber() {
	//Prompt the user for a number
	cout << "Welcome! This program will cube a number." << endl;
	cout << "Please provide a number: ";

	//Get the number
	double num;

	cin >> num;

	return num;
}

template < typename T>
T FindCubed(T n) {
	//Run the function to calculate the number cubed

	return n * n * n;
}

void PrintCubed(double cubed) {
	//Print the value back to the user

	cout << "Your number cubed is: " << cubed << endl;

}