#include "Vulkan/VulkanRenderEntity.hh"

#include "Vulkan/VulkanInstance.hh"

#include "Vulkan/Macro.hh"
namespace DWE {
    VulkanRenderEntity::VulkanRenderEntity(VulkanInstance* instance, EntitySettings settings)
        : _vulkan_instance(instance), 
          _utils(instance->getVulkanUtils())
    {
        static std::map<std::string, EntityType> entity_type_map{
            {"RenderEntity", EntityType::RenderEntity}, 
            {"ComputeEntity", EntityType::ComputeEntity}
        };

        static std::map<std::string, MeshCullMode> cull_mode_map{
            {"None", MeshCullMode::None}, 
            {"FrontFace", MeshCullMode::FrontFace}, 
            {"BackFace", MeshCullMode::BackFace}
        };
    
        _type = entity_type_map.at(settings.entity_type.value());
        _cull_mode = cull_mode_map.at(settings.cull_mode.value());
    }

    VulkanRenderEntity::~VulkanRenderEntity()
    {

    }

    void VulkanRenderEntity::addTexture(VulkanTexture *texture)
    {
        _vulkan_textures.push_back(texture);
    }

    void VulkanRenderEntity::addMesh(VulkanMesh *mesh)
    {
        _vulkan_meshes.push_back(mesh);
    }

    void VulkanRenderEntity::addShader(VulkanShader *shader)
    {
        switch (shader->getShaderType()) {
            case ShaderType::Vertex: _vertex_shader = shader; break;
            case ShaderType::Fragment: _frag_shader = shader; break;
            case ShaderType::Geometry:
            case ShaderType::Compute:
              break;
            }
        _vulkan_shaders.push_back(shader);
    }

    void VulkanRenderEntity::initialize()
    {
        for (VulkanShader* shader : _vulkan_shaders) {
            if (shader->getShaderType() == ShaderType::Vertex) {
                _vertex_data_flags = shader->getVertexDataFlags();
                _vertex_shader = shader;
            }
            if (shader->getShaderType() == ShaderType::Fragment) {
                _frag_shader = shader;
            }
            _uniform_data_flags |= shader->getUniformDataFlags();
        }

        createSamplers();
        createUniformBuffers();
        createPipelineLayout();
        createPipeline();
        createDescriptorPool();
        createDescriptorSets();
        updateDescriptorSets();

        uint32_t swapchain_size = _vulkan_instance->getSwapchainSize();
        UniformData data{};
        for (uint32_t i = 0; i < swapchain_size; ++i) {
            updateUniformData(i, data);
        }
    }

    void VulkanRenderEntity::updateUniformData(const uint32_t& image_index, const UniformData& data)
    {
        if (_uniform_buffers.empty())
            return;

        uint32_t uniform_offset = 0;
        for (const auto& shader : _vulkan_shaders) {
            uint32_t uniform_size = shader->getUniformBufferSize();

            uint8_t uniform_flags = shader->getUniformDataFlags();

            auto data_buffer = data.getUniformData(uniform_flags);

            memcpy((char*)_memory_maps[image_index] + uniform_offset, data_buffer.data(), data_buffer.size());

            uniform_offset += uniform_size;
        }
    }

