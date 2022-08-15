#include <string>

using namespace std;

#pragma once

enum PacketHeaderTypes
{
    PHT_Invalid = 0,
    PHT_IsConnected,
    PHT_MakeGuess,
    PHT_PeerGuess,
    PHT_PeerConnected,
    PHT_Count
};

struct GamePacket
{
    GamePacket() {}
    PacketHeaderTypes Type = PHT_Invalid;
};

struct IsConnectedPacket : public GamePacket
{
    IsConnectedPacket()
    {
        Type = PHT_IsConnected;
        name = "";
        status = false;
    }

    string name;
    bool status;
};

struct MakeGuessPacket : public GamePacket
{
    MakeGuessPacket()
    {
        Type = PHT_MakeGuess;
        name = "";
        guess = NULL;
        isCorrect = false;
    }

    string name;
    int guess;
    bool isCorrect;
};

struct PeerConnectedPacket : public GamePacket
{
    PeerConnectedPacket()
    {
        Type = PHT_PeerConnected;
        name = "";
        status = false;
    }

    string name;
    bool status;
};

struct PeerGuessPacket : public GamePacket
{
    PeerGuessPacket()
    {
        Type = PHT_PeerGuess;
        name = "";
        guess = NULL;
        isCorrect = false;
    }

    string name;
    int guess;
    bool isCorrect;
};