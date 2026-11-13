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
    auto& som = engine.GetSoundManager();
    ECSEngine::SoundID jumpSound = som.RegisterSound(resourcePath("sfx_jump.ogg"));
    ECSEngine::SoundID landSound = som.RegisterSound(resourcePath("footstep_grass_003.ogg"));
    ECSEngine::SoundID collectSound = som.RegisterSound(resourcePath("sfx_gem.ogg"));
    ECSEngine::SoundID wallSound = som.RegisterSound(resourcePath("footstep_snow_001.ogg"));

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
    em.AddComponent(player,
                    ECSEngine::GravityComponent{.acceleration = ECSEngine::Point2D(0, 700)});
    em.AddComponent(player, ECSEngine::RespawnComponent{.loc = playerStartPos});

    // Player sprites
    std::string characterSheet = resourcePath("spritesheet-characters-default.png");
    ECSEngine::SpriteID idleSprite = sp.RegisterSprite(characterSheet, {128, 768, 128, 128});
    ECSEngine::SpriteID walk1Sprite = sp.RegisterSprite(characterSheet, {128, 128, 128, 128});
    ECSEngine::SpriteID walk2Sprite = sp.RegisterSprite(characterSheet, {128, 256, 128, 128});
    ECSEngine::SpriteID jumpSprite = sp.RegisterSprite(characterSheet, {128, 384, 128, 128});
    ECSEngine::SpriteID fallSprite = sp.RegisterSprite(characterSheet, {128, 640, 128, 128});
    ECSEngine::SpriteID dashSprite = sp.RegisterSprite(characterSheet, {128, 896, 128, 128});

    em.AddComponent(player, ECSEngine::InputComponent(idleSprite, walk1Sprite, walk2Sprite,
                                                      jumpSprite, fallSprite, dashSprite,
                                                      jumpSound, landSound, collectSound, wallSound,
                                                      1));
    em.AddComponent(
        player, ECSEngine::CollisionComponent{.localBounds = ECSEngine::Rect(-22, -70, 44, 70)});

    em.AddComponent(player, ECSEngine::SpriteComponent{
                                .spriteID = idleSprite, .offset = {-64, -128}, .worldSpace = true});

    // Setup score component
    ECSEngine::ScoreComponent scoreComp = {};
    for (int i = 0; i < 10; i++) {
        scoreComp.digitSpriteID[i] = sp.RegisterSprite(
            resourcePath("spritesheet-tiles-default.png"), {832, float(13 - i) * 64, 64, 64});
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
