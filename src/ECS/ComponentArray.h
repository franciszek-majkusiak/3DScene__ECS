#pragma once
#include "types.h"
#include <unordered_map>
#include <array>
#include <iostream>


namespace ecs 
{
	class IComponentArray
	{
	public:
		virtual void EntityDestroyed(Entity entity) {};
	};

	template<typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		void InsertData(Entity entity, T component)
		{
			if (m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end())
			{
				fprintf(stderr, "Component already Added", entity);
				exit(EXIT_FAILURE);
			}


			size_t newIndex = m_Size;
			m_EntityToIndexMap[entity] = newIndex;
			m_IndexToEntityMap[newIndex] = entity;
			m_ComponentArray[newIndex] = component;
			m_Size++;
		}

		void RemoveData(Entity entity)
		{
			if (m_EntityToIndexMap.find(entity) == m_EntityToIndexMap.end())
			{
				fprintf(stderr, "Component does not exists", entity);
				exit(EXIT_FAILURE);
			}


			size_t indexOfRemovedEntity = m_EntityToIndexMap[entity];
			size_t indexOfLastElement = m_Size - 1;
			m_ComponentArray[indexOfRemovedEntity] = m_ComponentArray[indexOfLastElement];

			// Update map to point to moved spot
			Entity entityOfLastElement = m_IndexToEntityMap[indexOfLastElement];
			m_EntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
			m_IndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

			m_EntityToIndexMap.erase(entity);
			m_IndexToEntityMap.erase(indexOfLastElement);

			m_Size--;
		}

		T& GetData(Entity entity)
		{
			if (m_EntityToIndexMap.find(entity) == m_EntityToIndexMap.end())
			{
				fprintf(stderr, "Entity does not have such component");
				exit(EXIT_FAILURE);
			}



			return m_ComponentArray[m_EntityToIndexMap[entity]];
		}

		bool CheckComponent(Entity entity)
		{
			if (m_EntityToIndexMap.find(entity) == m_EntityToIndexMap.end())
			{
				return false;
			}

			return true;
		}

		void EntityDestroyed(Entity entity) override
		{
			if (m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end())
			{
				RemoveData(entity);
			}
		}

	private:
		// The packed array of components (of generic type T),
		// set to a specified maximum amount, matching the maximum number
		// of entities allowed to exist simultaneously, so that each entity
		// has a unique spot.
		std::array<T, MAX_ENTITIES> m_ComponentArray;

		// Map from an entity ID to an array index.
		std::unordered_map<Entity, size_t> m_EntityToIndexMap;

		// Map from an array index to an entity ID.
		std::unordered_map<size_t, Entity> m_IndexToEntityMap;

		// Total size of valid entries in the array.
		size_t m_Size;
	};
}
