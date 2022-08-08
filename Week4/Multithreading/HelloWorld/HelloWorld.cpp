// HelloWorld.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>

using namespace std;

//static int HeapInt = 0;

bool didQuit = false;
int ThreadVar = 0;
void HelloWorldThread();

int main()
{

    //HeapInt = 5;

    cout << "Hello World!" << endl;

    char userInput;

    std::thread Hello(HelloWorldThread);

    while (!didQuit)
    {
        cout << "Enter any key to display counter" << endl;
        cout << "Enter q to quit" << endl;
        cin >> userInput;
        didQuit = (userInput == 'q') || (userInput == 'Q');
        cout << "ThreadVar: " << ThreadVar << endl;
    }


    Hello.join();

    return 0;
}

void HelloWorldThread()
{
    while (!didQuit)
    {
        ThreadVar++;

        if (ThreadVar > 1000)
        {
            ThreadVar = 0;
        }
    }
}
