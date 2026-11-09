#pragma once

#include "MathUtil.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace ECSEngine {

class WindowManager {
public:
    WindowManager(unsigned int width, unsigned int height, const std::string& title);
    ~WindowManager() = default;
    std::shared_ptr<sf::RenderWindow> GetWindow() const;
    Point2D GetWindowSize() const;
    Rect GetWindowBounds() const;

    void SetWorldCenter(const Point2D& worldCenter);
    void SetWorldRect(const Rect& worldCameraRect);
    void SetWorldScale(float worldUnitsPerPixel);
    Point2D GetWorldCenter() const;
    float GetWorldScale() const;

    float WindowToWorld(float x) const;
    float WorldToWindow(float x) const;
    Rect WindowToWorld(const Rect& rect) const;
    Rect WorldToWindow(const Rect& rect) const;
    Point2D WorldToWindow(const Point2D& pt) const;
    Point2D WindowToWorld(const Point2D& pt) const;

    void DebugDrawRect(const Rect& rect, int red, int green, int blue, int alpha = 255);
    void DebugFrameRect(const Rect& rect, int red, int green, int blue, int alpha = 255);

private:
    std::shared_ptr<sf::RenderWindow> mWindow;
    Point2D mCameraPosition;
    float mWorldUnitsPerPixel = 1;
};

}  // namespace ECSEngine
