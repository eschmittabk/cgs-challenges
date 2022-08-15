// NumberGuesser.cpp : This file contains the 'main' function. Program execution begins and ends there.
// This is the SERVER side of the chat bot. Only open 1 server. Please open at least 2 clients to chat.
// Ws2_32.lib, Winmm.lib

#include <enet/enet.h>
#include <iostream>
#include <thread>
#include <string>

#include "PacketHeader.h"

using namespace std;

int numClients = 0;
int randomNumber;
bool quitServer = false;
ENetAddress address;
ENetHost* server = nullptr;

void CreateServer(string hostName, enet_uint16 portNum);
void ManageServerThread(ENetHost* host);
void ManageServer(ENetHost* host);
void HandleReceivePacket(ENetHost* host, const ENetEvent& event);
void SendConnectionPacket(ENetHost* host, ENetPeer* peer, bool connection);
void SendPeerConnectionPacket(ENetHost* host, ENetPeer* peer, string name, bool connection);
void SendGuessPacket(ENetHost* host, ENetPeer* peer, bool correctGuess, int clientGuess);
void SendPeerGuessPacket(ENetHost* host, ENetPeer* peer, string winnerName, bool correctGuess, int clientGuess);
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

    //can ask user for host name and port number
    string hostName = "127.0.0.1";
    enet_uint16 portNum = 1234;

    //Creating an ENet server
    cout << "Creating Server..." << endl;
    CreateServer(hostName, portNum);

    cout << "Server created." << endl;

    //TODO: Pick a random number 1-100
    srand(static_cast<int>(time(nullptr)));
    randomNumber = rand() % 100 + 1;

    cout << "The number to guess is: " << randomNumber << endl;

    //we want this in a thread
    std::thread ServerThread(ManageServerThread, std::ref(server));

    //join
    ServerThread.join();

    if (server != nullptr)
    {
        enet_host_destroy(server);
    }

    return EXIT_SUCCESS;
}

void CreateServer(string hostName, enet_uint16 portNum)
{
    /* Bind the server to the default localhost.     */
    /* A specific host address can be specified by   */
     //enet_address_set_host (&address, hostName.c_str()); 
    address.host = ENET_HOST_ANY;

    // Bind the server to portNum (in this case 1234). 
    address.port = portNum;
    server = enet_host_create(&address /* the address to bind the server host to */,
        32      /* allow up to 32 clients and/or outgoing connections */,
        2      /* allow up to 2 channels to be used, 0 and 1 */,
        0      /* assume any amount of incoming bandwidth */,
        0      /* assume any amount of outgoing bandwidth */);

    if (server == nullptr)
    {
        fprintf(stderr,
            "An error occurred while trying to create an ENet server host.\n");
        exit(EXIT_FAILURE);
    }
}

void ManageServerThread(ENetHost* host)
{
    while (!quitServer)
    {
        ManageServer(host);
    }
}

void ManageServer(ENetHost* host)
{
    ENetEvent event;
    /* Wait up to 1000 milliseconds for an event. */
    while (enet_host_service(host, &event, 1000) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            SendConnectionPacket(host, event.peer, true);
            cout << "Client connected from "
                << event.peer->address.host << ":"
                << event.peer->address.port << endl;
            /* Store any relevant client information here. */
            ++numClients;
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            HandleReceivePacket(host, event);
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            cout << "Client disconnected." << endl;
            /* Reset the peer's client information. */
            event.peer->data = NULL;
            --numClients;

            if (numClients == 0)
            {
                cout << "Lobby is empty! End Server!" << endl;
                quitServer = true;
            }
            break;
        }
    }
}

