// aStar.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <unordered_map>
#include <string>
#include <list>
#include "Node.h"

using namespace std;

int GetNodeAmount();
void GetNodes(list<Node> &userNodes, int numEdges);
void GetPath(char& start, char& goal);

template < typename T>
void ValidateInput(T &input, string outputMessage, int loop, bool isChar);

Node CreateNewEdge(char x, char y, int cost);
void AstarSearch(list<Node> edges, char start, char goal);

void DisplayResults(char start, char goal, int cost);

int main()
{
    int  numEdges = GetNodeAmount();

    //edges map: cost to move from node 1 to node 2
    //cameFrom map: node, fScore of node (f = g + h), parent
    list<Node> edges;

    //unordered_map<Node, 

    GetNodes(edges, numEdges);

    char start;
    char goal;

    GetPath(start, goal);



    if (edges.size() > 0) 
    {
        AstarSearch(edges, start, goal);
    }

    return 0;
}

int GetNodeAmount()
{
    int numEdges;

    ValidateInput(numEdges, "How many coordinate edges are in your map?: ", NULL, false);

    return numEdges;
}

void GetNodes(list<Node> &edges, int numEdges)
{
    char start;
    char end;
    int  cost;

    cout << "Input the start, end, and cost to travel between the 2 points for all edges." << endl;

    for (int i = 0; i < numEdges; ++i)
    {
        ValidateInput(start, "Input start node for edge #", i + 1, true);

        ValidateInput(end, "Input end node for edge #", i + 1, true);

        ValidateInput(cost, "Input cost for edge #", i + 1, false);

        edges.push_back(CreateNewEdge(start, end, cost));
    }
}

void GetPath(char& start, char& goal)
{
    ValidateInput(start, "Input start node for path: ", NULL, true);

    ValidateInput(goal, "Input end node for path: ", NULL, true);
}

template < typename T>
void ValidateInput(T &input, string outputMessage, int loop, bool isChar)
{
    string errorMessage = "Invalid input! Please input a single letter!";
    
    if (!isChar)
    {
        errorMessage = "Invalid input! Please input a positive integer!";
    }

    do
    {
        if (loop != NULL)
        {
            cout << outputMessage << loop << ": ";
        }
        else
        {
            cout << outputMessage;
        }
        

        cin >> input;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system("cls");
            cout << errorMessage << endl;
        }
        else if (cin.get() != '\n')
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system("cls");
            cout << errorMessage << endl;
        }
        if (isChar)
        {
                if (input < 'A' || input > 'z')
                {
                    cin.clear();
                    system("cls");
                    cout << errorMessage << endl;
                }
                else if (input < 'a')
                {
                    input = tolower(input);
                    break;
                }
                else
                {
                    break;
                }
        }
        else
        {
            if (input <= 0)
            {
                cin.clear();
                system("cls");
                cout << errorMessage << endl;
            }
            else
            {
                break;
            }
        }
    } while (true);
}

Node CreateNewEdge(char x, char y, int cost) {
    return Node(x, y, cost);
}

void AstarSearch(list<Node> edges, char start, char goal)
{
    list<Node> openList; //list of connected nodes we still have to visit
    list<Node> closedList; //list of nodes we have fully explored
    /*
    //add the start node to the openList
    openList.push_back(start);

    while (!openList.empty())
    {
        if (openList.front() == goal)
        {
            break;
        }
    }
    */
}

void DisplayResults(char start, char goal, int cost) {
    //add in cout of path
    system("cls");
    cout << "RESULTS: " << endl;
    cout << " * Starting position: " << start << endl;
    cout << " * Ending position: " << goal << endl;
    cout << " * Minimum cost to end position: " << cost << endl;
}