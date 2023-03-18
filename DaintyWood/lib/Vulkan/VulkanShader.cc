#include "Vulkan/VulkanShader.hh"

#include "Vulkan/Macro.hh"
namespace DWE {
    static std::vector<char> readFromFile(const std::string& file)
    {
        std::ifstream in_file(file, std::ios::ate | std::ios::binary);
        if (!in_file.is_open())
            throw std::runtime_error("failed to load shaderCode file: " + file);
        size_t file_size = in_file.tellg();
        std::vector<char> buffer(file_size);
        in_file.seekg(0);
        in_file.read(buffer.data(), file_size);
        in_file.close();

        return buffer;
    }

    VulkanShader::VulkanShader(vk::Device device, ShaderSettings settings)
        : _device(device), 
        _shader_name(settings.shader_name.value()), 
        _compiled_file_path(settings.root_dir + settings.compiled_file_path.value()), 
        _entry_point(settings.entry_point.value())
    {
        static const std::map<std::string, ShaderType> shader_type_map{
            {"VertexShader", ShaderType::Vertex}, 
            {"FragmentShader", ShaderType::Fragment}, 
            {"GeometryShader", ShaderType::Geometry}, 
            {"ComputeShader", ShaderType::Compute}
        };

        static const std::map<std::string, VertexDataFlag> vertex_flag_map{
            {"Position", VertexDataFlag::Position}, 
            {"Color", VertexDataFlag::Color}, 
            {"TexCoord", VertexDataFlag::Texcoord}, 
            {"Normal", VertexDataFlag::Normal}, 
        };

        static const std::map<std::string, UniformFlag> uniform_flag_map{
            {"ModelMatrix", UniformFlag::ModelMatrix}, 
            {"ViewMatrix", UniformFlag::ViewMatrix}, 
            {"ProjectionMatrix", UniformFlag::ProjectionMatrix}, 
        };

        if (shader_type_map.count(settings.shader_type.value()) == 0) {
            throw std::runtime_error("ShaderType: " + settings.shader_type.value() + " does not exist...");
        }

        _shader_type = shader_type_map.at(settings.shader_type.value());

        for (const std::optional<std::string>& vertex_flag : settings.vertex_data_flags) {
            _vertex_data_flags |= vertex_flag_map.at(vertex_flag.value());
        }

        for (const std::optional<std::string>& unifom_flag : settings.uniform_data_flags) {
            _uniform_data_flags |= uniform_flag_map.at(unifom_flag.value());
        }

        for (const std::optional<std::string>& sampler_name : settings.sampler_name_list) {
            _sampler_names.push_back(sampler_name.value());
        }

    }

    VulkanShader::~VulkanShader()
    {

    }

    void VulkanShader::createDescriptorSetLayout()
    {
        if (!_uniform_data_flags) {

        }
    }

    void VulkanShader::createShaderModule()
    {
        std::vector<char> shader_code = readFromFile(_compiled_file_path);

        vk::ShaderModuleCreateInfo shader_module_create_info{};
        shader_module_create_info
            .setCodeSize(shader_code.size())
            .setPCode(reinterpret_cast<const uint32_t*>(shader_code.data()));
        _shader_bin = _device.createShaderModule(shader_module_create_info);
        CHECK_NULL(_shader_bin);
    }

    void VulkanShader::cleanShaderModule()
    {
        _device.destroy(_shader_bin);
    }

    vk::PipelineShaderStageCreateInfo VulkanShader::getStageInfo()
    {
        createShaderModule();

        vk::PipelineShaderStageCreateInfo pipeline_stage_create_info{};
        pipeline_stage_create_info
            .setModule(_shader_bin)
            .setPName(_entry_point.c_str());

        switch(_shader_type) {
            case ShaderType::Vertex: {
                pipeline_stage_create_info.setStage(vk::ShaderStageFlagBits::eVertex);
            } break;
            case ShaderType::Fragment: {
                pipeline_stage_create_info.setStage(vk::ShaderStageFlagBits::eFragment);
            } break;
            case ShaderType::Geometry: {
                pipeline_stage_create_info.setStage(vk::ShaderStageFlagBits::eGeometry);
            } break;
            case ShaderType::Compute: {
                pipeline_stage_create_info.setStage(vk::ShaderStageFlagBits::eCompute);
            } break;
            default: break;
            }

        return pipeline_stage_create_info;
    }

