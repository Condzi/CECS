/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

/*
	Testing playground :3
*/

#include <iostream>
#include <memory>

struct base_t
{
	~base_t()
	{
		std::cout << "base_t dtor\n";
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
		std::cout << "inherit_t dtor\n";
	}
};

int main()
{
	{
		std::shared_ptr<void> ptr( std::make_shared<inherit_t>() );

		std::static_pointer_cast<inherit_t>( ptr )->SayHello();
	}


	std::cin.get();
	return 0;
}