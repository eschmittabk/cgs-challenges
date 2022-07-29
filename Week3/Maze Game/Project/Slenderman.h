#pragma once
#include "PlacableActor.h"
//Updates to make later
//Text art welcome screen
//sounds change as you collect pages
//Easy mode: 0-2 pages. crickets
//Medium mode: 3-4 pages. low droning sound
//Hard mode: 5-6 pages. loud wind
//Extreme mode: 7 pages. mysterious and unnerving beeping sound
//End game: 8 pages. fades into silence, static if Slenderman is close.

//need walking sound, death sound and pick up paper sound

class Slenderman : public PlacableActor
{
public:
	Slenderman();

	virtual ActorType GetType() override { return ActorType::Slenderman; }
	virtual void Draw() override;
};