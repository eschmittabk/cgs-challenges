//Find the "Sum" and "Average" of "three" numbers.
//Used double instead of int to allow for decimals and precision
#include <iostream>

using namespace std;

void WelcomeMessage();

double GetNumbers(int count);

double SumNumbers(double num1, double num2, double num3);

double AvgNumber(double sum);

void PrintResults(double num1, double num2, double num3, double sum, double avg);

int main()
{
	WelcomeMessage();

	//Get the 3 numbers
	double num1, num2, num3;

	num1 = GetNumbers(1);
	num2 = GetNumbers(2);
	num3 = GetNumbers(3);

	//Calculate sum
	double sum;
	sum = SumNumbers(num1, num2, num3);

	//Calculate average
	double avg;

	avg = AvgNumber(sum);


	//Output
	PrintResults(num1, num2, num3, sum, avg);

}

void WelcomeMessage() {
	//Provide a prompt to the user for 3 numbers
	cout << "Welcome! This program will find the sum and average of 3 numbers." << endl;
	cout << "Provide 3 numbers you would like to use for calculations." << endl;
}

double GetNumbers(int count) {

	cout << "Number " << count << ": ";

	double num;

	cin >> num;

	return num;
}

double SumNumbers(double num1, double num2, double num3) {
	double sum;

	//Calculate sum
	sum = num1 + num2 + num3;

	return sum;
}

double AvgNumber(double sum) {
	double avg;

	//Calculate average
	avg = (sum / 3.0);

	return avg;
}

void PrintResults(double num1, double num2, double num3, double sum, double avg) {

	cout << "The sum of " << num1 << ", " << num2 << ", and " << num3 << " is: " << sum << endl;
	cout << "The average of " << num1 << ", " << num2 << ", and " << num3 << " is: " << avg << endl;

}