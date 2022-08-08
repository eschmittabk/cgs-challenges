// HelloWorld.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>

using namespace std;

//static int HeapInt = 0;

bool didQuit = false;
int ThreadVar = 0;
int ThreadVar2 = 0;
void HelloWorldThread();
void HelloWorldThread2();

int main()
{

    //HeapInt = 5;

    cout << "Hello World!" << endl;

    char userInput;

    std::thread Hello(HelloWorldThread);
    std::thread Hello2(HelloWorldThread2);

    while (!didQuit)
    {
        cout << "Enter any key to display counter" << endl;
        cout << "Enter q to quit" << endl;
        cin >> userInput;
        didQuit = (userInput == 'q') || (userInput == 'Q');
        cout << "ThreadVar: " << ThreadVar << endl;
        cout << "ThreadVar2: " << ThreadVar2 << endl;
    }


    Hello.join();
    Hello2.join();

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

void HelloWorldThread2()
{
    while (!didQuit)
    {
        ThreadVar2++;

        if (ThreadVar2 > 1000)
        {
            ThreadVar2 = 0;
        }
    }
}
