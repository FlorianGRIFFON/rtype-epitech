/*
** EPITECH PROJECT, 2023
** B-CPP-500-MAR-5-1-rtype-florian.griffon
** File description:
** config.hpp
*/

#pragma once

#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <map>
#include <SFML/Graphics.hpp>

#include "Maths/Vec2.hpp"

#include "Core/EntityManager.hpp"
#include "Core/ComponentManager.hpp"
#include "Core/SystemManager.hpp"
#include "Core/Coordinator.hpp"

#include "Components/PositionComponent.hpp"
#include "Components/InitialPositionComponent.hpp"
#include "Components/SpriteComponent.hpp"
#include "Components/ProjectileVectorComponent.hpp"
#include "Components/PlayerComponent.hpp"
#include "Components/LifeComponent.hpp"

#include "Game/Projectile/Projectile.hpp"
#include "Systems/MovementSystem.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/DamageSystem.hpp"
#include "Systems/ProjectileSystem.hpp"

#include "Game/Enemy/Enemy.hpp"
#include "Game/Player/Player.hpp"
#include "Game/Projectile/Projectile.hpp"

#include "../shared/Config.hpp"

using boost::asio::ip::udp;

sf::Color getRandomColor();
Vec2 getRandomPos();
bool sendEntityData(const Entity entity, udp::socket& socket);
void sendStructPeriodically(udp::socket& socket, struct GameStruct *gameStruct);
void createMosquitoSafely(struct GameStruct *gameStruct);
void createPlaneSafely(struct GameStruct *gameStruct);
void createAlienBossSafely(struct GameStruct *gameStruct);
void createProjectilesPlayerSafely(struct GameStruct *gameStruct, udp::endpoint sender_endpoint);
bool revivePlayers(struct GameStruct *gameStruct);
void killEveryEntities(struct GameStruct *gameStruct);
bool checkIfLevel(struct GameStruct *gameStruct);
bool checkIfAlive();

extern std::map<udp::endpoint, Entity> endpointToEntityMap;

typedef struct GameStruct
{
    std::shared_ptr<MovementSystem> movementSystem;
    std::shared_ptr<ProjectileSystem> projectileSystem;
    std::shared_ptr<CollisionSystem> collisionSystem;
    std::shared_ptr<DamageSystem> damageSystem;

    std::vector<Entity> projectilesPlayerVector;
    std::vector<Entity> enemiesMosquitoVector;
    std::vector<Entity> enemiesPlaneVector;
    std::vector<Entity> enemiesAlienBossVector;
    std::vector<Entity> projectilesPlaneVector;
    std::vector<Entity> projectilesStraightVector;

    int entityID;
    bool startLevelOne;
    bool startLevelTwo;

    GameStruct (Coordinator& gCoordinator) {
        gCoordinator.RegisterComponent<PositionComponent>();
        gCoordinator.RegisterComponent<PlayerComponent>();
        gCoordinator.RegisterComponent<SpriteComponent>();
        gCoordinator.RegisterComponent<LifeComponent>();
        gCoordinator.RegisterComponent<InitialPositionComponent>();
        gCoordinator.RegisterComponent<ProjectileVectorComponent>();

        movementSystem = gCoordinator.RegisterSystem<MovementSystem>();
        projectileSystem = gCoordinator.RegisterSystem<ProjectileSystem>();
        collisionSystem = gCoordinator.RegisterSystem<CollisionSystem>();
        damageSystem = gCoordinator.RegisterSystem<DamageSystem>();

        startLevelOne = false;
        startLevelTwo = false;
        entityID = 0;
    }
} GameStruct_t;
