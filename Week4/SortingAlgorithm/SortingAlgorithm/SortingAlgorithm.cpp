// SortingAlgorithm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//Includes
#include <iostream>
#include <chrono>
#include <stdlib.h>

//Defines
//#define ARRAY_SIZE 100 //no longer needed because I used a dynamic array size

//Namespaces
using namespace std;

int GetArraySize();

uint32_t GetNanos();

void FillArray(int arrayToFill[], int sizeOfArray);
int PickSort(int arrayToSort[], int sizeOfArray);
void SortArray(int arrayToSort[], int sizeOfArray, int choice);

void BubbleSort(int arrayToSort[], int sizeOfArray);
void SelectionSort(int arrayToSort[], int sizeOfArray);
void InsertionSort(int arrayToSort[], int sizeOfArray);

int ChooseASearchKey(int arrayToPickFrom[], int sizeOfArray);
void LinearSearch(int arrayToSearch[], int sizeOfArray, int searchKey);
void BinarySearch(int arrayToSearch[], int low, int high, int searchKey);

void PrintArray(int arrayToSort[], int sizeOfArray);

int main()
{
    int arraySize = GetArraySize();
       
    //Create a dataset
    int* dataset = new int[arraySize];

    //Seed our random generator and fill the dataset array with random numbers
    srand(time(nullptr));
    FillArray(dataset, arraySize);

    int choice = PickSort(dataset, arraySize);

    //Pick a random number (that exists in the array) to search for
    const int searchKey = ChooseASearchKey(dataset, arraySize);

    //Perform a linear search
    uint32_t startNanos = GetNanos();
    LinearSearch(dataset, arraySize, searchKey);
    uint32_t endNanos = GetNanos();

    cout << "Linear Search took: " << (endNanos - startNanos) << " ns" << endl << endl;

    PrintArray(dataset, arraySize);

    //Sort our dataset
    SortArray(dataset, arraySize, choice);

    PrintArray(dataset, arraySize);

    //Search for search key with binary search
    startNanos = GetNanos();
    BinarySearch(dataset, 0, arraySize, searchKey);
    endNanos = GetNanos();

    cout << "Binary Search took: " << (endNanos - startNanos) << " ns" << endl << endl;

    delete[] dataset;
    dataset = nullptr;
}

int GetArraySize()
{
    int arraySize;

    do
    {
        cout << "Please pick a size for your array: ";
        cin >> arraySize;

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
        else if (arraySize <= 0)
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

    return arraySize;
}

uint32_t GetNanos()
{
    using namespace std::chrono;
    return static_cast<uint32_t>(duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count());
}

void FillArray(int arrayToFill[], int sizeOfArray)
{
    for (int i = 0; i < sizeOfArray; ++i)
    {
        //Fill the array with a random number between 1 and 100
        arrayToFill[i] = rand() % sizeOfArray + 1;
    }
}

int PickSort(int arrayToSort[], int sizeOfArray)
{
    int choice;

    do
    {
        cout << "What kind of sort would you like to do?" << endl;
        cout << "1. Bubble Sort" << endl;
        cout << "2. Selection Sort" << endl;
        cout << "3. Insertion Sort" << endl;

        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system("cls");
            cout << "Invalid input! Please 1, 2, or 3!" << endl;
        }
        else if (cin.get() != '\n')
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system("cls");
            cout << "Invalid input! Please 1, 2, or 3!" << endl;
        }
        else if (choice <= 0 || choice > 3)
        {
            cin.clear();
            system("cls");
            cout << "Invalid input! Please 1, 2, or 3!" << endl;
        }
        else
        {
            break;
        }

    } while (true);

    return choice;
}

void SortArray(int arrayToSort[], int sizeOfArray, int choice)
{
    uint32_t startNanos;
    uint32_t endNanos;

    switch (choice)
    {
    case 1:
        startNanos = GetNanos();
        BubbleSort(arrayToSort, sizeOfArray);
        endNanos = GetNanos();

        cout << "Bubble Sort took: " << (endNanos - startNanos) << " ns" << endl << endl;
        break;
    case 2:
        startNanos = GetNanos();
        SelectionSort(arrayToSort, sizeOfArray);
        endNanos = GetNanos();

        cout << "Selection Sort took: " << (endNanos - startNanos) << " ns" << endl << endl;
        break;
    case 3:
        startNanos = GetNanos();
        InsertionSort(arrayToSort, sizeOfArray);
        endNanos = GetNanos();

        cout << "Insertion Sort took: " << (endNanos - startNanos) << " ns" << endl << endl;
        break;
    default:
        break;
    }
}

