#include "InputManager.h"

#include <memory>

#include "Keyboard.h"
#include "MathUtil.h"
#include "SFML/Graphics/RenderWindow.hpp"

namespace ECSEngine {

InputManager::InputManager() {
    mKeyState.resize(Key::Count);
    mLastKeyState.resize(Key::Count);
}

bool InputManager::KeyIsDown(unsigned int keycode) {
    if (keycode >= mKeyState.size()) return false;
    return mKeyState[keycode];
}
bool InputManager::KeyJustPressed(unsigned int keycode) {
    if (keycode >= mKeyState.size()) return false;
    return mKeyState[keycode] && !mLastKeyState[keycode];
}
bool InputManager::KeyJustReleased(unsigned int keycode) {
    if (keycode >= mKeyState.size()) return false;
    return mLastKeyState[keycode] && !mKeyState[keycode];
}
Point2D InputManager::GetMousePosition() { return mMousePosition; }

void InputManager::update(std::shared_ptr<sf::RenderWindow> window) {
    // check for events
    copy(mKeyState.begin(), mKeyState.end(), back_inserter(mLastKeyState));

    while (const std::optional event = window->pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window->close();
        } else if (event->is<sf::Event::MouseMoved>()) {
            sf::Vector2i pos = event->getIf<sf::Event::MouseMoved>()->position;
            mMousePosition = {(float)pos.x, (float)pos.y};
        } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            mKeyState[int(keyPressed->scancode)] = true;
        } else if (const auto* keyPressed = event->getIf<sf::Event::KeyReleased>()) {
            mKeyState[int(keyPressed->scancode)] = false;
        }
    }
}

}  // namespace ECSEngine
