#include "InputManager.h"

#include <memory>

#include "Keyboard.h"
#include "MathUtil.h"
#include "SFML/Graphics/RenderWindow.hpp"

namespace ECSEngine {

bool InputManager::KeyIsDown(unsigned int keycode) {
    if (keycode >= mKeyDown.size()) return false;
    return mKeyDown[keycode];
}
bool InputManager::KeyJustPressed(unsigned int keycode) {
    if (keycode >= mKeyDown.size()) return false;
    return mKeyDown[keycode] && !mLastKeyDown[keycode];
}
bool InputManager::KeyJustReleased(unsigned int keycode) {
    if (keycode >= mKeyDown.size()) return false;
    return mLastKeyDown[keycode] && !mKeyDown[keycode];
}
Point2D InputManager::GetMousePosition() { return mMousePosition; }

void InputManager::update(std::shared_ptr<sf::RenderWindow> window) {
    // check for events

    for (size_t i = 0; i < mKeyDown.size(); i++) {
        mLastKeyDown[i] = mKeyDown[i];
    }

    while (const std::optional event = window->pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window->close();
        } else if (event->is<sf::Event::MouseMoved>()) {
            sf::Vector2i pos = event->getIf<sf::Event::MouseMoved>()->position;
            mMousePosition = {(float)pos.x, (float)pos.y};
        } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            mKeyDown[int(keyPressed->scancode)] = true;
        } else if (const auto* keyPressed = event->getIf<sf::Event::KeyReleased>()) {
            mKeyDown[int(keyPressed->scancode)] = false;
        }
    }
}

}  // namespace ECSEngine
