#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>

std::ostream	&operator<<( std::ostream& out, const glm::mat4& mat )
{
	for ( int i = 0; i < 4; ++i )
	{
		out << "(";
		for ( int j = 0; j < 4; ++j )
		{
			out << mat[i][j];
			if (j < 3) out << ", ";
		}
		out << ")";
		if ( i < 3 ) out << "\n";
	}
	return ( out );
}

std::ostream	&operator<<( std::ostream& out, const glm::vec4& vec )
{
	out << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
	return ( out );
}

int main( void )
{
	glfwInit();

	glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
	GLFWwindow* window = glfwCreateWindow( 800, 600, "Vulkan window", nullptr, nullptr );

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties( nullptr, &extensionCount, nullptr );

	std::cout << extensionCount << " extensions supported\n";

	glm::mat4 matrix;
	glm::vec4 vec;
	auto test = matrix * vec;
	std::cout << "Matrix * vec: " << test << std::endl;

	while ( !glfwWindowShouldClose( window ) )
		glfwPollEvents();

	glfwDestroyWindow( window );

	glfwTerminate();

	return ( 0 );
}
