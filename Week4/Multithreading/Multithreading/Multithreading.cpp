// Multithreading.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>

using namespace std;

bool didQuit = false;
bool shouldDecrementLife = false;
mutex Mutex;

void UpdateCharacter1();
void UpdateCharacter2();
void ProcessInput();

struct Character
{
    float Position = 0.0f;
    atomic<int> Score = 0;
    //int Score = 0;
    int Lives = 1;

    void DisplayStats()
    {
        cout << "Lives: " << Lives << endl;
    }
};

Character player;

int main()
{
    thread InputHandler(ProcessInput);
    thread CharacterUpdate1(UpdateCharacter1);
    thread CharacterUpdate2(UpdateCharacter2);

    InputHandler.join();
    CharacterUpdate1.join();
    CharacterUpdate2.join();

    return 0;
}

void UpdateCharacter1()
{
    while (!didQuit)
    {
        if (shouldDecrementLife)
        {
            lock_guard<mutex> Guard(Mutex);
            if (player.Lives > 0)
            {
                //the two lines below essentially do the same thing
                //this_thread::sleep_for(chrono::milliseconds(500));
                this_thread::yield();
                --player.Lives;
            }
        }
    }
}

void UpdateCharacter2()
{
    while (!didQuit)
    {
        if (shouldDecrementLife)
        {
            lock_guard<mutex> Guard(Mutex);
            if (player.Lives > 0)
            {
                //the two lines below essentially do the same thing
                //this_thread::sleep_for(chrono::milliseconds(500));
                this_thread::yield();
                --player.Lives;
            }
        }
    }
}

void ProcessInput()
{
    while (!didQuit)
    {
        cout << "'a' to decrement player life" << endl;
        cout << "'d' to display player stats" << endl;
        cout << "'q' to quit" << endl;

        char userInput;
        cin >> userInput;

        switch (userInput)
        {
        case 'a':
            shouldDecrementLife = true;
            break;
        case 'd':
            player.DisplayStats();
            break;
        case 'q':
            didQuit = true;
            break;
        default:
            break;
        }

        didQuit = (userInput == 'q') || (userInput == 'Q');
    }
}