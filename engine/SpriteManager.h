#pragma once

#include <unordered_map>
#include <utility>

#include "MathUtil.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <stdexcept>
#include "Storage.h"

namespace ECSEngine {

using SpriteID = size_t;

class SpriteManager {
public:
    SpriteManager() = default;
    ~SpriteManager() = default;
    [[nodiscard]] Ticket RegisterTexture(const std::string& texturePath, const Rect& sourceRect);
    sf::Sprite& GetSprite(Ticket id);

    Storage<std::pair<std::string, Rect>> managedSprites;

private:
};

}  // namespace ECSEngine
