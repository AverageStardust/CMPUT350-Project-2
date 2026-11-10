#include "SpriteManager.h"

namespace ECSEngine {

Ticket SpriteManager::RegisterTexture(const std::string& texturePath, const Rect& sourceRect) {
    sf::Texture newTexture;
    if (!newTexture.loadFromFile(texturePath, false, sf::IntRect({static_cast<int>(sourceRect.topLeft.x), static_cast<int>(sourceRect.topLeft.y)},
                                                                {static_cast<int>(sourceRect.Size().x), static_cast<int>(sourceRect.Size().y)})))
    {
        throw std::runtime_error("Could not load texture");
    }
    
    Ticket textureID = managedTextures.Store(newTexture);
    return managedSprites.Store(sf::Sprite(managedTextures.Get(textureID)));
}

sf::Sprite& SpriteManager::GetSprite(Ticket id) {
    return managedSprites.Get(id);
}

}  // namespace ECSEngine
