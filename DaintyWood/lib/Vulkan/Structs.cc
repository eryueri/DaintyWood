#include "Vulkan/Structs.hh"

namespace DWE {
    bool QueueFamilyIndices::isComplete()
    {
        return graphics_family.has_value() && present_family.has_value();
    }

    bool QueueFamilyIndices::isDiscrete()
    {
        return graphics_family.value() != present_family.value();
    }

    std::vector<uint32_t> QueueFamilyIndices::operator()()
    {
        return std::vector<uint32_t>{graphics_family.value(), present_family.value()};
    }
}
