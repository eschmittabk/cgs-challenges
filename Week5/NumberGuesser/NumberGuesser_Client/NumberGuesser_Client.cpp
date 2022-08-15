// NumberGuesser_Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
// This is the CLIENT side of the chat bot. Please open at least 2 clients to chat.
// Ws2_32.lib, Winmm.lib

#include <enet/enet.h>
#include <iostream>
#include <thread>
#include <string>
#include <algorithm>
#include <mutex>

#include "PacketHeader.h"

using namespace std;

string clientName;
string quitMessage = "quit";
bool clientQuit = false;
mutex mtx;
ENetAddress address;
ENetHost* client = nullptr;

void CreateClient();
void ConnectToServer(string hostName, enet_uint16 portNum);
void ManageClientThread(ENetHost* host);
void ManageClient(ENetHost* host);
void HandleReceivePacket(const ENetEvent& event);
string GetClientMessage();
bool DidClientQuit(string message);
void EraseLine();
void SendPacketThread(ENetHost* host);
void SendConnectionPacket(ENetHost* host, bool connection);
void SendGuessPacket(ENetHost* host, string message);
//void ClientDisconnect(ENetPeer* peer);

int main(int argc, char** argv)
{
    //ENet Initialization
    if (enet_initialize() != 0)
    {
        fprintf(stderr, "An error occurred while initializing ENet.\n");
        cout << "An error occurred while initializing ENet." << endl;
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode; GetConsoleMode(h, &consoleMode);
    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(h, consoleMode);

    //can ask user for host name and port number
    string hostName = "127.0.0.1";
    enet_uint16 portNum = 1234;

    //Creating an ENet client
    cout << "Creating Client..." << endl;
    CreateClient();

    ConnectToServer(hostName, portNum);

    //also want this in a thread
    std::thread ManageThread(ManageClientThread, std::ref(client));

    std::thread ClientThread(SendPacketThread, std::ref(client));

    //joins
    ManageThread.join();
    ClientThread.join();

    if (client != nullptr)
    {
        enet_host_destroy(client);
    }

    return EXIT_SUCCESS;
}

void CreateClient()
{
    cout << "Please enter a username!\nThis is how others will see you in the chat room so choose wisely." << endl;
    getline(cin, clientName);

    client = enet_host_create(NULL /* create a client host */,
        1 /* only allow 1 outgoing connection */,
        2 /* allow up 2 channels to be used, 0 and 1 */,
        0 /* assume any amount of incoming bandwidth */,
        0 /* assume any amount of outgoing bandwidth */);

    if (client == nullptr)
    {
        fprintf(stderr,
            "An error occurred while trying to create an ENet client host.\n");
        exit(EXIT_FAILURE);
    }
}

void ConnectToServer(string hostName, enet_uint16 portNum)
{
    ENetAddress address;
    ENetEvent event;
    ENetPeer* peer;
    /* Connect to 127.0.0.1:1234. (127.0.0.1 is localhost)*/
    enet_address_set_host(&address, hostName.c_str());
    address.port = portNum;
    /* Initiate the connection, allocating the two channels 0 and 1. */
    peer = enet_host_connect(client, &address, 2, 0);
    if (peer == NULL)
    {
        fprintf(stderr,
            "No available peers for initiating an ENet connection.\n");
        exit(EXIT_FAILURE);
    }
    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service(client, &event, 10000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        cout << "Connection to " << hostName << ":" << portNum << " succeeded." << endl;
        //SendPacket(client, clientName + " joined the chat room!");
        SendConnectionPacket(client, true);
    }
    else
    {
        /* Either the 5 seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the 5 seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset(peer);
        cout << "Connection to " << hostName << ":" << portNum << " failed." << endl;
    }
}

void ManageClientThread(ENetHost* host)
{
    while (!clientQuit)
    {
        ManageClient(client);
    }
}

void ManageClient(ENetHost* host)
{
    ENetEvent event;
    /* Wait up to 1000 milliseconds for an event. */
    while (enet_host_service(host, &event, 1000) > 0)
    {
        switch (event.type)
        {
        case  ENET_EVENT_TYPE_CONNECT:
            cout << "Connection succeeded" << endl;
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            HandleReceivePacket(event);
            break;
        }
    }
}

void HandleReceivePacket(const ENetEvent& event)
{
    EraseLine();
    
    GamePacket* RecGamePacket = (GamePacket*)(event.packet->data);
    if (RecGamePacket)
    {
        if (RecGamePacket->Type == PHT_IsConnected)
        {
            IsConnectedPacket* isConnected = (IsConnectedPacket*)(event.packet->data);
            if (isConnected)
            {
                
                if (isConnected->status)
                {
                    mtx.lock();
                    EraseLine();
                    cout << "Welcome to the number guessing game!" << endl;
                    cout << clientName << ": ";
                    mtx.unlock();
                }
                else
                {
                    EraseLine();
                    cout << "You have left the game. Goodbye!" << endl;
                    clientQuit = true;
                }
            }
        }
        else if (RecGamePacket->Type == PHT_PeerConnected)
        {
            PeerConnectedPacket* peerConnected = (PeerConnectedPacket*)(event.packet->data);
            if (peerConnected)
            {
                mtx.lock();
                if (peerConnected->status)
                {
                    EraseLine();
                    cout << peerConnected->name << " joined the number guessing game!" << endl;
                    cout << clientName << ": ";
                }
                else
                {
                    EraseLine();
                    cout << peerConnected->name << " has left the game. Goodbye " << peerConnected->name  << "!" << endl;

                    if (!clientQuit)
                    {
                        cout << clientName << ": ";
                    }
                }
                mtx.unlock();
            }
        }
        else if (RecGamePacket->Type == PHT_MakeGuess)
        {
            MakeGuessPacket* guess = (MakeGuessPacket*)(event.packet->data);
            if (guess)
            {
                if (guess->isCorrect)
                {
                    cout << "Congrats! You have won!" << endl
                         << "Your guess of " << guess->guess
                         << " was correct." << endl;
                    //SendConnectionPacket(client, false);
                    clientQuit = true;
                }
                else
                {
                    mtx.lock();
                    cout << "Sorry, your guess of " << guess->guess
                         << " was not correct." << endl
                         << "Please try again!" << endl
                         << clientName << ": ";
                    mtx.unlock();
                }
            }
            
        }
        else if (RecGamePacket->Type == PHT_PeerGuess)
        {
            PeerGuessPacket* guess = (PeerGuessPacket*)(event.packet->data);
            if (guess)
            {
                if (guess->isCorrect)
                {
                    cout << "Sorry, the game is over!" << endl
                         << guess->name << "'s guess of " << guess->guess
                         << " was correct." << endl;
                    cout << "Game Over!" << endl;
                    SendConnectionPacket(client, false);
                }
                if (!guess->isCorrect)
                {
                    mtx.lock();
                    cout << guess->name << "'s guess of " << guess->guess
                         << " was not correct." << endl
                         << clientName << ": ";
                    mtx.unlock();
                }
            }
        }
    }
    else
    {
        cout << "Invalid Packet " << endl;
    }

    /* Clean up the packet now that we're done using it. */
    enet_packet_destroy(event.packet);
    
    //enet_host_flush(client);
}

string GetClientMessage()
{
    //Prompt user for message
    string message;
    getline(cin, message);
    cout << clientName << ": ";

    return message;
}

bool DidClientQuit(string message)
{
    string quit = message;
    transform(quitMessage.begin(), quitMessage.end(), quitMessage.begin(), ::tolower);
    transform(quit.begin(), quit.end(), quit.begin(), ::tolower);

    return (quit == quitMessage ? true : false);
}

void EraseLine()
{
    cout << "\x1b[2K"; //delete current line
    //cout << "\x1b[1A"; //move cursor up a line
    cout << "\r";      //resume cursor at the beginning of next line
}

void SendPacketThread(ENetHost* host)
{
    while (!clientQuit)
    {
        string message = GetClientMessage();

        if (DidClientQuit(message))
        {
            SendConnectionPacket(host, false);
        }
        else
        {
            SendGuessPacket(host, message);
        }
    }
}

void SendConnectionPacket(ENetHost* host, bool connection)
{
    IsConnectedPacket* isConnected = new IsConnectedPacket();
    isConnected->name = clientName;
    isConnected->status = connection;
    ENetPacket* packet = enet_packet_create(isConnected,
                         sizeof(*isConnected), //added a * into sizeof(IsDeadPacket) to handle the dynamically sized string
                         ENET_PACKET_FLAG_RELIABLE);

    /* One could also broadcast the packet by         */
    enet_host_broadcast(host, 0, packet);
    //enet_peer_send(event.peer, 0, packet);

    /* One could just use enet_host_service() instead. */
    //enet_host_service();
    enet_host_flush(host);

    delete isConnected;
}

void SendGuessPacket(ENetHost* host, string message)
{
    MakeGuessPacket* guess = new MakeGuessPacket();
    guess->name = clientName;
    guess->guess = stoi(message);
    ENetPacket* packet = enet_packet_create(guess,
                         sizeof(*guess), //added a * into sizeof(IsDeadPacket) to handle the dynamically sized string
                         ENET_PACKET_FLAG_RELIABLE);

    /* One could also broadcast the packet by         */
    enet_host_broadcast(host, 0, packet);
    //enet_peer_send(event.peer, 0, packet);

    /* One could just use enet_host_service() instead. */
    //enet_host_service();
    enet_host_flush(host);
    delete guess;
}