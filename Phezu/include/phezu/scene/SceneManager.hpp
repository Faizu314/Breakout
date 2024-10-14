#pragma once

#include <memory>
#include <vector>

namespace Phezu {
    
    class SceneManager {
    public:
        std::weak_ptr<Scene> CreateScene();
        std::weak_ptr<Scene> GetActiveScene() const;
        void LoadScene(std::weak_ptr<Scene> scene) const;
    private:
        std::vector<std::shared_ptr<Scene>> m_AllScenes;
    };
}
