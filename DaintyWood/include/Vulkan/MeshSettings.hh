#pragma once

#include "pch.hh"

namespace DWE {
    struct MeshSettings {
        std::string root_dir = "./";
        std::optional<std::string> mesh_name;
        std::optional<std::string> file_path;
        std::optional<std::string> cull_mode;
    };
}
