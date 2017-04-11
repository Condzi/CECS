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

		auto entity = system.CreateEntity();
		auto entity2 = system.CreateEntity();

		system.AddComponent<base_t>( entity );
		system.AddComponent<inherit_t>( entity2 );

		auto handle = system.GetComponent<inherit_t>( entity2 );
		std::static_pointer_cast<inherit_t>( handle.data )->SayHello();
	}

	std::cin.get();
	return 0;
}