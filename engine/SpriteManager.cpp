#include "SpriteManager.h"

namespace ECSEngine {

Ticket SpriteManager::RegisterTexture(const std::string& texturePath, const Rect& sourceRect) {
    sf::Texture texture;

    if (managedTextures.contains(texturePath)) {
        // already cached, use it again
        texture = managedTextures[texturePath];
    } else {
        // load this new texture
        if (!texture.loadFromFile(texturePath, false)) {
            throw std::runtime_error("Could not load texture");
        }

        managedTextures[texturePath] = texture;
    }

    return managedSprites.Store(sf::Sprite(texture, sourceRect.AsSfmlRectI()));
}

sf::Sprite& SpriteManager::GetSprite(Ticket id) { return managedSprites.Get(id); }

}  // namespace ECSEngine
