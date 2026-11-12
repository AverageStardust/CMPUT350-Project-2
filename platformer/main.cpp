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
    // auto& som = engine.GetSoundManager();
    // ECSEngine::SoundID testSound = som.RegisterSound(resourcePath("sfx_gem.ogg"));

    // Load maps
    LoadMap(engine, resourcePath("sky.map"));
    LoadMap(engine, resourcePath("testWorld.map"));

    // Managers
    auto& em = engine.GetEntityManager();
    auto& sp = engine.GetSpriteManager();

    // Place player
    ECSEngine::EntityID player = em.CreateEntity();

    ECSEngine::Point2D playerStartPos = ECSEngine::Point2D(0, 0);
    em.AddComponent(player, ECSEngine::LocationComponent{.loc = playerStartPos});
    em.AddComponent(player, ECSEngine::MovementComponent{.velocity = ECSEngine::Point2D(0, 0)});
    em.AddComponent(player, ECSEngine::GravityComponent(ECSEngine::Point2D(0, 100), 0));
    em.AddComponent(player, ECSEngine::RespawnComponent{.loc = playerStartPos});

    em.AddComponent(player, ECSEngine::InputComponent());
    em.AddComponent(player,
                    ECSEngine::CollisionComponent{.localBounds = ECSEngine::Rect(-22, -70, 44, 70)});

    ECSEngine::SpriteID playerSprite = sp.RegisterSprite(
        resourcePath("spritesheet-characters-default.png"), {0 * 128, 5 * 128, 128, 128});
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
