#pragma once

// Source
// https://austinmorlan.com/posts/entity_component_system/
//



#include <cstdint>
#include <bitset>


namespace ecs 
{
	typedef std::uint32_t Entity;

	const Entity MAX_ENTITIES = 5000;

	typedef std::uint8_t ComponentType;

	const ComponentType MAX_COMPONENTS = 32;



	typedef std::bitset<MAX_COMPONENTS> Signature;
}
