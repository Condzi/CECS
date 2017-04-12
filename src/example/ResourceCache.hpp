/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <memory>
#include <unordered_map>

#include "Logger.hpp"

namespace ex
{	/*
	===============================================================================
		Class created for holding resources. You can acces them using 'get' method.
		Because of optimalization you must use enum with defined names of 
		resources. Before you'll able to acces resources you must add them using
		'add' method. 

	===============================================================================
	*/
	template<enum resourceNames_t, class ResourceType>
	class ResourceCache final
	{
	public:
		ResourceCache()
		{}

		std::shared_ptr<ResourceType> add( resourceNames_t name, std::unique_ptr<ResourceType>& resource )
		{
			if ( !resource )
				return;

			// co robic?
			resources.insert( name, std::swap(resource) );
		}

		std::shared_ptr<ResourceType> get( resourceNames_t name );

	private:
		std::unordered_map<resourceNames_t, std::shared_ptr<ResourceType>> resources;
	};
}