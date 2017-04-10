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

		template<class ComponentType>
		componentWrapper_t AddComponent( entityID_t entity )
		{
			size_t componentHashCode = typeid( ComponentType ).hash_code();

			if ( this->isComponentRegistered( componentHashCode ) )
			{
				this->registerComponent( componentHashCode );
				return this->AddComponent<ComponentType>( entity );
			}
			if ( this->isCurrentBlockOverloaded( componentHashCode ) )
				this->allocateNewBlock( componentHashCode );

			return this->addToBlock<ComponentType>( entity );
		}

	private:
		std::vector<internal::entityAttributes_t> entitiesAttributes;
		std::vector<size_t> componentsHashCodes;
		std::vector<internal::componentBlock_t> componentsBlocks;

		bool isComponentRegistered( size_t componentHashCode );
		void registerComponent( size_t componentHashCode );
		bool isCurrentBlockOverloaded( size_t componentHashCode );
		void allocateNewBlock( size_t componentHashCode );
		template<class ComponentType>
		componentWrapper_t addToBlock( entityID_t entity )
		{
			size_t componentHashCode = typeid( ComponentType ).hash_code();

			auto componentBlockPosition = this->componentsBlocks.begin();
			for ( auto i = this->componentsBlocks.begin(), tooFar = this->componentsBlocks.end(); i != tooFar; i++ )
				if ( i->hashCode == componentHashCode )
					componentBlockPosition = i;

			componentWrapper_t component;
			component.ownerEntityID = entity;
			component.data = std::make_shared<ComponentType>();

			componentBlockPosition->data.push_back( component );

			return component;
		}
	};
}