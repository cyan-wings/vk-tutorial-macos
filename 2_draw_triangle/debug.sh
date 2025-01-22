
export VULKAN_SDK=../../VulkanSDK/macOS
export PATH=$PATH:../../VulkanSDK/macOS/bin
export DYLD_LIBRARY_PATH=../../VulkanSDK/macOS/lib
export VK_ICD_FILENAMES=../../VulkanSDK/macOS/share/vulkan/icd.d/MoltenVK_icd.json
export VK_LAYER_PATH=../../VulkanSDK/macOS/share/vulkan/explicit_layer.d

/usr/bin/lldb ./draw_triangle