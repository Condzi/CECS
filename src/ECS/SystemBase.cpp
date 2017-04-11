/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#include <ECS/SystemBase.hpp>

namespace ecs
{
	entityID_t SystemBase::CreateEntity()
	{
		entitiesAttributes.emplace_back();
		entitiesAttributes.back().entityID = this->entitiesAttributes.size();
		return entitiesAttributes.back().entityID = this->entitiesAttributes.size();
	}

	bool SystemBase::DeleteEntity( entityID_t entity )
	{
		if ( entity == UNASSIGNED_ENTITY_ID || !this->isEntityInSystem( entity ) )
			return false;

		for ( auto& block : this->componentsBlocks )
			for(auto& component : block.data )
				if ( component.ownerEntityID == entity )
				{
					component.ownerEntityID = UNASSIGNED_ENTITY_ID;
					component.data.reset();
					component.wishDelete = false;
				}

		for ( auto it = this->entitiesAttributes.begin(); it != entitiesAttributes.end(); it++ )
			if ( it->entityID == entity )
			{
				this->entitiesAttributes.erase( it );
				return true;
			}

		// Temporary, in future it'll be something like ECS_ASSERT
		std::cerr << "\nASSERT: cannot find entity in entitiesAttributes for some reason\n" ;
		abort();

		return false;
	}

	bool SystemBase::isComponentRegistered( size_t componentHashCode )
	{
		return (
			std::find( this->componentsHashCodes.begin(), this->componentsHashCodes.end(), componentHashCode )
			!= this->componentsHashCodes.end()
			);
	}

	void SystemBase::registerComponent( size_t componentHashCode )
	{
		this->componentsHashCodes.push_back( componentHashCode );
	}

	bool SystemBase::isCurrentBlockOverloaded( size_t componentHashCode )
	{
		// We want to allocate new block if there is no any
		if ( !this->componentsBlocks.size() )
			return true;

		bool found = false;
		auto componentBlockPosition = this->componentsBlocks.begin();
		for ( auto i = this->componentsBlocks.begin(), tooFar = this->componentsBlocks.end(); i != tooFar; i++ )
			if ( i->hashCode == componentHashCode )
			{
				found = true;
				componentBlockPosition = i;
			}

		if ( !componentBlockPosition->HasFreeSpace() ||
			componentBlockPosition->hashCode != componentHashCode )
			return true;

		return false;
	}

	componentWrapper_t SystemBase::addToBlock( entityID_t entity, size_t componentHashCode )
	{
		auto componentBlockPosition = this->componentsBlocks.begin();
		for ( auto i = this->componentsBlocks.begin(), tooFar = this->componentsBlocks.end(); i != tooFar; i++ )
			if ( i->hashCode == componentHashCode )
				componentBlockPosition = i;

		for ( const auto& component : componentBlockPosition->data )
			if ( component.ownerEntityID == entity )
				return componentWrapper_t();

		auto& component = *componentBlockPosition->GetFreeComponentWrapper();
		component.ownerEntityID = entity;

		return component;
	}

	bool SystemBase::isEntityInSystem( entityID_t id )
	{
		for ( const auto& attrib : this->entitiesAttributes )
			if ( attrib.entityID == id )
				return true;

		return false;
	}
}
