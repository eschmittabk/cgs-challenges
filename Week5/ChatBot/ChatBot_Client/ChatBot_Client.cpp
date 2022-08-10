// ChatBot_Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
// This is the CLIENT side of the chat bot. Please open at least 2 clients to chat.
// Ws2_32.lib, Winmm.lib

#include <enet/enet.h>
#include <iostream>
#include <thread>
#include <string>
#include <algorithm>

using namespace std;

string clientName;
bool clientQuit = false;
ENetAddress address;
ENetHost* client = nullptr;

void CreateClient();
void ConnectToServer(string hostName, enet_uint16 portNum);
void ManageClientThread(ENetHost* host);
void ManageClient(ENetHost* host);
string GetClientMessage();
void EraseLine();
void SendPacketThread(ENetHost* host);
void SendPacket(ENetHost* host, string message);
//void ClientDisconnect(ENetPeer* peer);

int main(int argc, char** argv) //ask about this tomorrow
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
    //std::thread ServerThread;

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
        SendPacket(client, clientName + " joined the chat room!");
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
        case ENET_EVENT_TYPE_RECEIVE:
            EraseLine();
            cout << (char*)event.packet->data << endl;
            cout << clientName << ": ";
            //Clean up the packet now that we're done using it
            enet_packet_destroy(event.packet);
            break;
        }
    }
}

string GetClientMessage()
{
    //Prompt user for message
    string message;
    getline(cin, message);
    cout << clientName << ": ";
   
    return message;
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
        message = clientName + ": " + message;
        SendPacket(host, message);
    }
}

void SendPacket(ENetHost* host, string message)
{
    string quit = message;
    string quitMessage = clientName + ": quit";
    transform(quitMessage.begin(), quitMessage.end(), quitMessage.begin(), ::tolower);
    transform(quit.begin(), quit.end(), quit.begin(), ::tolower);
    
    if (quit == quitMessage)
    {
        EraseLine();
        cout << "You have left the chat room. Goodbye!" << endl;
        SendPacket(client, clientName + " left the chat room!");
        clientQuit = true;
    }
    else if (message.length() != 0)
    {
        /* Create a reliable packet of size 7 containing "packet\0" */
        ENetPacket* packet = enet_packet_create(message.c_str(),
                             strlen(message.c_str()) + 1,
                             ENET_PACKET_FLAG_RELIABLE);
        /* Extend the packet so and append the string "foo", so it now */
        /* contains "packetfoo\0"                                      */
        //enet_packet_resize(packet, strlen("packetfoo") + 1);
        //strcpy(&packet->data[strlen("packet")], "foo");
        /* Send the packet to the peer over channel id 0. */
        /* One could also broadcast the packet by         */
        enet_host_broadcast(host, 0, packet);
        
        //enet_peer_send(host->peers, 0, packet);

        //enet_host_service() //also works
        enet_host_flush(host);
    }

}