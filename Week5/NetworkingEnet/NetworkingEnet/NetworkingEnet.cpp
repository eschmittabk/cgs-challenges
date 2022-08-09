// NewtworkingEnet.cpp : This file contains the 'main' function. Program execution begins and ends there.
//http://enet.bespin.org/Tutorial.html
//https://docs.microsoft.com/en-us/windows/win32/winsock/creating-a-basic-winsock-application
//https://docs.microsoft.com/en-us/windows/win32/api/timeapi/nf-timeapi-timebeginperiod
#include <enet/enet.h>
#include <iostream>
#include <thread>
#include <string>

using namespace std;

ENetAddress address;
ENetHost* server = nullptr;
ENetHost* client = nullptr;

bool CreateServer(enet_uint16 portNum);
bool CreateClient();
void ConnectToServer(string hostName, enet_uint16 portNum);
void ManageServer(ENetHost* host);
void ManageClient(ENetHost* host);
void SendPacket(ENetHost* host);
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

    int userInput;
    cout << "1. Create Server " << endl;
    cout << "2. Create Client " << endl;
    cin >> userInput;

    //can ask user for host name and port number
    string hostName = "127.0.0.1";
    enet_uint16 portNum = 1234;
    //std::thread ServerThread;

   if(userInput == 1)
   {
        //Creating an ENet server
        cout << "Creating Server..." << endl;
        if (!CreateServer(portNum))
        {
            fprintf(stderr,
                "An error occurred while trying to create an ENet server host.\n");
            exit(EXIT_FAILURE);
        }

        //we want this in a thread
        //ServerThread= thread(ManageServer, server);

        while (1)
        {
            //Managing an ENet host
            ManageServer(server);
        }
    }
   else if (userInput == 2)
   {
       //Creating an ENet client
       cout << "Creating Client..." << endl;
       if (!CreateClient())
       {
           fprintf(stderr,
               "An error occurred while trying to create an ENet client host.\n");
           exit(EXIT_FAILURE);
       }

       ConnectToServer(hostName, portNum);
       //also want this in a thread
       while (1)
       {
           ManageClient(client);
       }
   }
   else
   {
        cout << "Invalid Input" << endl;
   }
    //join?
    if (server != nullptr)
    {
        enet_host_destroy(server);
    }

    if (client != nullptr)
    {
        enet_host_destroy(client);
    }

    return EXIT_SUCCESS;
}

bool CreateServer(enet_uint16 portNum)
{
    /* Bind the server to the default localhost.     */
    /* A specific host address can be specified by   */
    /* enet_address_set_host (& address, "x.x.x.x"); */
    address.host = ENET_HOST_ANY;

    // Bind the server to portNum (in this case 1234). 
    address.port = portNum;
    server = enet_host_create(&address /* the address to bind the server host to */,
        32      /* allow up to 32 clients and/or outgoing connections */,
        2      /* allow up to 2 channels to be used, 0 and 1 */,
        0      /* assume any amount of incoming bandwidth */,
        0      /* assume any amount of outgoing bandwidth */);

    return server != nullptr;
}

bool CreateClient()
{
    client = enet_host_create(NULL /* create a client host */,
        1 /* only allow 1 outgoing connection */,
        2 /* allow up 2 channels to be used, 0 and 1 */,
        0 /* assume any amount of incoming bandwidth */,
        0 /* assume any amount of outgoing bandwidth */);

    return client != nullptr;
}

void ConnectToServer(string hostName, enet_uint16 portNum)
{
    ENetAddress address;
    ENetEvent event;
    ENetPeer* peer;
    /* Connect to 127.0.0.1:1234. (127.0.0.1 is localhost)*/
    enet_address_set_host(&address, &hostName.front());
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
    if (enet_host_service(client, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        cout << "Connection to " << hostName << ":" << portNum << " succeeded." << endl;
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

void ManageServer(ENetHost* host)
{
    ENetEvent event;
    /* Wait up to 1000 milliseconds for an event. */
    while (enet_host_service(server, &event, 1000) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            cout << "A new client connected from "
                << event.peer->address.host << ":"
                << event.peer->address.port << endl;
            /* Store any relevant client information here. */
            event.peer->data = (void*)("Client information");
            //Sending a packet to an ENet peer
            SendPacket(host);
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            cout << "A packet of length " << event.packet->dataLength << endl
                << "containing " << (char*)event.packet->data
                << " was received from " << (char*)event.peer->data
                << " on channel " << event.channelID << endl;
            /* Clean up the packet now that we're done using it. */
            enet_packet_destroy(event.packet);

            //When we receive a message, we pass that message along to the clients
            //Call send packet with event.packet->data as the message
            //or do
            //enet_host_broadcast(server, 0, event.packet);
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            cout << (char*)event.peer->data << " disconnected." << endl;
            /* Reset the peer's client information. */
            event.peer->data = NULL;
        }
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
            cout << "A packet of length " << event.packet->dataLength << endl
                << "containing " << (char*)event.packet->data << endl;

            //Clean up the packet now that we're done using it
            enet_packet_destroy(event.packet);

            SendPacket(client);
        }
    }
}

void SendPacket(ENetHost* host)
{
    //Prompt user for message
    cout << "Message: ";
    string message;
    cin.ignore(); //needed? breaks with multiple clients
    //cout << name << ": ";
    getline(cin, message);
    
    //message = name + ": " + message;

    if (message.length() != 0)
    {
        /* Create a reliable packet of size 7 containing "packet\0" */
        ENetPacket* packet = enet_packet_create(&message.front(),
                             strlen(&message.front()) + 1,
                             ENET_PACKET_FLAG_RELIABLE);
        /* Extend the packet so and append the string "foo", so it now */
        /* contains "packetfoo\0"                                      */
        //enet_packet_resize(packet, strlen("packetfoo") + 1);
        //strcpy(&packet->data[strlen("packet")], "foo");
        /* Send the packet to the peer over channel id 0. */
        /* One could also broadcast the packet by         */
        enet_host_broadcast(host, 0, packet);
        //enet_peer_send(peers, 0, packet);

        //enet_host_service() //also works
        enet_host_flush(host);
    }
    
}

//void ClientDisconnect(ENetPeer* peer)
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