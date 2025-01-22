# Vulkan Playtest on macOS

## Setup

1. Install glm and glfw.
```
brew install glm
brew install glfw
```
2. Download and install SDK from [LunarG]https://vulkan.lunarg.com/
3. Replace the Makefile variable `VK` to the directory of the installed SDK

## Compile and running

1. Compile using `make`.
```
make
```
2. Run
```
make run
```

## Cleanup

```
make fclean
```
