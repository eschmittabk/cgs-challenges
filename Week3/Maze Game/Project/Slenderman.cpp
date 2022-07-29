#include "Slenderman.h"
#include "GameplayState.h"
#include "AudioManager.h"
#include <iostream>

class GameplayState;

Slenderman::Slenderman()
	: PlacableActor(0, 0)
{

}

void Slenderman::Draw()
{
	std::cout << (char)33;
}

