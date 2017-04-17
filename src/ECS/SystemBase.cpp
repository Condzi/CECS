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
			for ( auto it = block.data.begin(); it != block.data.end(); it++ )
				if ( it->ownerEntityID == entity )
				{
					it->ownerEntityID = UNASSIGNED_ENTITY_ID;
					it->wishDelete = false;
					break;
				}

		for ( auto it = this->entitiesAttributes.begin(); it != entitiesAttributes.end(); it++ )
			if ( it->entityID == entity )
			{
				this->entitiesAttributes.erase( it );
				return true;
			}

		ECS_ASSERT( false, "Cannot find Entity in entitiesAttributes vector for unknow reason" );
		return false;
	}

	bool SystemBase::SetEntityWishDelete( entityID_t entity, bool val )
	{
		if ( entity == UNASSIGNED_ENTITY_ID || !this->isEntityInSystem( entity ) )
			return false;

		for ( auto it = this->entitiesAttributes.begin(); it != entitiesAttributes.end(); it++ )
			if ( it->entityID == entity )
			{
				it->wishDelete = val;
				return true;
			}

		ECS_ASSERT( false, "Cannot find Entity in entitiesAttributes vector for unknow reason" );
		return false;
	}

	void SystemBase::ClearAll()
	{
		this->entitiesAttributes.clear();
		this->componentsHashCodes.clear();
		this->componentsBlocks.clear();
	}

	void SystemBase::RemoveAllThatWishToDelete()
	{
		for ( auto it = this->entitiesAttributes.begin(); it != this->entitiesAttributes.end(); )
		{
			if ( it->wishDelete )
				it = this->entitiesAttributes.erase( it );
			else
				it++;
		}

		for ( auto vecIt = this->componentsBlocks.begin(); vecIt != this->componentsBlocks.end(); )
		{
			for ( auto blockIt = vecIt->data.begin(); blockIt != vecIt->data.end(); blockIt++ )
			{
				if ( blockIt->wishDelete )
				{
					blockIt->wishDelete = false;
					blockIt->ownerEntityID = UNASSIGNED_ENTITY_ID;
				}
			}

			if ( vecIt->data.empty() )
				vecIt = this->componentsBlocks.erase( vecIt );
			else
				vecIt++;
		}

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
		for ( auto it = this->componentsBlocks.begin(); it != this->componentsBlocks.end(); it++ )
			if ( it->hashCode == componentHashCode )
			{
				found = true;
				componentBlockPosition = it;
			}

		if ( !componentBlockPosition->HasFreeSpace() ||
			!found )
			return true;

		return false;
	}

	componentWrapper_t SystemBase::addToBlock( entityID_t entity, size_t componentHashCode )
	{
		auto componentBlockPosition = this->componentsBlocks.begin();
		for ( auto it = this->componentsBlocks.begin(); it != this->componentsBlocks.end(); it++ )
			if ( it->hashCode == componentHashCode )
				componentBlockPosition = it;

		for ( const auto& component : componentBlockPosition->data )
			if ( component.ownerEntityID == entity )
				return componentWrapper_t( 0 );

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
