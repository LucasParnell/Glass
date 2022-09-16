#pragma once
#include <memory>
struct IComponent {
	enum ComponentType {
		METADATA,
		TRANSFORM,
		MESH,
		SHADER,
		TEXTURE
	};

	ComponentType componentType;

	IComponent() {};
	virtual ~IComponent() {};
};
