#pragma once

#include <cstdlib>

#include "CollisionComponent.h"
#include "EntityManager.h"
#include "InputManager.h"
#include "LocationComponent.h"
#include "SoundManager.h"
#include "SpriteManager.h"
#include "SystemManager.h"
#include "WindowManager.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace ECSEngine {

template <typename... Components>
class EngineView {
public:
    virtual SoundManager& GetSoundManager() = 0;
    virtual SpriteManager& GetSpriteManager() = 0;
    virtual EntityManager<Components...>& GetEntityManager() = 0;
    virtual WindowManager& GetWindowManager() = 0;
    virtual InputManager& GetInputManager() = 0;
};

template <typename... Components>
class ECSEngine : public EngineView<Components...> {
public:
    ECSEngine(unsigned int width, unsigned int height, const std::string& name)
        : mWindowManager(width, height, name) {};
    ~ECSEngine() {};

    void Run() {
        std::shared_ptr<sf::RenderWindow> window = mWindowManager.GetWindow();

        while (window->isOpen()) {
            // cleanup old objects
            mSoundManager.Cleanup();

            mInputManager.update(window);

            // clear background
            window->clear(sf::Color::Black);

            // run systems
            if (!mSystemManager.Run(this)) {
                window->close();
            }

            if (debug) {
                DebugDraw();
            }

            // display window
            window->display();
        }
    };

    SoundManager& GetSoundManager() override { return mSoundManager; }

    SpriteManager& GetSpriteManager() override { return mSpriteManager; }

    EntityManager<Components...>& GetEntityManager() override { return mEntityManager; }

    WindowManager& GetWindowManager() override { return mWindowManager; }

    SystemManager<Components...>& GetSystemManager() { return mSystemManager; }

    InputManager& GetInputManager() { return mInputManager; }

private:
    bool ProcessEvents() { return true; };
    void DebugDraw() {
        mWindowManager.SetWorldCenter(mInputManager.GetMousePosition() + 200);

        // draw entities (temperary for testings)
        for (Entity entity : mEntityManager) {
            if (entity.template HasComponent<LocationComponent>() &&
                entity.template HasComponent<CollisionComponent>()) {
                LocationComponent& location =
                    mEntityManager.template GetComponent<LocationComponent>(entity);
                CollisionComponent& collision =
                    mEntityManager.template GetComponent<CollisionComponent>(entity);

                Rect rect = collision.localBoundingBox + location.loc;

                mWindowManager.DebugFrameRect(rect, 255, 0, 0, 255);
            }
        }
    };

    EntityManager<Components...> mEntityManager;
    SystemManager<Components...> mSystemManager;
    SpriteManager mSpriteManager;
    SoundManager mSoundManager;
    WindowManager mWindowManager;
    InputManager mInputManager;

    bool debug = true;
};

}  // namespace ECSEngine
