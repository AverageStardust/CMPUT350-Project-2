#pragma once

#include <cstdlib>

#include "CollisionComponent.h"
#include "EntityManager.h"
#include "LocationComponent.h"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
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
            // check for events
            while (const std::optional event = window->pollEvent()) {
                if (event->is<sf::Event::Closed>()) window->close();
                if (event->is<sf::Event::MouseMoved>()) {
                    sf::Vector2i pos = event->getIf<sf::Event::MouseMoved>()->position;
                    mWindowManager.SetWorldCenter({(float)pos.x, (float)pos.y});
                }
            }

            if (!mSystemManager.Run(this)) {
                window->close();
            }

            window->clear(sf::Color::Black);

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

            window->display();
        }
    };

    SoundManager& GetSoundManager() override { return mSoundManager; }

    SpriteManager& GetSpriteManager() override { return mSpriteManager; }

    EntityManager<Components...>& GetEntityManager() override { return mEntityManager; }

    WindowManager& GetWindowManager() override { return mWindowManager; }

    SystemManager<Components...>& GetSystemManager() { return mSystemManager; }

private:
    bool ProcessEvents() { return true; };
    void DebugDraw() {};

    EntityManager<Components...> mEntityManager;
    SystemManager<Components...> mSystemManager;
    SpriteManager mSpriteManager;
    SoundManager mSoundManager;
    WindowManager mWindowManager;

    // For debugging - pause and step the engine
    bool running, step;
    int debug;
};

}  // namespace ECSEngine
