#include "pch.h"
#include "Doppler.h"
#include "Globals.h"
#include "Metadata.h"
#include "glm/vec3.hpp"


uint32_t Doppler::CreateEngineInstance() {
	
	//Create entity

	for (int i = 0; i < 1000; i++) {
		Entity Player;
		Player = Globals::mEntityManager->CreateEntity();
		//Assign Metadata component to player
		Component transform = Globals::mComponentManager->RegisterComponent<Transform>();
		Component metadata = Globals::mComponentManager->RegisterComponent<Metadata>();

		Metadata* meta = (Metadata*)Globals::mComponentManager->GetComponentPtr(metadata);
		Transform* tform = (Transform*)Globals::mComponentManager->GetComponentPtr(transform);

		meta->name = std::string("Player");
		tform->position = glm::vec3(0, 1, 1);

		Globals::mComponentManager->AddComponent(Player, transform);
		Globals::mComponentManager->AddComponent(Player, metadata);

		MLOG(LOG_INFO, "Entity: " + std::to_string(Player) + " created with name: " + meta->name);
	}

	//Seems to work

	return 0;
}