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
			{
				// reserving memory for components - it won't allocate new memory for every single data.push_back = time saved during gameplay
				data.reserve( MAX_COMPONENT_BLOCK_SIZE );
			}
		};
	}
}