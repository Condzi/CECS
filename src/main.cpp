/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

/*
	Testing playground.
*/

#include <iostream>
#include <memory>
#include <ECS/ECS.hpp>

#include "example/Time/Clock.hpp"

namespace unitTest
{
	struct position final
	{
		float x, y;

		position( float xx = 0.0f, float yy = 0.0f ) :
			x( xx ), y( yy )
		{}
	};

	struct weight final
	{
		float mass;

		weight( float m = 0.0f ) :
			mass( m )
		{}
	};

	void applyGravity( ecs::SystemBase& system, ecs::componentWrapper_t& component, float value )
	{
		auto positionComponent = std::static_pointer_cast<position>( component.data );
		std::shared_ptr<weight> weightComponent;

		{
			auto weightComponentWrapper = system.GetComponent<weight>( component.ownerEntityID );
			if ( weightComponentWrapper.ownerEntityID == ecs::UNASSIGNED_ENTITY_ID )
				return;
			weightComponent = std::static_pointer_cast<weight>( weightComponentWrapper.data );
		}

		positionComponent->y += weightComponent->mass * value;
	}

	void print( ecs::SystemBase&, ecs::componentWrapper_t& component )
	{
		auto positionComponent = std::static_pointer_cast<position>( component.data );

		std::cout << "X( " << positionComponent->x << " ) Y( " << positionComponent->y << " )\n";
	}
}

void testA();
void testB();
void testC();

int main()
{
	int opt = 0;

	do
	{
		std::cin >> opt;
		if ( opt == 1 )
			testA();
		else if ( opt == 2 )
			testB();
		else if ( opt == 3 )
			testC();
		std::cin.get();
	} while ( opt != 0 );

}

void testA()
{
	std::cout << "Test A begin\n";
	ecs::SystemBase system;

	auto entity = system.CreateEntity();
	system.AddComponent<unitTest::position>( entity );
	system.AddComponent<unitTest::weight>( entity );

	auto positionWrapper = system.GetComponent<unitTest::position>( entity );
	auto position = std::static_pointer_cast<unitTest::position>( positionWrapper.data );
	position->y = 10.0f;

	auto weightWrapper = system.GetComponent<unitTest::weight>( entity );
	auto weight = std::static_pointer_cast<unitTest::weight>( weightWrapper.data );
	weight->mass = 2.0f;

	std::function<void( ecs::SystemBase&, ecs::componentWrapper_t&, float )> gravityFunc = unitTest::applyGravity;
	std::function<void( ecs::SystemBase&, ecs::componentWrapper_t& )> printFunc = unitTest::print;

	system.ForEach<unitTest::position>( printFunc );
	std::cout << "Applying gravity... (expected output is 10.0f + (2.0f * 10.0f) == 30.0f)\n";
	system.ForEach<unitTest::position>( gravityFunc, 10.0f );
	system.ForEach<unitTest::position>( printFunc );

	auto vec = system.GetAllComponentsOfType<unitTest::position>();

	std::cout << "Setting Y position to 0 and wishing it to delete... (expected output is Y( 0 ))\n";
	for ( ecs::componentWrapper_t& component : *vec )
	{
		std::static_pointer_cast<unitTest::position>( component.data )->y = 0;
		component.wishDelete = true;
	}

	system.ForEachLambda<unitTest::position>( unitTest::print );
//	system.ForEach<unitTest::position>( printFunc );
	system.RemoveAllThatWishToDelete();
	std::cout << "Deleted component (expected output is definitly not position info in next line)\n";
//	system.ForEach<unitTest::position>( printFunc );
	system.ForEachLambda<unitTest::position>( unitTest::print );
	std::cout << "Test end.\n";
}

void testB()
{
	std::cout << "Test B begin \n";

	unitTest::Clock clock;
	ecs::SystemBase system;

	for ( auto i = 0u; i < 1280; i++ )
		system.AddComponent<unitTest::position>( system.CreateEntity() );

	for ( auto i = 1u; i < ecs::MAX_COMPONENT_BLOCK_SIZE * 10; i++ )
	{
		auto componentWrapper = system.GetComponent<unitTest::position>( i );
		auto component = std::static_pointer_cast<unitTest::position>( componentWrapper.data );
		component->x = 1.0f;
		component->y = 2.0f;
	}

	auto vector = system.GetAllComponentsOfType<unitTest::position>();

	for ( ecs::componentWrapper_t& component : *vector )
	{
		if ( component.ownerEntityID == 11 )
			component.wishDelete = false;
		else
		{
			system.SetEntityWishDelete( component.ownerEntityID, true );
			component.wishDelete = true;
		}
	}

	system.RemoveAllThatWishToDelete();

	std::cout << "Time: " << clock.Restart().AsMilliseconds() << "ms\nTest ended.\n";
}

void testC()
{
	std::cout << "Test C begin \n";

	unitTest::Clock clock;
	ecs::SystemBase system;

	auto vec = system.ReserveComponentBlocks<unitTest::position>( ecs::MAX_COMPONENT_BLOCKS );

	std::cout << "Reserved " << ecs::MAX_COMPONENT_BLOCKS << " component blocks (" << ecs::MAX_COMPONENT_BLOCKS * ecs::MAX_COMPONENT_BLOCK_SIZE << " components) in " << clock.Restart().AsMilliseconds() << "ms\n";
	

	for ( ecs::internal::componentBlock_t& block : *vec )
		for ( auto& component : block.data )
			component.wishDelete = true;

	std::cout << "Iterating through them and marking them as wishDelete in " << clock.Restart().AsMilliseconds() << "ms\n";

	system.RemoveAllThatWishToDelete();

	std::cout << "Removed them using RemoveAllThatWishToDelete() method in " << clock.Restart().AsMilliseconds() << "ms\n";

	std::cout << "Test C end \n";
}