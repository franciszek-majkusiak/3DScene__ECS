#pragma once
#include "types.h"
#include <queue>
#include <array>
#include <iostream>


namespace ecs
{
	class EntityManager
	{
	public:
		EntityManager()
		{
			for (Entity  e = 0; e < MAX_ENTITIES; e++)
				m_avaliableEntities.push(e);
		}

		Entity CreateEntity()
		{
			if (m_EnitityCount >= MAX_ENTITIES)
			{
				fprintf(stderr, "Num Of Entities: %d. No Entity slots left\n", m_EnitityCount);
				exit(EXIT_FAILURE);
			}
			Entity entity = m_avaliableEntities.front();
			m_avaliableEntities.pop();
			m_EnitityCount++;

			return entity;
		}

		void DestroyEntity(Entity entity)
		{
			if (entity >= MAX_ENTITIES)
			{
				fprintf(stderr, "Entity out of range\n");
				exit(EXIT_FAILURE);
			}

			m_avaliableEntities.push(entity);
			m_EnitityCount--;
		}

		void SetSignature(Entity entity, Signature signature)
		{
			if (entity >= MAX_ENTITIES)
			{
				fprintf(stderr, "Entity id: %d out of range\n", entity);
				exit(EXIT_FAILURE);
			}

			m_Signatures[entity] = signature;
		}

		Signature GetSignature(Entity entity)
		{
			if (entity >= MAX_ENTITIES)
			{
				fprintf(stderr, "Entity id: %d out of range\n", entity);
				exit(EXIT_FAILURE);
			}

			return m_Signatures[entity];
		}

	private:
		std::queue<Entity> m_avaliableEntities;
		std::array<Signature, MAX_ENTITIES> m_Signatures;

		std::uint32_t m_EnitityCount = 0;
	};
}
