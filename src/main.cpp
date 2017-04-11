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

struct base_t
{
	~base_t()
	{
//		std::cout << "base_t dtor\n";
	}
};

struct inherit_t : public base_t
{
	void SayHello()
	{
		std::cout << "Hello!\n";
	}

	~inherit_t()
	{
	//	std::cout << "inherit_t dtor\n";
	}
};

int main()
{
	{
		ecs::SystemBase system;

		for ( int i = 0; i < ecs::MAX_COMPONENT_BLOCK_SIZE * 10; i++ )
			system.AddComponent<inherit_t>( system.CreateEntity() );

		std::function<void( ecs::componentWrapper_t& wrap, int i )> func = []( ecs::componentWrapper_t& wrap, int i ) {std::static_pointer_cast<inherit_t>( wrap.data )->SayHello(); std::cout << i << "\n"; };

		system.ForEach<inherit_t>( func, 123 );
	}

	std::cin.get();
	return 0;
}