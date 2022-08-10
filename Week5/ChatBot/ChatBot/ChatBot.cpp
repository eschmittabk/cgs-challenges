// ChatBot.cpp : This file contains the 'main' function. Program execution begins and ends there.
// This is the SERVER side of the chat bot. Only open 1 server. Please open at least 2 clients to chat.
// Ws2_32.lib, Winmm.lib

#include <enet/enet.h>
#include <iostream>
#include <thread>
#include <string>

using namespace std;

int numClients = 0;
bool quitServer = false;
ENetAddress address;
ENetHost* server = nullptr;

void CreateServer(string hostName, enet_uint16 portNum);
void ManageServerThread(ENetHost* host);
void ManageServer(ENetHost* host);
void SendPacket(ENetHost* host, ENetPeer* peer, string message);
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

    cout << "Server created. Press esc at anytime to exit the program." << endl << endl; 
    //we want this in a thread
    std::thread ServerThread(ManageServerThread, std::ref(server));

    //join?
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
            SendPacket(host, event.peer, "Welcome to the chat room!\n");
            cout << "Client connected from "
                << event.peer->address.host << ":"
                << event.peer->address.port << endl;
            /* Store any relevant client information here. */
            ++numClients;
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            cout << (char*)event.packet->data << endl;

            //When we receive a message, we pass that message along to the clients
            //SendPacket(host, event.peer, (char*)event.packet->data);

            for (int i = 0; i < (int)host->peerCount; ++i)
            {
                if ((&(host->peers[i]) != event.peer) && (&(host->peers[i]) != nullptr))
                {
                    SendPacket(host, &(host->peers[i]), (char*)event.packet->data);
                }
            }

            /* Clean up the packet now that we're done using it. */
            enet_packet_destroy(event.packet);
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

void SendPacket(ENetHost* host, ENetPeer* peer, string message)
{
    if (message.length() != 0)
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
        //enet_host_broadcast(host, 0, packet);
        
        enet_peer_send(peer, 0, packet);

        //enet_host_service() //also works
        enet_host_flush(host);
    }

}

//void ClientDisconnect(ENetPeer* peer) //kick someone?
//{
//    ENetEvent event;
//
//    enet_peer_disconnect(peer, 0);
//    /* Allow up to 3 seconds for the disconnect to succeed
//     * and drop any packets received packets.
//     */
//    while (enet_host_service(client, &event, 3000) > 0)
//    {
//        switch (event.type)
//        {
//        case ENET_EVENT_TYPE_RECEIVE:
//            enet_packet_destroy(event.packet);
//            break;
//        case ENET_EVENT_TYPE_DISCONNECT:
//            puts("Disconnection succeeded.");
//            return;
//        }
//    }
//    /* We've arrived here, so the disconnect attempt didn't */
//    /* succeed yet.  Force the connection down.             */
//    enet_peer_reset(peer);
//}