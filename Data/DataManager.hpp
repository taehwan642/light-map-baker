#pragma once
#include <string>
#include <vector>
#include <memory>
#include "GLM/vec3.hpp"
#include "../Renderer/Mesh.hpp"

namespace LightmapBaker
{
namespace Data
{
    class DataManager
    {
    private:
    public:
        void Save(std::string path);
        std::vector<std::shared_ptr<Renderer::Mesh>> Load(const std::string& path);
        // for debug
        std::vector<std::shared_ptr<Renderer::Mesh>> Load();
        std::vector<glm::vec3> GetVertices();
    };
}
}
