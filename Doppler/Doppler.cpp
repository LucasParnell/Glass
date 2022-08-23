#include "pch.h"
#include "Doppler.h"
#include "Globals.h"
#include "Metadata.h"
#include "glm/vec3.hpp"

//Using macros because i cant pass types to a template in runtime (obviously)
#define DPCAST(Type, Args) std::dynamic_pointer_cast<Type>(Args)
#define GETCOMPONENT(Type, ID) DPCAST(Type, Globals::mComponentManager->GetComponentPtr(ID));


uint32_t Doppler::CreateEngineInstance() {

	MLOG(LOG_INFO, "Hello World");
	
	//Below is just a test of my Entity-Component implementation
	//Will simplify down the line, want to get things working first.

	Entity Player;
	Player = Globals::mEntityManager->CreateEntity();
	//Assign Metadata component to player
	Component transform = Globals::mComponentManager->RegisterComponent<Transform>();
	Component metadata = Globals::mComponentManager->RegisterComponent<Metadata>();

	//Get the actual component struct from the registered ID
	std::shared_ptr<Metadata> meta = GETCOMPONENT(Metadata, metadata)

	meta.get()->name = std::string("Player");

	//Add components to the 'Player'
	Globals::mComponentManager->AddComponent(Player, transform);
	Globals::mComponentManager->AddComponent(Player, metadata);

	//Test of the GetParent function
	MLOG(LOG_INFO, "'meta' is owned by Entity " + Globals::mComponentManager->GetParent(metadata));

	
	return 0;
}