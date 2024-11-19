#pragma once

#include <vector>
#include "scene/PrefabEntity.hpp"

namespace Phezu {
    
    class Prefab {
    public:
        uint64_t GetPrefabID() { return RootEntity.GetPrefabEntityID(); }
        PrefabEntity RootEntity;
    };
    
}
