#version 450

layout(set = 0, binding = 0) uniform sampler2D basicSampler;

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    // outColor = texture(basicSampler, fragTexCoord);
    outColor = fragColor;
}
