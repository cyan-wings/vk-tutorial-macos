export VULKAN_SDK=../VulkanSDK/macOS
export PATH=$PATH:../VulkanSDK/macOS/bin
export DYLD_LIBRARY_PATH=../VulkanSDK/macOS/lib
export VK_ICD_FILENAMES=../VulkanSDK/macOS/share/vulkan/icd.d/MoltenVK_icd.json
export VK_LAYER_PATH=../VulkanSDK/macOS/share/vulkan/explicit_layer.d

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -O2 -g
INCLUDES = -I/opt/homebrew/include -I../VulkanSDK/macOS/include
LIBS = -lvulkan -L../VulkanSDK/macOS/lib -lglfw -L/opt/homebrew/lib

SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)
NAME = vk-macos

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $(NAME) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $<

run:
	./$(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all run clean fclean re
