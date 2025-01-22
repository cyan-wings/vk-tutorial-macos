#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

std::ostream& operator<<(std::ostream& os, const std::vector<char const*>& vec) {
	for (size_t i = 0; i < vec.size(); ++i) {
		os << vec[i];
		if (i != vec.size() - 1) {
			os << " ";  // Separate strings by spaces (or any separator)
		}
	}
	return os;
}

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
		VkInstance		_instance;

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

		void			confAppInfo( VkApplicationInfo &appInfo )
		{
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "Hello Triangle";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "No Engine";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_0;
		}

		void			confCreateInfo( VkInstanceCreateInfo &createInfo, VkApplicationInfo &appInfo, std::vector<const char*> &requiredExtensions )
		{
			uint32_t		glfwExtensionCount = 0;
			char const		**glfwExtensions;

			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;
			createInfo.enabledLayerCount = 0;

			glfwExtensions = glfwGetRequiredInstanceExtensions( &glfwExtensionCount );
			for(uint32_t i = 0; i < glfwExtensionCount; i++)
				requiredExtensions.emplace_back(glfwExtensions[i]);
			requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

			std::cout << requiredExtensions<<std::endl;

			createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
			createInfo.enabledExtensionCount = (uint32_t) requiredExtensions.size();
			createInfo.ppEnabledExtensionNames = requiredExtensions.data();
		}

		void			checkExtensions( std::vector<const char*> &requiredExtensions )
		{
			uint32_t	extensionCount = 0;

			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
			std::vector<VkExtensionProperties> extensions(extensionCount);
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
			std::cout << "available extensions:\n";
			for (const auto& ext : extensions)
				std::cout << '\t' << ext.extensionName << '\n';
			for (auto const& reqext : requiredExtensions)
			{
				auto it = std::find_if( extensions.begin(), extensions.end(),
					[&reqext] ( VkExtensionProperties const &ext ) -> bool { return std::string(ext.extensionName) == reqext; } );
				if (it != extensions.end())
					std::cout << reqext << " found!" << std::endl;
				else
					std::cout << reqext << " found!" << std::endl;
			}
		}

		/*
			Data is optional. But useful for optimization of specifics.
		*/
		void			createInstance( void )
		{
			VkApplicationInfo			appInfo{};
			VkInstanceCreateInfo		createInfo{};
			std::vector<char const *>	requiredExtensions;
			
			confAppInfo( appInfo );
			confCreateInfo( createInfo, appInfo, requiredExtensions );
			VkResult	result = vkCreateInstance(&createInfo, nullptr, &_instance);
			( void )result;
			if (vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS)
				throw std::runtime_error("failed to create instance!");
			checkExtensions( requiredExtensions );
			
		}

		void			initVulkan( void )
		{
			createInstance();
		}

		void			mainLoop( void )
		{
			while ( !glfwWindowShouldClose(_window) )
				glfwPollEvents();
		}

		void			cleanup()
		{
			vkDestroyInstance(_instance, nullptr);
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