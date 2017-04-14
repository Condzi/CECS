/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <memory>

#include "Config.hpp"

namespace ecs
{
	/*
	===============================================================================
		Wraps component data into structure with useful informations like does its
		parent entity wish delete and owner entity id. Note: data is a shared_ptr,
		so you must use static_cast<ComponentType>(componentWrapper.data) before
		use. If ownerEntityID is equal to UNASSIGNED_ENTITY_ID, use ResetComponent
		method to be sure that component is clear.

	===============================================================================
	*/
	struct componentWrapper_t final
	{
		entityID_t ownerEntityID;
		bool wishDelete;
		const size_t hashCode;
		std::shared_ptr<void> data;

		componentWrapper_t( size_t hash, entityID_t owner = UNASSIGNED_ENTITY_ID) :
			ownerEntityID( owner ),
			wishDelete( false ),
			hashCode( hash )
		{}

		// If ownerEntityID is not UNASSIGNED_ENTITY_ID you must use force argument to reset component.
		template<class ComponentType, class ...Args>
		void ResetComponent( Args&& ...args, bool force = false )
		{
			if ( this->ownerEntityID != UNASSIGNED_ENTITY_ID && force )
				if ( typeid( ComponentType ).hash_code == this->hashCode )
					*this->data = ComponentType( std::forward( args )... );
		}
	};
}