    void VulkanRenderEntity::writeDrawingCommands(uint32_t image_index)
    {
        vk::CommandBuffer command_buffer = _vulkan_instance->getRenderCommandBuffer(image_index);
        command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, _graphics_pipeline);
        command_buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, _pipeline_layout, 0, getDescriptorSets(image_index), nullptr);

        if (!_vulkan_meshes.empty()) {
            for (VulkanMesh* mesh : _vulkan_meshes) {
                mesh->writeDrawingCommands(_vertex_data_flags, image_index);
            }
        }
    }

    void VulkanRenderEntity::createPipelineLayout()
    {
        std::vector<vk::DescriptorSetLayout> descriptor_set_layouts{};
        if (!_vertex_shader || !_frag_shader) {
            throw std::runtime_error("shader needed not all initialized...");
        }
        vk::DescriptorSetLayout vertex_descriptor_set_layout = _vertex_shader->getDescriptorSetLayout();
        // CHECK_NULL(vertex_descriptor_set_layout);
        if (vertex_descriptor_set_layout)
            descriptor_set_layouts.push_back(vertex_descriptor_set_layout);
        vk::DescriptorSetLayout fragment_descriptor_set_layout = _frag_shader->getDescriptorSetLayout();
        // CHECK_NULL(fragment_descriptor_set_layout);
        if (fragment_descriptor_set_layout)
            descriptor_set_layouts.push_back(fragment_descriptor_set_layout);

        vk::PipelineLayoutCreateInfo pipeline_layout_create_info{};
        pipeline_layout_create_info
            .setSetLayoutCount(descriptor_set_layouts.size())
            .setPSetLayouts(descriptor_set_layouts.data())
            .setPushConstantRangeCount(0)
            .setPPushConstantRanges(nullptr);

        _pipeline_layout = _vulkan_instance->getLogicalDevice().createPipelineLayout(pipeline_layout_create_info);
        CHECK_NULL(_pipeline_layout);
    }

    void VulkanRenderEntity::createPipeline()
    {
        std::vector<vk::PipelineShaderStageCreateInfo> shader_stage_infos{};
        shader_stage_infos.push_back(_vertex_shader->getStageInfo());
        shader_stage_infos.push_back(_frag_shader->getStageInfo());

        vk::PipelineVertexInputStateCreateInfo vertex_input_create_info{};
        auto vertex_binding_descriptions = _vertex_shader->getVertexBindingDescriptions();
        auto vertex_attribute_descriptions = _vertex_shader->getVertexAttributeDexcriptions();
        vertex_input_create_info
            .setVertexBindingDescriptions(vertex_binding_descriptions)
            .setVertexAttributeDescriptions(vertex_attribute_descriptions);

        vk::PipelineInputAssemblyStateCreateInfo input_assembly_create_info{};
        input_assembly_create_info
            .setTopology(vk::PrimitiveTopology::eTriangleList)
            .setPrimitiveRestartEnable(false);

        vk::Extent2D swapchain_extent = _vulkan_instance->getSwapchainExtent();

        vk::Viewport viewport{
            0.0f, 0.0f, // x, y
            (float)swapchain_extent.width, (float)swapchain_extent.height, // width, height
            0.0f, 1.0f // min/max depth
        };

        vk::Rect2D scissor{
            vk::Offset2D{ 0, 0 }, // x, y
            swapchain_extent
        };

        std::vector<vk::DynamicState> dynamic_states = {
            vk::DynamicState::eViewport, 
            vk::DynamicState::eScissor, 
        };

        vk::PipelineDynamicStateCreateInfo dynamic_state_create_info{};
        dynamic_state_create_info.setDynamicStates(dynamic_states);

        vk::PipelineViewportStateCreateInfo viewport_state_create_info{};
        viewport_state_create_info
            .setViewports(viewport)
            .setScissors(scissor);

        vk::PipelineRasterizationStateCreateInfo rasterizer_create_info{};
        rasterizer_create_info
            .setDepthClampEnable(false)
            .setRasterizerDiscardEnable(false)
            .setPolygonMode(vk::PolygonMode::eFill)
            .setLineWidth(1.0f)
            .setFrontFace(vk::FrontFace::eClockwise)
            .setDepthBiasEnable(false);
        switch(_cull_mode) {
            case MeshCullMode::None: {
                rasterizer_create_info.setCullMode(vk::CullModeFlagBits::eNone);
            } break;
            case MeshCullMode::FrontFace: {
                rasterizer_create_info.setCullMode(vk::CullModeFlagBits::eFront);
            } break;
            case MeshCullMode::BackFace: {
                rasterizer_create_info.setCullMode(vk::CullModeFlagBits::eBack);
            } break;
            default: break;
        }

        vk::PipelineMultisampleStateCreateInfo multisampling_create_info{};
        multisampling_create_info
            .setSampleShadingEnable(false)
            .setRasterizationSamples(vk::SampleCountFlagBits::e1)
            .setMinSampleShading(1.0f)
            .setPSampleMask(nullptr)
            .setAlphaToCoverageEnable(false)
            .setAlphaToOneEnable(false);

        vk::PipelineDepthStencilStateCreateInfo depth_stencil_create_info{};
        depth_stencil_create_info
            .setDepthTestEnable(true)
            .setDepthWriteEnable(true)
            .setDepthCompareOp(vk::CompareOp::eLess)
            .setDepthBoundsTestEnable(false)
            .setMinDepthBounds(0.0f)
            .setMaxDepthBounds(1.0f)
            .setStencilTestEnable(false)
            .setFront({})
            .setBack({});

        vk::PipelineColorBlendAttachmentState color_blend_attachment{};
        color_blend_attachment
            .setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA)
            .setBlendEnable(false)
            .setSrcColorBlendFactor(vk::BlendFactor::eOne)
            .setDstColorBlendFactor(vk::BlendFactor::eZero)
            .setColorBlendOp(vk::BlendOp::eAdd)
            .setSrcAlphaBlendFactor(vk::BlendFactor::eOne)
            .setDstAlphaBlendFactor(vk::BlendFactor::eZero)
            .setAlphaBlendOp(vk::BlendOp::eAdd);

        vk::PipelineColorBlendStateCreateInfo color_blend_create_info{};
        color_blend_create_info
            .setLogicOpEnable(false)
            .setLogicOp(vk::LogicOp::eCopy)
            .setAttachments(color_blend_attachment)
            .setBlendConstants({0.0f, 0.0f, 0.0f, 0.0f});

        vk::GraphicsPipelineCreateInfo pipeline_create_info{};
        pipeline_create_info
            .setStages(shader_stage_infos)
            .setPVertexInputState(&vertex_input_create_info)
            .setPInputAssemblyState(&input_assembly_create_info)
            .setPDynamicState(&dynamic_state_create_info)
            .setPViewportState(&viewport_state_create_info)
            .setPRasterizationState(&rasterizer_create_info)
            .setPMultisampleState(&multisampling_create_info)
            .setPDepthStencilState(&depth_stencil_create_info)
            .setPColorBlendState(&color_blend_create_info)
            .setLayout(_pipeline_layout) // might need refractor for furture use of multi render pass
            .setRenderPass(_vulkan_instance->getRenderPass())
            .setSubpass(0);

        vk::Result result;
        std::tie(result, _graphics_pipeline) = _vulkan_instance->getLogicalDevice().createGraphicsPipeline(nullptr, pipeline_create_info);

        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("failed to create graphics pipeline...");
        }

        _vertex_shader->cleanShaderModule();
        _frag_shader->cleanShaderModule();
    }

    void VulkanRenderEntity::createUniformBuffers()
    {
        vk::DeviceSize buffer_size = 0;
        uint32_t swapchain_size = _vulkan_instance->getSwapchainSize();

        for (const auto& shader : _vulkan_shaders) {
            buffer_size += shader->getUniformBufferSize();
        }

        if (buffer_size == 0)
            return;

        _uniform_buffers.resize(swapchain_size);
        _uniform_memories.resize(swapchain_size);
        _memory_maps.resize(swapchain_size);

        for (uint32_t i = 0; i < swapchain_size; ++i) {
            _utils->allocateBufferMemory(
                    buffer_size, 
                    vk::BufferUsageFlagBits::eUniformBuffer, 
                    vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, 
                    _uniform_buffers[i], 
                    _uniform_memories[i]);

            vk::Result result = _vulkan_instance->getLogicalDevice().mapMemory(_uniform_memories[i], 0, buffer_size, vk::MemoryMapFlags(0), &_memory_maps[i]);
            if (result != vk::Result::eSuccess) {
                throw std::runtime_error("failed to map uniform buffer memory");
            }
        }
    }

    void VulkanRenderEntity::createSamplers() 
    {
        static const std::map<AddressMode, vk::SamplerAddressMode> address_mode = {
            {AddressMode::Repeat, vk::SamplerAddressMode::eRepeat}, 
            {AddressMode::MirroredRepeat, vk::SamplerAddressMode::eMirroredRepeat}, 
            {AddressMode::ClampToEdge, vk::SamplerAddressMode::eClampToEdge}, 
            {AddressMode::ClampToBorder, vk::SamplerAddressMode::eClampToBorder}, 
            {AddressMode::MirrorClampToEdge, vk::SamplerAddressMode::eMirrorClampToEdge}, 
        };

        static const std::map<BorderColor, vk::BorderColor> border_color = {
            {BorderColor::IntOpaqueBlack, vk::BorderColor::eIntOpaqueBlack}, 
            {BorderColor::FloatOpaqueBlack, vk::BorderColor::eFloatOpaqueBlack}, 
            {BorderColor::IntTransparentBlack, vk::BorderColor::eIntTransparentBlack}, 
            {BorderColor::FloatTransparentBlack, vk::BorderColor::eFloatTransparentBlack}, 
            {BorderColor::IntOpaqueWhite, vk::BorderColor::eIntOpaqueWhite}, 
            {BorderColor::FloatOpaqueWhite, vk::BorderColor::eFloatOpaqueWhite}, 
        };

        vk::PhysicalDeviceProperties gpu_properties = _vulkan_instance->getGPU().getProperties();
        vk::Device device = _vulkan_instance->getLogicalDevice();

        for (const auto& texture : _vulkan_textures) {
            SamplerInfo sampler_info = texture->getSamplerInfo();

            vk::SamplerCreateInfo create_info{};
            create_info
                .setMagFilter(vk::Filter::eLinear)
                .setMinFilter(vk::Filter::eLinear)
                .setAddressModeU(address_mode.at(sampler_info.address_mode))
                .setAddressModeV(address_mode.at(sampler_info.address_mode))
                .setAddressModeW(address_mode.at(sampler_info.address_mode))
                .setAnisotropyEnable(false)
                .setMaxAnisotropy(gpu_properties.limits.maxSamplerAnisotropy)
                .setBorderColor(border_color.at(sampler_info.border_color))
                .setUnnormalizedCoordinates(false)
                .setCompareEnable(false)
                .setCompareOp(vk::CompareOp::eAlways)
                .setMipmapMode(vk::SamplerMipmapMode::eLinear)
                .setMipLodBias(0.0f)
                .setMinLod(0.0f)
                .setMaxLod(0.0f);
            
            _samplers[sampler_info.sampler_name] = device.createSampler(create_info);
            CHECK_NULL(_samplers.at(sampler_info.sampler_name));

            _image_views[sampler_info.sampler_name] = texture->getImageView();
        }
    }

    void VulkanRenderEntity::createDescriptorPool()
    {

        std::vector<vk::DescriptorPoolSize> pool_sizes{};

        uint32_t uniform_buffer_count = 0;
        uint32_t sampler_count = 0;
        uint32_t swapchain_size = _vulkan_instance->getSwapchainSize();

        for (const auto& shader : _vulkan_shaders) {
            uniform_buffer_count += shader->getUniformBufferCount();
            sampler_count += shader->getSamplerCount();
        }

        vk::DescriptorPoolSize uniform_pool_size{};
        if (uniform_buffer_count > 0) {
            uniform_pool_size
                .setType(vk::DescriptorType::eUniformBuffer)
                .setDescriptorCount(swapchain_size * uniform_buffer_count * MAX_FRAMES_IN_FLIGHT/* shader uniform buffer count multiply MAX_FRAME_IN_FLIGHT*/);
            pool_sizes.push_back(uniform_pool_size);
        }

        vk::DescriptorPoolSize sampler_pool_size{};
        if (sampler_count > 0) {
            sampler_pool_size
                .setType(vk::DescriptorType::eCombinedImageSampler)
                .setDescriptorCount(swapchain_size * sampler_count * MAX_FRAMES_IN_FLIGHT);
            pool_sizes.push_back(sampler_pool_size);
        }

        vk::DescriptorPoolCreateInfo create_info{};

        create_info
            .setPoolSizes(pool_sizes)
            .setMaxSets(swapchain_size * MAX_FRAMES_IN_FLIGHT * _vulkan_shaders.size());

        _descriptor_pool = _vulkan_instance->getLogicalDevice().createDescriptorPool(create_info);
        CHECK_NULL(_descriptor_pool);
    }

    void VulkanRenderEntity::createDescriptorSets()
    {
        uint32_t swapchain_size = _vulkan_instance->getSwapchainSize();

        auto allocateDescriptorSets = [this, swapchain_size](VulkanShader* shader)
        {
            if (!shader->getDescriptorSetLayout())
                return;
            std::vector<vk::DescriptorSetLayout> layouts(swapchain_size, shader->getDescriptorSetLayout());
            vk::DescriptorSetAllocateInfo allocate_info{};
            allocate_info
                .setDescriptorPool(_descriptor_pool)
                .setSetLayouts(layouts);

            _descriptor_sets[shader] = _vulkan_instance->getLogicalDevice().allocateDescriptorSets(allocate_info);
        };

        for (const auto& shader : _vulkan_shaders) {
            allocateDescriptorSets(shader);
        }
    }

    void VulkanRenderEntity::updateDescriptorSets()
    {
        uint32_t swapchain_size = _vulkan_instance->getSwapchainSize();
        uint32_t uniform_buffer_offset = 0;
        vk::Device device = _vulkan_instance->getLogicalDevice();

        auto updateDescriptorSets = [this, &swapchain_size, &uniform_buffer_offset, device](VulkanShader* shader) {
            uint32_t uniform_buffer_size = shader->getUniformBufferSize();

            if (uniform_buffer_size == 0) 
                return;

            for (uint32_t i = 0; i < swapchain_size; ++i) {
                vk::DescriptorBufferInfo buffer_info{};
                if (!_uniform_buffers.empty()) {
                    buffer_info
                        .setBuffer(_uniform_buffers[i])
                        .setOffset(uniform_buffer_offset)
                        .setRange(uniform_buffer_size);
                }

                std::vector<vk::DescriptorImageInfo> image_infos;
                for (const auto& sampler_name : shader->getSamplerNameList()) {
                    vk::DescriptorImageInfo image_info{};
                    image_info
                        .setSampler(_samplers.at(sampler_name))
                        .setImageView(_image_views.at(sampler_name))
                        .setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal);
                    image_infos.push_back(image_info);
                }

                std::vector<vk::WriteDescriptorSet> descriptor_writes;
                uint32_t binding = 0;

                if (!image_infos.empty()) {
                    for (const auto& image_info : image_infos) {
                        vk::WriteDescriptorSet descriptor_write{};
                        descriptor_write
                            .setDstSet(_descriptor_sets[shader][i])
                            .setDstBinding(binding)
                            .setDstArrayElement(0)
                            .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
                            .setDescriptorCount(1)
                            .setImageInfo(image_info);
                        ++binding;
                        descriptor_writes.push_back(descriptor_write);
                    }
                }

                if (!_uniform_buffers.empty()) {
                    vk::WriteDescriptorSet descriptor_write{};
                    descriptor_write
                        .setDstSet(_descriptor_sets[shader][i])
                        .setDstBinding(binding)
                        .setDstArrayElement(0)
                        .setDescriptorType(vk::DescriptorType::eUniformBuffer)
                        .setDescriptorCount(1)
                        .setBufferInfo(buffer_info);
                        descriptor_writes.push_back(descriptor_write);
                }

                device.updateDescriptorSets(descriptor_writes, nullptr);

            }

            uniform_buffer_offset += uniform_buffer_size;
        };

        for (const auto& shader : _vulkan_shaders) {
            updateDescriptorSets(shader);
        }
    }

    void VulkanRenderEntity::updateDescriptorSet()
    {

    }

    std::vector<vk::DescriptorSet> VulkanRenderEntity::getDescriptorSets(uint32_t image_index)
    {
        std::vector<vk::DescriptorSet> descriptor_sets;

        for (const auto& shader : _vulkan_shaders) {
            if (_descriptor_sets.count(shader)) {
                descriptor_sets.push_back(_descriptor_sets.at(shader).at(image_index));
            }
        }

        return descriptor_sets;
    }
}
#include "Vulkan/UnMacro.hh"
