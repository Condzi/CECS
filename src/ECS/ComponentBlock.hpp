/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <vector>

#include <ECS/Config.hpp>
#include <ECS/ComponentWrapper.hpp>

namespace ecs
{
	namespace internal
	{
		/*
		===============================================================================
			Component Block data type. It has unique 'hashCode' of types of components
			that are stored. It also has the data vector of componentWrapper_t.

		===============================================================================
		*/
		struct componentBlock_t final
		{
			size_t hashCode;
			std::vector<componentWrapper_t> data;

			componentBlock_t()
			{}

			template<class ComponentType>
			void ReserveComponents( size_t size )
			{
				for ( size_t i = 0; i < size; i++ )
				{
					componentWrapper_t component;
					component.ownerEntityID = UNASSIGNED_ENTITY_ID;
					component.wishDelete = false;
					component.data = std::make_shared<ComponentType>();
					this->data.push_back( component );
				}
			}

			bool HasFreeSpace()
			{
				for ( auto& componentWrapper : this->data )
					if ( componentWrapper.ownerEntityID == UNASSIGNED_ENTITY_ID )
						return true;

				return false;
			}

			componentWrapper_t& GetFreeComponentWrapper()
			{
				for ( auto& wrapper : this->data )
					if ( wrapper.ownerEntityID == UNASSIGNED_ENTITY_ID )
						return wrapper;

				return componentWrapper_t();
			}
		};
	}
}