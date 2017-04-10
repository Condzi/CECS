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
		use.

	===============================================================================
	*/
	struct componentWrapper_t final
	{
		entityID_t ownerEntityID;
		bool wishDelete;
		std::shared_ptr<void> data;

		componentWrapper_t( entityID_t owner = UNASSIGNED_ENTITY_ID ) :
			ownerEntityID( owner ),
			wishDelete( false )
		{}
	};
}