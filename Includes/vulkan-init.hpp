#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_vulkan.h>

class VulkanInit {
private:
vk::raii::Context context;
vk::raii::Instance instance{nullptr};
public:
    void vulkan_init();
    void create();
    void clean();
};