void BubbleSort(int arrayToSort[], int sizeOfArray)
{
    long long int comparisons = 0;
    
    //Loop to control the number of pass
    for (int i = 0; i < sizeOfArray; ++i)
    {
        //Loop to control the number of comparisons per pass
        for (int j = 0; j < sizeOfArray - 1; ++j)
        {
            ++comparisons;
            
            //Compare adjacent element values
            if (arrayToSort[j] > arrayToSort[j + 1])
            {
                //Store the current integer in a temporary variable
                int temp = arrayToSort[j];

                //Shift the next element to the current position
                arrayToSort[j] = arrayToSort[j + 1];

                //Assign the next value to the value held in the temporary variable
                arrayToSort[j + 1] = temp;
            }
        }
    }
    cout << "Bubble Sort comparisons: " << comparisons << endl;
}

void SelectionSort(int arrayToSort[], int sizeOfArray)
{
    long long int comparisons = 0;

    for (int i = 0; i < sizeOfArray - 1; ++i)
    {
        int min = arrayToSort[i];

        for (int j = i+1; j < sizeOfArray; ++j)
        {
            ++comparisons;
            
            if (arrayToSort[j] < min)
            {
                int temp;

                temp = arrayToSort[i];
                arrayToSort[i] = arrayToSort[j];
                arrayToSort[j] = temp;
            }
        }
    }

    cout << "Selection Sort comparisons: " << comparisons << endl;
}

void InsertionSort(int arrayToSort[], int sizeOfArray)
{
    long long int comparisons = 0;

    for (int i = 1; i < sizeOfArray; ++i)
    {
        int temp = arrayToSort[i];
        int j = i;

        if (arrayToSort[i] >= arrayToSort[j - 1])
        {
            ++comparisons;
        }

        while (j > 0 && arrayToSort[j - 1] > temp)
        {
            ++comparisons;

            arrayToSort[j] = arrayToSort[j - 1];

            --j;
        }
        arrayToSort[j] = temp;
    }
    cout << "Insertion Sort comparisons: " << comparisons << endl;
}

int ChooseASearchKey(int arrayToPickFrom[], int sizeOfArray)
{
    int key;
    key = rand() % sizeOfArray;

    return arrayToPickFrom[key];
}

void LinearSearch(int arrayToSearch[], int sizeOfArray, int searchKey)
{
    int comparisons = 0;
    
    //Loop over each element in the array
    for (int i = 0; i < sizeOfArray; ++i)
    {
        ++comparisons;
        //Check if this value matches the search key
        if (arrayToSearch[i] == searchKey)
        {
            //Search complete
            cout << "Search key " << searchKey << " found at index: " << i << endl;
            cout << "Linear Search comparisons: " << comparisons << endl;
            return;
        }
    }

    cout << "Search key not found!" << endl;
}

void BinarySearch(int arrayToSearch[], int low, int high, int searchKey)
{
    int comparisons = 0;
    
    //Loop until the low subscript is greater than the high subscript
    while (low <= high) 
    {
        //Find the middle of the array
        int middle = (low + high) / 2;

        ++comparisons;

        if (searchKey == arrayToSearch[middle])
        {
            //Search complete
            cout << "Search key " << searchKey << " found at index: " << middle << endl;
            cout << "Binary Search comparisons: " << comparisons << endl;
            return;
        }
        else if (searchKey < arrayToSearch[middle])
        {
            //Search the lower half of the remaining array
            high = middle - 1;
        }
        else //(searchKey > arrayToSearch[middle])
        {
            //Search the higher half of the remaining array
            low = middle + 1;
        }
    }

    cout << "Search key not found!" << endl;
}

void PrintArray(int arrayToSort[], int sizeOfArray)
{
    for (int i = 0; i < sizeOfArray; ++i)
    {
        cout << arrayToSort[i] << " ";
    }
    cout << endl << endl;
}
