/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include "ECS/Entity.hpp"
#include "example/components/Position.hpp"
#include "example/components/Velocity.hpp"

namespace ex
{
	class BlockPrefab :
		public ecs::Entity
	{
	public:
		BlockPrefab( ecs::SystemBase& system ) :
			Entity( system )
		{}

		void SetUpComponents() override
		{
			*this->AddComponent<PositionComponent>() = PositionComponent( 5.0f, 5.0f );
			*this->AddComponent<VelocityComponent>() = VelocityComponent( 2.0f, 5.0f );
		}
	};
}