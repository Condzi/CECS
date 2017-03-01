/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <memory>
#include <vector>

#include <ECS/Config.hpp>

namespace ecs
{
	namespace internal
	{
		/*
		===============================================================================
			Component Block data type. It has unique 'hashCode' of types of components
			that are stored.
			Note:
			data is vector of shared_ptr of void - you must cast it up using
			std::dynamic_pointer_cast (or std::static_pointer_cast, I have to check
			it later).

		===============================================================================
		*/
		struct componentBlock_t final
		{
			componentBlock_t()
			{
				// reserving memory for components - it won't allocate new memory for every single data.push_back = time saved during gameplay
				data.reserve( MAX_COMPONENT_BLOCK_SIZE );
			}

			size_t hashCode;
			std::vector<std::shared_ptr<void>> data;
		};
	}
}