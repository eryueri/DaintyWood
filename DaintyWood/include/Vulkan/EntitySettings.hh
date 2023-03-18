#pragma once

#include "pch.hh"

namespace DWE {
    enum class EntityType {
        RenderEntity, 
        ComputeEntity
    };

    struct EntitySettings {
        std::optional<std::string> entity_type;
        std::optional<std::string> cull_mode;
    };

    struct EntityResourceData {
        std::vector<std::optional<std::string>> mesh_names;
        std::vector<std::optional<std::string>> texture_names;
        std::vector<std::optional<std::string>> shader_names;
    };
}
