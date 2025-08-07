#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

class VulkanInit {
private:
public:
    void vulkan_init();
    void createInstance();
    void clean();
};