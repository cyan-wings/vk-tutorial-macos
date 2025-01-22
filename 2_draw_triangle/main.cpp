#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

class HelloTriangleApplication
{
	public:
		void run( void )
		{
			initWindow();
			initVulkan();
			mainLoop();
			cleanup();
		}

	private:
		uint32_t const	WIDTH = 800;
		uint32_t const	HEIGHT = 600;
		GLFWwindow		*_window;

		/*
			1. Initialise glfw.
			2. GLFW by default creates OpenGL context. GLFW_NO_API informs to not create it.
			3. Disable resizable windows for now
			4. 
		*/
		void			initWindow( void )
		{
			glfwInit();
			glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
			glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );
			_window = glfwCreateWindow( WIDTH, HEIGHT, "My 1st Vulkan Project", nullptr, nullptr );
		}

		void			initVulkan( void ) {

		}

		void			mainLoop( void )
		{
			while ( !glfwWindowShouldClose(_window) )
				glfwPollEvents();
		}

		void			cleanup()
		{
			glfwDestroyWindow( _window );
			glfwTerminate();
		}
};

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