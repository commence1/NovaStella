#include <vulkan-init.hpp>

void VulkanInit::vulkan_init() {
    create();
}
void VulkanInit::create() {
    vk::ApplicationInfo applicationInfo;

    applicationInfo.pApplicationName = "Nova Stella";
    applicationInfo.pEngineName = "No Engine";
    applicationInfo.apiVersion = 1.4;

    vk::InstanceCreateInfo createInfo({}, &applicationInfo);

    uint32_t extensionCount = 0;
    const char * const *tmp = SDL_Vulkan_GetInstanceExtensions(&extensionCount);
    if (!tmp) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to get Vulkan instance extensions: %s", SDL_GetError());
        return;
    }
    std::vector<const char *>required_extensions(tmp, tmp + extensionCount);
    createInfo.setPEnabledExtensionNames(required_extensions);
    createInfo.enabledExtensionCount = static_cast<uint32_t>(required_extensions.size());
    createInfo.ppEnabledExtensionNames = required_extensions.data();

    instance = vk::raii::Instance(context, createInfo);
}
void VulkanInit::interate() {
    
}
void VulkanInit::clean() {

}