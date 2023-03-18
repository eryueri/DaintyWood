#include "Vulkan/VulkanMesh.hh"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Vulkan/ShaderSettings.hh"

namespace DWE {
    VulkanMesh::VulkanMesh(MeshSettings settings) 
        : _mesh_name(settings.mesh_name.value())
    {
        Assimp::Importer importer{};
        const aiScene* scene = importer.ReadFile(
                settings.root_dir + settings.file_path.value(), 
                aiProcess_Triangulate | 
                aiProcess_FlipUVs);

        processNode(scene->mRootNode, scene);
    }

    void VulkanMesh::processNode(aiNode* node, const aiScene* scene) 
    {
        for (uint32_t i = 0; i < node->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            processMesh(mesh, scene);
        }

        for (uint32_t i = 0; i < node->mNumChildren; ++i) {
            processNode(node->mChildren[i], scene);
        }
    }

    void VulkanMesh::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        for (uint32_t i = 0; i < mesh->mNumVertices; ++i) {
            _vertex_positions.push_back({mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z});
            _vertex_normals.push_back({mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z});
            if (mesh->mTextureCoords[0])
                _vertex_textures.push_back({mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y});
            else
                _vertex_textures.push_back({0.0f, 0.0f});
            if (mesh->mColors[0])
                _vertex_colors.push_back({mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b, mesh->mColors[0][i].a});
            else 
                _vertex_colors.push_back({1.0f, 1.0f, 1.0f, 1.0f});
        }

        for (uint32_t i = 0; i < mesh->mNumFaces; ++i) {
            aiFace& face = mesh->mFaces[i];
            for (uint32_t j = 0; j < face.mNumIndices; ++j) {
                _vertex_indexs.push_back(face.mIndices[j]);
            }
        }

        // maybe in the near future we'll use material, but not today
    }

    void VulkanMesh::setVertexDataFlags(uint8_t flags) 
    {
        _vertex_data_flags = flags;
    }

    void VulkanMesh::createGeometryBuffers()
    {
        if (!_vertex_data_flags) return;

        if (_vertex_data_flags & VertexDataFlag::Position) {

        }

        if (_vertex_data_flags & VertexDataFlag::Color) {

        }

        if (_vertex_data_flags & VertexDataFlag::Texcoord) {

        }

        if (_vertex_data_flags & VertexDataFlag::Normal) {

        }
    }
}
