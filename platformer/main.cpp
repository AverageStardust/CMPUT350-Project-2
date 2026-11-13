#include "main.h"

#include "CameraComponent.h"
#include "Entity.h"
#include "ScoreComponent.h"
#include "SoundManager.h"
#include "loadMap.h"
#include "resource.h"

int main(int argc, char* argv[]) {
    srand(time(0));
    setupResources(argc, argv);

    // Engine test code
    Engine engine(1024, 768, "Test Engine");

    // Load sounds
    // auto& som = engine.GetSoundManager();
    // ECSEngine::SoundID testSound = som.RegisterSound(resourcePath("sfx_gem.ogg"));

    // Managers
    auto& em = engine.GetEntityManager();
    auto& sp = engine.GetSpriteManager();

    // Load maps
    ECSEngine::SpriteID spawnerSprite = sp.RegisterSprite(
        resourcePath("spritesheet-tiles-default.png"), {10 * 64 + 12, 5 * 64 + 12, 40, 40});
    LoadMap(engine, resourcePath("sky.map"), spawnerSprite, {40, 40});
    LoadMap(engine, resourcePath("world.map"), spawnerSprite, {40, 40});

    // Place player
    ECSEngine::EntityID player = em.CreateEntity();

    ECSEngine::Point2D playerStartPos = ECSEngine::Point2D(832, 512);
    em.AddComponent(player, ECSEngine::LocationComponent{.loc = playerStartPos});
    em.AddComponent(player, ECSEngine::MovementComponent{.velocity = ECSEngine::Point2D(0, 0)});
    em.AddComponent(player, ECSEngine::GravityComponent{.acceleration = ECSEngine::Point2D(0, 700)});
    em.AddComponent(player, ECSEngine::RespawnComponent{.loc = playerStartPos});

    em.AddComponent(player, ECSEngine::InputComponent());
    em.AddComponent(
        player, ECSEngine::CollisionComponent{.localBounds = ECSEngine::Rect(-22, -70, 44, 70)});

    ECSEngine::SpriteID playerSprite = sp.RegisterSprite(
        resourcePath("spritesheet-characters-default.png"), {1 * 128, 6 * 128, 128, 128});
    em.AddComponent(player,
                    ECSEngine::SpriteComponent{
                        .spriteID = playerSprite, .offset = {-64, -128}, .worldSpace = true});

    // Setup score component
    ECSEngine::ScoreComponent scoreComp = {};
    for (int i = 0; i < 10; i++) {
        scoreComp.digitSpriteID[i] = sp.RegisterSprite(
            resourcePath("spritesheet-tiles-default.png"), {13 * 64, float(13 - i) * 64, 64, 64});
    }

    ECSEngine::EntityID score = em.CreateEntity();
    em.AddComponent(score, scoreComp);

    // Setup Camera
    ECSEngine::EntityID camera = em.CreateEntity();
    em.AddComponent(camera, ECSEngine::CameraComponent{.position = playerStartPos});
    em.AddComponent(camera, ECSEngine::CameraFollower{.targetID = player,
                                                      .hardExclusion = {256, 0, 512, 768},
                                                      .softExclusion = {448, 0, 128, 768},
                                                      .speed = 128});
    em.AddComponent(camera, ECSEngine::CameraShake{});

    // Add all systems
    auto& sm = engine.GetSystemManager();
    sm.AddSystem(std::make_unique<ECSEngine::InputSystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::GravitySystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::MovementSystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::CollisionLateSystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::CollisionSystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::ScoreSystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::CameraSystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::SpriteSystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::SpawnSystem<COMPONENTS>>());

    engine.Run();
    return 0;
}
