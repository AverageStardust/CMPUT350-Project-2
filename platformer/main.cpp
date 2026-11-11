#include "main.h"

#include "Entity.h"
#include "ScoreComponent.h"
#include "SoundManager.h"
#include "loadMap.h"
#include "resource.h"

int main(int argc, char* argv[]) {
    setupResources(argc, argv);

    // Engine test code
    Engine engine(1024, 768, "Test Engine");

    // Load sounds
    //auto& som = engine.GetSoundManager();
    //ECSEngine::SoundID testSound = som.RegisterSound(resourcePath("sfx_gem.ogg"));

    // Load maps
    LoadMap(engine, resourcePath("sky.map"));
    LoadMap(engine, resourcePath("world.map"));

    // Place player
    auto& em = engine.GetEntityManager();
    // ECSEngine::EntityID player = em.CreateEntity();

    // Setup score component
    auto& sp = engine.GetSpriteManager();
    ECSEngine::ScoreComponent scoreComp = {};
    for (int i = 0; i < 10; i++) {
        scoreComp.digitSpriteID[i] = sp.RegisterSprite(
            resourcePath("spritesheet-tiles-default.png"), {13 * 64, float(13 - i) * 64, 64, 64});
    }

    ECSEngine::EntityID score = em.CreateEntity();
    em.AddComponent(score, scoreComp);

    // Setup Camera

    // Add all systems
    auto& sm = engine.GetSystemManager();
    sm.AddSystem(std::make_unique<ECSEngine::CollisionLateSystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::InputSystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::GravitySystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::MovementSystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::CollisionSystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::ScoreSystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::CameraSystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::SpriteSystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::SpawnSystem<COMPONENTS>>());

    engine.Run();
    return 0;
}
