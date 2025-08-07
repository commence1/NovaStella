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

    instance = vk::raii::Instance(context, createInfo);
}
void VulkanInit::clean() {

}