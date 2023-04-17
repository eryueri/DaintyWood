#include "Vulkan/UniformData.hh"

#include "Vulkan/ShaderSettings.hh"

namespace DWE {
    std::vector<char> UniformData::getUniformData(uint8_t uniform_flags) const
    {
        auto uniform_types = uniformFlagsGrab(uniform_flags);
        std::vector<char> buffer;
        for (auto type : uniform_types) {
            char* data = nullptr;
            uint32_t size = 0;
            switch (type) {
            case UniformFlag::ModelMatrix: {
                data = (char*)&ModelMatrix;
                size = sizeof(ModelMatrix);
                std::vector<char> tmp(data, data+size);
                buffer.insert(buffer.end(), tmp.begin(), tmp.end());
            } break;
            case UniformFlag::ViewMatrix: {
                data = (char*)&ViewMatrix;
                size = sizeof(ViewMatrix);
                std::vector<char> tmp(data, data+size);
                buffer.insert(buffer.end(), tmp.begin(), tmp.end());
            } break;
            case UniformFlag::ProjectionMatrix: {
                data = (char*)&ProjectionMatrix;
                size = sizeof(ProjectionMatrix);
                std::vector<char> tmp(data, data+size);
                buffer.insert(buffer.end(), tmp.begin(), tmp.end());
            } break;
            case UniformFlag::ViewProjectionMatrix: {
                data = (char*)&ViewProjectionMatrix;
                size = sizeof(ViewProjectionMatrix);
                std::vector<char> tmp(data, data+size);
                buffer.insert(buffer.end(), tmp.begin(), tmp.end());
            } break;
            case UniformFlag::ModelViewProjectionMatrix: {
                data = (char*)&ModelViewProjectionMatrix;
                size = sizeof(ModelViewProjectionMatrix);
                std::vector<char> tmp(data, data+size);
                buffer.insert(buffer.end(), tmp.begin(), tmp.end());
            } break;
            default: break;
            }
        }

        return buffer;
    }
}
