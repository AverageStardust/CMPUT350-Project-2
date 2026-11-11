#pragma once

#include <memory>
#include <vector>
#include "MathUtil.h"
#include "SFML/Graphics/RenderWindow.hpp"
namespace ECSEngine {

class InputManager {
public:
    InputManager();
    ~InputManager() = default;

    bool KeyIsDown(unsigned int keycode);
    bool KeyJustPressed(unsigned int keycode);
    bool KeyJustReleased(unsigned int keycode);
    Point2D GetMousePosition();

private:
    template <typename... Any>
    friend class ECSEngine;

    std::vector<bool> mKeyState = {};
    std::vector<bool> mLastKeyState = {};
    Point2D mMousePosition = {};

    void update(std::shared_ptr<sf::RenderWindow> window);
};

}  // namespace ECSEngine
