/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <algorithm>
#include <type_traits>
#include <functional>
#include <iostream>
#include <vector>

#include <ECS/Config.hpp>
#include <ECS/ComponentBlock.hpp>
#include <ECS/EntityAttributes.hpp>
#include <ECS/Assert.hpp>

namespace ecs
{
	/*
	===============================================================================
		Base class for Entity Systems. Use CreateEntity to create it in system and
		get its unique id. Use DeleteEntity to safely remove it from System. To add
		component to entity use AddComponent() method - it returns componentWrapper_t
		structure with your component data.

	===============================================================================
	*/
	class SystemBase
	{
	public:
		SystemBase()
		{}
		virtual ~SystemBase() = default;

		entityID_t CreateEntity();
		bool DeleteEntity( entityID_t entity );

		// Returns componentWrapper_t with id UNASSIGNED_ENTITY_ID if found same
		template<class ComponentType>
		componentWrapper_t AddComponent( entityID_t entity );

		// returns componentWrapper_t with id UNASSIGNED_ENTITY_ID if doesn't found
		template<class ComponentType>
		componentWrapper_t GetComponent( entityID_t entity );
		template<class ComponentType>
		bool HasComponent( entityID_t entity );

		// Calls given function with componentWrapper reference and custom parameters for given components
		// Usage: system.ForEach<component_t>(function, additionalArgs, otherAdditionalArgs); 
		template<class ComponentType, typename ...Args>
		void ForEach( std::function<void( componentWrapper_t&, Args... )> func, Args&&... args );
		// Returns shared pointer to vector of std::reference_rapper<componentWrapper_t> with components types
		template<class ComponentType>
		std::shared_ptr<std::vector<std::reference_wrapper<componentWrapper_t>>> GetAllComponentsOfType();

		void ClearAll();

	private:
		std::vector<internal::entityAttributes_t> entitiesAttributes;
		std::vector<size_t> componentsHashCodes;
		std::vector<internal::componentBlock_t> componentsBlocks;

		bool isComponentRegistered( size_t componentHashCode );
		void registerComponent( size_t componentHashCode );
		bool isCurrentBlockOverloaded( size_t componentHashCode );
		template<class ComponentType>
		void allocateNewBlock();
		componentWrapper_t addToBlock( entityID_t entity, size_t componentHashCode );
		bool isEntityInSystem( entityID_t id );
	};

#include "SystemBase.inl"
}