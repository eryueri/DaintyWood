#pragma once

namespace DWE {
    enum class TextureType : uint8_t { // add more in the futrue
        ImageFile, 
    };

    enum class AddressMode : uint8_t {
        Repeat, 
        MirroredRepeat, 
        ClampToEdge, 
        ClampToBorder, 
        MirrorClampToEdge
    };

    enum class BorderColor : uint8_t {
        IntOpaqueBlack, FloatOpaqueBlack, 
        IntTransparentBlack, FloatTransparentBlack, 
        IntOpaqueWhite, FloatOpaqueWhite, 
    };

    struct TextureSettings {
        std::string root_dir = "./";
        std::optional<std::string> file_path;
        std::optional<std::string> texture_name;
        std::optional<std::string> texture_type;
        std::optional<std::string> address_mode;
        std::optional<std::string> border_color;
        std::optional<std::string> sampler_name;
    };
}
