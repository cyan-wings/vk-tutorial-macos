SHELL := /bin/zsh

VK = ../../VulkanSDK/macOS

export VULKAN_SDK=$(VK)
export PATH=$PATH:$(VK)/bin
export DYLD_LIBRARY_PATH=$(VK)/lib
export VK_ICD_FILENAMES=$(VK)/share/vulkan/icd.d/MoltenVK_icd.json
export VK_LAYER_PATH=$(VK)/share/vulkan/explicit_layer.d

CXX = g++
#FSANITIZE = -fsanitize=address
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -O2 -g$(if $(FSANITIZE), $(FSANITIZE))
IFLAGS = -I/opt/homebrew/include -I$(VK)/include -Iincludes
LFLAGS = -lvulkan -L$(VK)/lib -lglfw -L/opt/homebrew/lib

SRCDIR = srcs
OBJDIR = objs

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

NAME = draw_triangle

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $(NAME) $(LFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(IFLAGS) -c $< -o $@

run:
	./$(NAME)

debug:
	. ./debug.sh

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all run clean fclean re
