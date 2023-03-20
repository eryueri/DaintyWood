#include "Vulkan/VulkanRenderEntity.hh"

#include "Vulkan/VulkanInstance.hh"

#include "Vulkan/Macro.hh"
namespace DWE {
    VulkanRenderEntity::VulkanRenderEntity(VulkanInstance* instance, EntitySettings settings)
        : _vulkan_instance(instance)
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
        _vulkan_shaders.push_back(shader);
    }

    void VulkanRenderEntity::initialize()
    {
        for (VulkanShader* shader : _vulkan_shaders) {
            if (shader->getShaderType() == ShaderType::Vertex) {
                _vertex_data_flags = shader->getVertexDataFlag();
                _vertex_shader = shader;
            }
            if (shader->getShaderType() == ShaderType::Fragment) {
                _frag_shader = shader;
            }
        }

        createPipelineLayout();
        createPipeline();
    }

    void VulkanRenderEntity::writeDrawingCommands(uint32_t image_index)
    {
        vk::CommandBuffer command_buffer = _vulkan_instance->getRenderCommandBuffer(image_index);
        command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, _graphics_pipeline);
        if (!_vulkan_textures.empty()) {
            for (VulkanTexture* texture : _vulkan_textures) {
                texture->writeDrawingCommands(image_index);
            }
        }

        if (!_vulkan_meshes.empty()) {
            for (VulkanMesh* mesh : _vulkan_meshes) {
                mesh->writeDrawingCommands(_vertex_data_flags, image_index);
            }
        }
        command_buffer.draw(3, 1, 0, 0);
    }

    void VulkanRenderEntity::createPipelineLayout()
    {
        std::vector<vk::DescriptorSetLayout> descriptor_set_layout{};
        if (!_vertex_shader || !_frag_shader) {
            throw std::runtime_error("shader needed not all initialized...");
        }
        vk::DescriptorSetLayout vertex_descriptor_set_layout = _vertex_shader->getDescriptorSetLayout();
        // CHECK_NULL(vertex_descriptor_set_layout);
        descriptor_set_layout.push_back(vertex_descriptor_set_layout);
        vk::DescriptorSetLayout fragment_descriptor_set_layout = _frag_shader->getDescriptorSetLayout();
        // CHECK_NULL(fragment_descriptor_set_layout);
        descriptor_set_layout.push_back(fragment_descriptor_set_layout);

        vk::PipelineLayoutCreateInfo pipeline_layout_create_info{};
        pipeline_layout_create_info
            .setSetLayoutCount(0)
            .setPSetLayouts(nullptr)
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
            .setDepthTestEnable(false)
            .setDepthWriteEnable(false);

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
            .setLayout(_pipeline_layout) // meight need refractor for furture use of multi render pass
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

    void VulkanRenderEntity::createDescriptorPool()
    {

    }

    void VulkanRenderEntity::createDescriptorSets()
    {

    }

    void VulkanRenderEntity::updateDescriptorSets()
    {

    }

    void VulkanRenderEntity::updateDescriptorSet()
    {

    }
}
#include "Vulkan/UnMacro.hh"
