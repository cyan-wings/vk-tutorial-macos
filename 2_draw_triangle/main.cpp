#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>

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
		uint32_t const					WIDTH = 800;
		uint32_t const					HEIGHT = 600;
		GLFWwindow						*_window;
		VkInstance						_instance;
		VkDebugUtilsMessengerEXT		debugMessenger;
		std::vector<const char*> const	validationLayers = { "VK_LAYER_KHRONOS_validation" };
		#ifdef NDEBUG
			bool const 					enableValidationLayers = false;
		#else
			bool const 					enableValidationLayers = true;
		#endif

		/*
			Available messageSeverity macros:
			1. VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
			2. VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
			3. VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
			4. VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT

			Available messageType macros:
			1. VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
			2. VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
			3. VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
		*/
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData)
		{
			//if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			( void )messageSeverity;
			( void )messageType;
			( void )pUserData;
			std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
			return ( VK_FALSE );
		}

		VkResult CreateDebugUtilsMessengerEXT
		(
			VkInstance instance,
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator,
			VkDebugUtilsMessengerEXT* pDebugMessenger
		)
		{
			auto	func = ( PFN_vkCreateDebugUtilsMessengerEXT ) vkGetInstanceProcAddr( instance, "vkCreateDebugUtilsMessengerEXT" );
			
			if ( func != nullptr )
				return ( func(instance, pCreateInfo, pAllocator, pDebugMessenger) );
			else
				return ( VK_ERROR_EXTENSION_NOT_PRESENT );
		}

		void DestroyDebugUtilsMessengerEXT
		(
			VkInstance instance,
			VkDebugUtilsMessengerEXT debugMessenger,
			const VkAllocationCallbacks* pAllocator
		)
		{
			auto	func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
			
			if (func != nullptr) {
				func(instance, debugMessenger, pAllocator);
			}
		}

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

		std::vector<char const *>	getRequiredExtensions( void )
		{
			uint32_t					glfwExtensionCount = 0;
			char const					**glfwExtensions;
			std::vector<char const *>	requiredExtensions;

			glfwExtensions = glfwGetRequiredInstanceExtensions( &glfwExtensionCount );
			for( uint32_t i = 0; i < glfwExtensionCount; i++ )
				requiredExtensions.emplace_back( glfwExtensions[i] );
			requiredExtensions.emplace_back( VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME );
			if (enableValidationLayers)
				requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			std::cout << requiredExtensions << std::endl;
			return ( requiredExtensions );
		}

		void			confCreateInfo
		(
			VkInstanceCreateInfo &createInfo,
			VkApplicationInfo &appInfo,
			std::vector<char const *> &requiredExtensions,
			VkDebugUtilsMessengerCreateInfoEXT	&debugCreateInfo
		)
		{
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;
			if ( enableValidationLayers )
			{
				createInfo.enabledLayerCount = static_cast<uint32_t>( validationLayers.size() );
				createInfo.ppEnabledLayerNames = validationLayers.data();
				populateDebugMessengerCreateInfo(debugCreateInfo);
				createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
			}
			else
			{
				createInfo.enabledLayerCount = 0;
				createInfo.pNext = nullptr;
			}
			requiredExtensions = getRequiredExtensions();
			checkExtensions( requiredExtensions );
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
			for (auto const &ext : extensions)
				std::cout << '\t' << ext.extensionName << '\n';
			for (auto const &reqext : requiredExtensions)
			{
				auto it = std::find_if( extensions.begin(), extensions.end(),
					[&reqext] ( VkExtensionProperties const &ext ) -> bool { return std::string(ext.extensionName) == reqext; } );
				if ( it != extensions.end() )
					std::cout << reqext << " found!" << std::endl;
				else
					std::cout << reqext << " found!" << std::endl;
			}
		}

		bool			checkValidationLayerSupport( void )
		{
			uint32_t	layerCount;

			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
			std::vector<VkLayerProperties> availableLayers(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
			for (const char* layerName : validationLayers)
			{
				bool	layerFound = false;

				for ( auto const &layerProperties : availableLayers )
				{
					std::cout << "Available layer: " << layerProperties.layerName << std::endl;
					if (strcmp(layerName, layerProperties.layerName) == 0)
					{
						std::cout << layerName << " found!" << std::endl;
						layerFound = true;
						break ;
					}
				}
				if ( !layerFound )
					return ( false );
			}
			return ( true );
		}

		/*
			Data is optional. But useful for optimization of specifics.
		*/
		void			createInstance( void )
		{
			VkApplicationInfo					appInfo{};
			VkInstanceCreateInfo				createInfo{};
			std::vector<char const *>			requiredExtensions;
			VkDebugUtilsMessengerCreateInfoEXT	debugCreateInfo{};

			if ( enableValidationLayers && !checkValidationLayerSupport() )
				throw ( std::runtime_error("validation layers requested, but not available!") );
			confAppInfo( appInfo );
			confCreateInfo( createInfo, appInfo, requiredExtensions, debugCreateInfo );
			if (vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS)
				throw std::runtime_error("failed to create instance!");
		}

		void			initVulkan( void )
		{
			createInstance();
			setupDebugMessenger();
		}

		void populateDebugMessengerCreateInfo( VkDebugUtilsMessengerCreateInfoEXT &createInfo )
		{
			createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			createInfo.pfnUserCallback = debugCallback;
		}

		/*
			pfnUserCallback is call back to the callback function
			pUserData, optional param to pass in to the callback function
		*/
		void setupDebugMessenger( void )
		{
			if (!enableValidationLayers) return;
			VkDebugUtilsMessengerCreateInfoEXT	createInfo{};
			populateDebugMessengerCreateInfo( createInfo );
			if (CreateDebugUtilsMessengerEXT(_instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
				throw std::runtime_error("failed to set up debug messenger!");
		}

		void			mainLoop( void )
		{
			while ( !glfwWindowShouldClose(_window) )
				glfwPollEvents();
		}

		void			cleanup( void )
		{
			if (enableValidationLayers)
				DestroyDebugUtilsMessengerEXT(_instance, debugMessenger, nullptr);
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