    std::vector<vk::VertexInputBindingDescription> VulkanShader::getVertexBindingDescriptions()
    {
        if (_shader_type != ShaderType::Vertex)
            throw std::runtime_error("calling vertexBindingDescription from non vertex shader...");
        std::vector<vk::VertexInputBindingDescription> binding_descriptions{};

        if (!_vertex_data_flags) return binding_descriptions;

        uint32_t binding = 0;

        if (_vertex_data_flags & VertexDataFlag::Position) {
            vk::VertexInputBindingDescription position_binding{};
            position_binding
                .setBinding(binding)
                .setStride(vertexDataStride(VertexDataFlag::Position))
                .setInputRate(vk::VertexInputRate::eVertex);
            binding_descriptions.push_back(position_binding);
            ++binding;
        }

        if (_vertex_data_flags & VertexDataFlag::Color) {
            vk::VertexInputBindingDescription color_binding{};
            color_binding
                .setBinding(binding)
                .setStride(vertexDataStride(VertexDataFlag::Color))
                .setInputRate(vk::VertexInputRate::eVertex);
            binding_descriptions.push_back(color_binding);
            ++binding;
        }

        if (_vertex_data_flags & VertexDataFlag::Texcoord) {
            vk::VertexInputBindingDescription texcoord_binding{};
            texcoord_binding
                .setBinding(binding)
                .setStride(vertexDataStride(VertexDataFlag::Color))
                .setInputRate(vk::VertexInputRate::eVertex);
            binding_descriptions.push_back(texcoord_binding);
            ++binding;
        }

        if (_vertex_data_flags & VertexDataFlag::Normal) {
            vk::VertexInputBindingDescription normal_binding{};
            normal_binding
                .setBinding(binding)
                .setStride(vertexDataStride(VertexDataFlag::Color))
                .setInputRate(vk::VertexInputRate::eVertex);
            binding_descriptions.push_back(normal_binding);
            ++binding;
        }

        return binding_descriptions;
    }

    std::vector<vk::VertexInputAttributeDescription> VulkanShader::getVertexAttributeDexcriptions()
    {
        if (_shader_type != ShaderType::Vertex)
            throw std::runtime_error("calling vertexAttributeDescription from non vertex shader...");
        std::vector<vk::VertexInputAttributeDescription> attribute_descriptions{};

        if (!_vertex_data_flags) return attribute_descriptions;

        uint32_t binding = 0;
        uint32_t location = 0;

        if (_vertex_data_flags & VertexDataFlag::Position) {
            vk::VertexInputAttributeDescription position_attribute{};
            position_attribute
                .setBinding(binding)
                .setLocation(location)
                .setFormat(vk::Format::eR32G32B32Sfloat)
                .setOffset(0);
            attribute_descriptions.push_back(position_attribute);
            ++binding; ++location;
        }

        if (_vertex_data_flags & VertexDataFlag::Color) {
            vk::VertexInputAttributeDescription color_attribute{};
            color_attribute
                .setBinding(binding)
                .setLocation(location)
                .setFormat(vk::Format::eR32G32B32Sfloat)
                .setOffset(0);
            attribute_descriptions.push_back(color_attribute);
            ++binding; ++location;
        }

        if (_vertex_data_flags & VertexDataFlag::Texcoord) {
            vk::VertexInputAttributeDescription texcoord_attribute{};
            texcoord_attribute
                .setBinding(binding)
                .setLocation(location)
                .setFormat(vk::Format::eR32G32Sfloat)
                .setOffset(0);
            attribute_descriptions.push_back(texcoord_attribute);
            ++binding; ++location;
        }

        if (_vertex_data_flags & VertexDataFlag::Normal) {
            vk::VertexInputAttributeDescription normal_attribute{};
            normal_attribute
                .setBinding(binding)
                .setLocation(location)
                .setFormat(vk::Format::eR32G32B32Sfloat)
                .setOffset(0);
            attribute_descriptions.push_back(normal_attribute);
            ++binding; ++location;
        }

        return attribute_descriptions;
    }

    vk::DescriptorSetLayout VulkanShader::getDescriptorSetLayout()
    {
        return _descriptor_set_layout;
    }

}
#include "Vulkan/UnMacro.hh"
