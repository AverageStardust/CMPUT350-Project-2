#pragma once

#include <unordered_map>

#include "MathUtil.h"
#include "Storage.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace ECSEngine {

using SpriteID = size_t;

class SpriteManager {
public:
    SpriteManager() = default;
    ~SpriteManager() = default;
    [[nodiscard]] Ticket RegisterSprite(const std::string& texturePath, const Rect& sourceRect);
    sf::Sprite& GetSprite(Ticket id);

    std::unordered_map<std::string, sf::Texture> managedTextures;
    Storage<sf::Sprite> managedSprites;

private:
};

}  // namespace ECSEngine
