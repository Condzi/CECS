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
		for ( int i = 0; i < 10; i++ )
			 system.AddComponent<inherit_t>( system.CreateEntity() );

		std::cout << "Deleting entity 1\n";
		system.DeleteEntity( 1 );
		std::function<void( ecs::componentWrapper_t& )> func = []( ecs::componentWrapper_t& wrap ) { std::cout << wrap.ownerEntityID << "\n"; };

		system.ForEach<inherit_t>( func );

		std::cout << std::boolalpha << system.HasComponent<inherit_t>( 1 );
	}

	std::cin.get();
	return 0;
}