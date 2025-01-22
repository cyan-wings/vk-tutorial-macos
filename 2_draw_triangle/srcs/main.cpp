#include "HelloTriangleApplication.hpp"

int	main( void )
{
	HelloTriangleApplication	app;

	try
	{
		app.run();
	}
	catch ( std::exception const &e )
	{
		std::cerr << e.what() << std::endl;
		return ( EXIT_FAILURE );
	}

	return ( EXIT_SUCCESS );
}