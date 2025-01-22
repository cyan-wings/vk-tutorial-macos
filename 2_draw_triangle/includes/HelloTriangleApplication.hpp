#ifndef __HELLOTRIANGLEAPPLICATION_HPP__
# define __HELLOTRIANGLEAPPLICATION_HPP__

# define GLFW_INCLUDE_VULKAN
# include <GLFW/glfw3.h>

# include <iostream>
# include <stdexcept>
# include <cstdlib>
# include <cstring>

#ifdef NDEBUG
	bool const	enableValidationLayers = false;
#else
	bool const	enableValidationLayers = true;
#endif

class HelloTriangleApplication
{
	public:
		void									run( void );

	private:
		uint32_t const							WIDTH = 800;
		uint32_t const							HEIGHT = 600;
		GLFWwindow								*_window;
		VkInstance								_instance;
		VkDebugUtilsMessengerEXT				_debugMessenger;
		std::vector<const char*> const			validationLayers = { "VK_LAYER_KHRONOS_validation" };

		void									initWindow( void );
		void									initVulkan( void );
		void									createInstance( void );
		bool									checkValidationLayerSupport( void );
		void									confAppInfo( VkApplicationInfo &appInfo );
		void									confCreateInfo
		(
			VkInstanceCreateInfo &createInfo,
			VkApplicationInfo &appInfo,
			std::vector<char const *> &requiredExtensions,
			VkDebugUtilsMessengerCreateInfoEXT	&debugCreateInfo
		);
		std::vector<char const *>				getRequiredExtensions( void );
		void			checkExtensions( std::vector<const char*> &requiredExtensions );
		void			setupDebugMessenger( void );
		void			populateDebugMessengerCreateInfo( VkDebugUtilsMessengerCreateInfoEXT &createInfo );
		static VKAPI_ATTR VkBool32 VKAPI_CALL	debugCallback
		(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData
		);
		VkResult								CreateDebugUtilsMessengerEXT
		(
			VkInstance instance,
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator,
			VkDebugUtilsMessengerEXT* pDebugMessenger
		);
		void									DestroyDebugUtilsMessengerEXT
		(
			VkInstance instance,
			VkDebugUtilsMessengerEXT debugMessenger,
			const VkAllocationCallbacks* pAllocator
		);
		void									mainLoop( void );
		void									cleanup( void );
};

std::ostream									&operator<<(std::ostream &os, std::vector<char const *> const &vec);

#endif
