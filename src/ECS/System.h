#pragma once
#include "types.h"
#include <set>


namespace ecs 
{
	class System
	{
	public:
		std::set<Entity> Entities;
		Signature signature;
	};
}

