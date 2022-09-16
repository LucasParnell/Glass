#pragma once
#include "IComponent.h"

class Metadata : public IComponent {
public:
	Metadata() { componentType = METADATA; }
	std::string name;
};