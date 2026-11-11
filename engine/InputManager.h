#pragma once

#include <bitset>
#include <memory>

#include "InputSystem.h"
#include "MathUtil.h"
#include "SFML/Graphics/RenderWindow.hpp"

namespace ECSEngine {

class InputManager {
public:
    InputManager() = default;
    ~InputManager() = default;

    bool KeyIsDown(unsigned int keycode);
    bool KeyJustPressed(unsigned int keycode);
    bool KeyJustReleased(unsigned int keycode);
    Point2D GetMousePosition();

private:
    template <typename... Any>
    friend class ECSEngine;

    std::bitset<Key::Count> mKeyDown;
    std::bitset<Key::Count> mLastKeyDown;
    Point2D mMousePosition;

    void update(std::shared_ptr<sf::RenderWindow> window);
};

}  // namespace ECSEngine
