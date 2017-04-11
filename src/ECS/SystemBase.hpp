/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <algorithm>
#include <type_traits>
#include <vector>

#include <ECS/Config.hpp>
#include <ECS/ComponentBlock.hpp>
#include <ECS/EntityAttributes.hpp>

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
		componentWrapper_t AddComponent( entityID_t entity )
		{
			size_t componentHashCode = typeid( ComponentType ).hash_code();

			if ( !this->isComponentRegistered( componentHashCode ) )
			{
				this->registerComponent( componentHashCode );
				return this->AddComponent<ComponentType>( entity );
			}
			if ( this->isCurrentBlockOverloaded( componentHashCode ) )
				this->allocateNewBlock<ComponentType>();

			return this->addToBlock( entity, componentHashCode );
		}

		// returns componentWrapper_t with id UNASSIGNED_ENTITY_ID if doesn't found
		template<class ComponentType>
		componentWrapper_t GetComponent( entityID_t entity )
		{
			if ( entity == 0 || !this->isEntityInSystem( entity ) )
				return componentWrapper_t();
			size_t componentHashCode = typeid( ComponentType ).hash_code();

			if ( !this->isComponentRegistered( componentHashCode ) )
				return componentWrapper_t();

			auto componentBlockPosition = this->componentsBlocks.begin();
			for ( auto i = this->componentsBlocks.begin(), tooFar = this->componentsBlocks.end(); i != tooFar; i++ )
				if ( i->hashCode == componentHashCode )
					componentBlockPosition = i;

			for ( auto& component : componentBlockPosition->data )
				if ( component.ownerEntityID == entity )
					return component;

			return componentWrapper_t();
		}

	private:
		std::vector<internal::entityAttributes_t> entitiesAttributes;
		std::vector<size_t> componentsHashCodes;
		std::vector<internal::componentBlock_t> componentsBlocks;

		bool isComponentRegistered( size_t componentHashCode );
		void registerComponent( size_t componentHashCode );
		bool isCurrentBlockOverloaded( size_t componentHashCode );
		template<class ComponentType>
		void allocateNewBlock()
		{
			this->componentsBlocks.emplace_back();
			this->componentsBlocks.back().hashCode = typeid( ComponentType ).hash_code();
			this->componentsBlocks.back().ReserveComponents<ComponentType>( MAX_COMPONENT_BLOCK_SIZE );
		}
		componentWrapper_t addToBlock( entityID_t entity, size_t componentHashCode );
		bool isEntityInSystem( entityID_t id );
	};
}