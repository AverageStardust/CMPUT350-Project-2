#include "WindowManager.h"

#include "MathUtil.h"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"

namespace ECSEngine {

WindowManager::WindowManager(unsigned int width, unsigned int height, const std::string& title)
    : mWindow(std::make_shared<sf::RenderWindow>(sf::VideoMode({width, height}), title)) {}

std::shared_ptr<sf::RenderWindow> WindowManager::GetWindow() const { return mWindow; }

Point2D WindowManager::GetWindowSize() const {
    sf::Vector2u size = mWindow->getSize();
    return Point2D(size.x, size.y);
}

Rect WindowManager::GetWindowBounds() const {
    sf::Vector2u size = mWindow->getSize();
    return Rect(0, 0, size.x, size.y);
}

void WindowManager::DebugDrawRect(const Rect& rect, int red, int green, int blue, int alpha) {
    Rect windowRect = WorldToWindow(rect);

    sf::RectangleShape sfRect(windowRect.Size().AsSfmlVectorF());
    sfRect.setPosition(windowRect.topLeft.AsSfmlVectorF());
    sfRect.setFillColor(sf::Color(red, green, blue, alpha));

    mWindow->draw(sfRect);
}

void WindowManager::DebugFrameRect(const Rect& rect, int red, int green, int blue, int alpha) {
    Rect windowRect = WorldToWindow(rect);

    sf::RectangleShape sfRect(windowRect.Size().AsSfmlVectorF());
    sfRect.setPosition(windowRect.topLeft.AsSfmlVectorF());
    sfRect.setFillColor(sf::Color::Transparent);
    sfRect.setOutlineColor(sf::Color(red, green, blue, alpha));
    sfRect.setOutlineThickness(std::max(1.0f, 1.0f / mWorldUnitsPerPixel));

    mWindow->draw(sfRect);
}

void WindowManager::DrawSpriteWorld(sf::Sprite& sprite, Rect bounds) {
    DrawSpriteWindow(sprite, bounds);
}

void WindowManager::DrawSpriteWindow(sf::Sprite& sprite, Rect bounds) {
    sprite.setPosition(bounds.topLeft.AsSfmlVectorF());

    Point2D textureSize = sprite.getTextureRect().size;
    sprite.setScale(bounds.Size().PairwiseDiv(textureSize).AsSfmlVectorF());

    mWindow->draw(sprite);
}

Rect WindowManager::WindowToWorld(const Rect& rect) const {
    if (!rect.Exists()) return rect;
    return Rect(WindowToWorld(rect.topLeft), WindowToWorld(rect.bottomRight));
}

Point2D WindowManager::WindowToWorld(const Point2D& pt) const {
    return ((pt - GetWindowSize() / 2.0f) * mWorldUnitsPerPixel) + mCameraPosition;
}

Rect WindowManager::WorldToWindow(const Rect& rect) const {
    if (!rect.Exists()) return rect;
    return Rect(WorldToWindow(rect.topLeft), WorldToWindow(rect.bottomRight));
}

Point2D WindowManager::WorldToWindow(const Point2D& pt) const {
    return (pt - mCameraPosition) / mWorldUnitsPerPixel + GetWindowSize() / 2.0f;
}

Point2D WindowManager::GetWorldCenter() const { return mCameraPosition; }

float WindowManager::GetWorldScale() const { return mWorldUnitsPerPixel; }

void WindowManager::SetWorldCenter(const Point2D& worldCenter) { mCameraPosition = worldCenter; }

void WindowManager::SetWorldScale(float worldUnitsPerPixel) {
    mWorldUnitsPerPixel = worldUnitsPerPixel;
}

void WindowManager::SetWorldRect(const Rect& worldCameraRect) {
    mCameraPosition = worldCameraRect.Center();
    Point2D worldSize = worldCameraRect.Size();
    Point2D windowSize = GetWindowSize();

    // zoom out to capture entire worldCameraRect along both axies
    mWorldUnitsPerPixel = std::max(worldSize.x / windowSize.x, worldSize.y / windowSize.y);
}

float WindowManager::WindowToWorld(float x) const { return x * mWorldUnitsPerPixel; }

float WindowManager::WorldToWindow(float x) const { return x / mWorldUnitsPerPixel; }

}  // namespace ECSEngine
