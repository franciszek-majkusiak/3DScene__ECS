#pragma once
#include "System.h"
#include <unordered_map>
#include <memory>

namespace ecs
{
	class SystemManager
	{
	public:
		template<typename T>
		std::shared_ptr<T> RegisterSystem()
		{
			const char* typeName = typeid(T).name();
			std::unordered_map<const char*, std::shared_ptr<System>>::iterator systemIt;
			if ((systemIt = m_Systems.find(typeName)) != m_Systems.end())
			{
				fprintf(stderr, "System already registered\n");
				return std::static_pointer_cast<T>(systemIt->second);
				//exit(EXIT_FAILURE);
			}



			auto system = std::make_shared<T>();
			m_Systems.insert({ typeName, system });
			return system;
		}

		template<typename T>
		bool IsSystemRegistered()
		{
			const char* typeName = typeid(T).name();

			if (m_Systems.find(typeName) != m_Systems.end())
			{
				return true;
			}
			return false;
		}

		/*template<typename T>
		std::shared_ptr<T> GetSystem()
		{
			const char* typeName = typeid(T).name();
			std::shared_ptr<T> system;
			if (system = m_Systems.find(typeName) != m_Systems.end())
			{
				fprintf(stderr, "System does not exist");
				exit(EXIT_FAILURE);
			}

			return system;
		}*/

		template<typename T>
		void SetSignature(Signature signature)
		{
			const char* typeName = typeid(T).name();

			if (m_Systems.find(typeName) == m_Systems.end())
			{
				fprintf(stderr, "System was not registered");
				exit(EXIT_FAILURE);
			}



			m_Signatures.insert({ typeName, signature });
		}

		void EntityDestroyed(Entity entity)
		{
			for (auto const& pair : m_Systems)
			{
				auto const& system = pair.second;

				system-> Entities.erase(entity);
			}
		}

		void EntitySignatureChanged(Entity entity, Signature entitySignature)
		{
			for (auto const& pair : m_Systems)
			{
				auto const& type = pair.first;
				auto const& system = pair.second;
				auto const& systemSignature = m_Signatures[type];

				if ((entitySignature & systemSignature) == systemSignature)
				{
					system-> Entities.insert(entity);
				}
				else
				{
					system-> Entities.erase(entity);
				}
			}
		}

	private:
		std::unordered_map<const char*, Signature> m_Signatures{};

		std::unordered_map<const char*, std::shared_ptr<System>> m_Systems{};
	};
}