void HandleReceivePacket(ENetHost* host, const ENetEvent& event)
{
    GamePacket* RecGamePacket = (GamePacket*)(event.packet->data);
    if (RecGamePacket)
    {
        if (RecGamePacket->Type == PHT_IsConnected)
        {
            IsConnectedPacket* isConnected = (IsConnectedPacket*)(event.packet->data);
            if (isConnected->status)
            {
                //When we receive a message, we pass that message along to the clients
                //SendConnectionPacket(host, event.peer, true);

                for (int i = 0; i < (int)host->peerCount; ++i)
                {
                    if ((&(host->peers[i]) != event.peer) && (&(host->peers[i]) != nullptr))
                    {
                        SendPeerConnectionPacket(host, &(host->peers[i]), isConnected->name, true);
                    }
                }
            }
            else if (!isConnected->status)
            {
                //When we receive a message, we pass that message along to the clients
                SendConnectionPacket(host, event.peer, false);

                for (int i = 0; i < (int)host->peerCount; ++i)
                {
                    if ((&(host->peers[i]) != event.peer) && (&(host->peers[i]) != nullptr))
                    {
                        SendPeerConnectionPacket(host, &(host->peers[i]), isConnected->name, false);
                    }
                }
            }
        }
        else if (RecGamePacket->Type == PHT_MakeGuess)
        {
            MakeGuessPacket* guess = (MakeGuessPacket*)(event.packet->data);
            if (guess)
            {
                cout << guess->name << ": " << guess->guess << endl;
                if (guess->guess == randomNumber)
                {
                    SendGuessPacket(host, event.peer, true, guess->guess);

                    for (int i = 0; i < (int)host->peerCount; ++i)
                    {
                        if ((&(host->peers[i]) != event.peer) && (&(host->peers[i]) != nullptr))
                        {
                            SendPeerGuessPacket(host, &(host->peers[i]), guess->name, true, guess->guess);
                        }
                    }
                }
                else
                {
                    SendGuessPacket(host, event.peer, false, guess->guess);

                    for (int i = 0; i < (int)host->peerCount; ++i)
                    {
                        if ((&(host->peers[i]) != event.peer) && (&(host->peers[i]) != nullptr))
                        {
                            SendPeerGuessPacket(host, &(host->peers[i]), guess->name, false, guess->guess);
                        }
                    }
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
    
    enet_host_flush(server);
}

void SendConnectionPacket(ENetHost* host, ENetPeer* peer, bool connection)
{
    IsConnectedPacket* isConnected = new IsConnectedPacket();
    isConnected->status = connection;
    ENetPacket* packet = enet_packet_create(isConnected,
                         sizeof(*isConnected), //added a * into sizeof(IsDeadPacket) to handle the dynamically sized string
                         ENET_PACKET_FLAG_RELIABLE);

    /* One could also broadcast the packet by         */
    //enet_host_broadcast(host, 0, packet);
    enet_peer_send(peer, 0, packet);

    /* One could just use enet_host_service() instead. */
    //enet_host_service();
    enet_host_flush(host);
    delete isConnected;
}

void SendPeerConnectionPacket(ENetHost* host, ENetPeer* peer, string name, bool connection)
{
    PeerConnectedPacket* peerConnected = new PeerConnectedPacket();
    peerConnected->status = connection;
    peerConnected->name = name;
    ENetPacket* packet = enet_packet_create(peerConnected,
                         sizeof(*peerConnected), //added a * into sizeof(IsDeadPacket) to handle the dynamically sized string
                         ENET_PACKET_FLAG_RELIABLE);

    /* One could also broadcast the packet by         */
    //enet_host_broadcast(host, 0, packet);
    enet_peer_send(peer, 0, packet);

    /* One could just use enet_host_service() instead. */
    //enet_host_service();
    enet_host_flush(host);
    delete peerConnected;
}

void SendGuessPacket(ENetHost* host, ENetPeer* peer, bool correctGuess, int clientGuess)
{
    MakeGuessPacket* guess = new MakeGuessPacket();
    guess->guess = clientGuess;
    guess->isCorrect = correctGuess;
    ENetPacket* packet = enet_packet_create(guess,
                         sizeof(*guess), //added a * into sizeof(IsDeadPacket) to handle the dynamically sized string
                         ENET_PACKET_FLAG_RELIABLE);

    /* One could also broadcast the packet by         */
    //enet_host_broadcast(host, 0, packet);
    enet_peer_send(peer, 0, packet);

    /* One could just use enet_host_service() instead. */
    //enet_host_service();
    enet_host_flush(host);
    delete guess;
}

void SendPeerGuessPacket(ENetHost* host, ENetPeer* peer, string winnerName, bool correctGuess, int clientGuess)
{
    PeerGuessPacket* guess = new PeerGuessPacket();
    guess->name = winnerName;
    guess->guess = clientGuess;
    guess->isCorrect = correctGuess;
    ENetPacket* packet = enet_packet_create(guess,
                         sizeof(*guess), //added a * into sizeof(IsDeadPacket) to handle the dynamically sized string
                         ENET_PACKET_FLAG_RELIABLE);

    /* One could also broadcast the packet by         */
    //enet_host_broadcast(host, 0, packet);
    enet_peer_send(peer, 0, packet);

    /* One could just use enet_host_service() instead. */
    //enet_host_service();
    enet_host_flush(host);
    delete guess;
}