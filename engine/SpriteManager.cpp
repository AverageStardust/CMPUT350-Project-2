#include "SpriteManager.h"

namespace ECSEngine {

Ticket SpriteManager::RegisterTexture(const std::string& texturePath, const Rect& sourceRect) {
    std::pair<std::string, Rect> spriteValues = std::make_pair(texturePath, sourceRect);
    return managedSprites.Store(spriteValues);
}

sf::Sprite& SpriteManager::GetSprite(Ticket id) {
    auto spriteValues = managedSprites.Get(id);
    std::string& texturePath = spriteValues.first;
    Rect sourceRect = spriteValues.second;
    static sf::Texture dummyTexture;
    if (!dummyTexture.loadFromFile(texturePath, false, sf::IntRect({static_cast<int>(sourceRect.topLeft.x), static_cast<int>(sourceRect.topLeft.y)},
                                                                {static_cast<int>(sourceRect.Size().x), static_cast<int>(sourceRect.Size().y)})))
    {
        throw std::runtime_error("Could not load texture");
    }
    static sf::Sprite dummy(dummyTexture);
    return dummy;
}

}  // namespace ECSEngine
