//Find the "Sum" and "Average" of "three" numbers.
//Used float instead of int to allow for decimals
#include <iostream>

using namespace std;

void WelcomeMessage();
float GetNumbers(int count);
float SumNumbers(float num1, float num2, float num3);
float AvgNumber(float sum);
void PrintResults(float num1, float num2, float num3, float sum, float avg);

int main()
{
	WelcomeMessage();

	//Get the 3 numbers
	float num1, num2, num3;

	num1 = GetNumbers(1);
	num2 = GetNumbers(2);
	num3 = GetNumbers(3);

	//Calculate sum
	float sum;
	sum = SumNumbers(num1, num2, num3);

	//Calculate average
	float avg;

	avg = AvgNumber(sum);


	//Output
	PrintResults(num1, num2, num3, sum, avg);
}

void WelcomeMessage() 
{
	//Provide a prompt to the user for 3 numbers
	cout << "Welcome! This program will find the sum and average of 3 numbers." << endl;
	cout << "Provide 3 numbers you would like to use for calculations." << endl;
}

float GetNumbers(int count) 
{
	float num;

	do
	{
		cout << "Number " << count << ": ";
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

float SumNumbers(float num1, float num2, float num3) 
{
	float sum;

	//Calculate sum
	sum = num1 + num2 + num3;

	return sum;
}

float AvgNumber(float sum) 
{
	float avg;

	//Calculate average
	avg = (sum / 3.0f);

	return avg;
}

void PrintResults(float num1, float num2, float num3, float sum, float avg) 
{
	cout << "The sum of " << num1 << ", " << num2 << ", and " << num3 << " is: " << sum << endl;
	cout << "The average of " << num1 << ", " << num2 << ", and " << num3 << " is: " << avg << endl;
}