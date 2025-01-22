VK = ../../VulkanSDK/macOS

export VULKAN_SDK=$(VK)
export PATH=$PATH:$(VK)/bin
export DYLD_LIBRARY_PATH=$(VK)/lib
export VK_ICD_FILENAMES=$(VK)/share/vulkan/icd.d/MoltenVK_icd.json
export VK_LAYER_PATH=$(VK)/share/vulkan/explicit_layer.d

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -O2 -g
INCLUDES = -I/opt/homebrew/include -I$(VK)/include
LIBS = -lvulkan -L$(VK)/lib -lglfw -L/opt/homebrew/lib

SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)
NAME = draw_triangle

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
