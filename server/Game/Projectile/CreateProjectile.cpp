/*
** EPITECH PROJECT, 2023
** B-CPP-500-MAR-5-1-rtype-florian.griffon
** File description:
** CreateProjectile.cpp
*/

#include "Projectile.hpp"

void createProjectilesPlayer(struct GameStruct* gameStruct, Coordinator& gCoordinator, udp::endpoint sender_endpoint)
{
    bool alive = gCoordinator.GetComponent<SpriteComponent>(endpointToEntityMap[sender_endpoint]).isAlive;

    if (alive == false)
        return;

    Entity projectile = gCoordinator.CreateEntity();
    Vec2 posComponent = gCoordinator.GetComponent<PositionComponent>(endpointToEntityMap[sender_endpoint]).position;
    Vec2 spriteComp = gCoordinator.GetComponent<SpriteComponent>(endpointToEntityMap[sender_endpoint]).size;

    gCoordinator.AddComponent(projectile, PositionComponent(posComponent.x + spriteComp.x, posComponent.y + spriteComp.y / 2.0f));
    gCoordinator.AddComponent(projectile, SpriteComponent(Vec2(12.0f, 12.0f), 11, gameStruct->entityID, true));
    gCoordinator.AddComponent(projectile, LifeComponent(1));

    gameStruct->entityID++;

    gameStruct->projectilesPlayerVector.push_back(projectile);
}

void createPlaneProjectile(struct GameStruct* gameStruct, Coordinator& gCoordinator, Vec2 position)
{
    Entity projectile = gCoordinator.CreateEntity();
    Vec2 posProjectile(position.x + 40, position.y + 50);

    int r = rand() % endpointToEntityMap.size();
    auto it = endpointToEntityMap.begin();
    std::advance(it, r);
    Entity player = it->second;

    SpriteComponent playerSprite = gCoordinator.GetComponent<SpriteComponent>(player);
    PositionComponent playerPos = gCoordinator.GetComponent<PositionComponent>(player);
    Vec2 playerPosMiddle(playerPos.position.x + (playerSprite.size.x / 2), playerPos.position.y + (playerSprite.size.y / 2));

    Vec2 projectileVector(playerPosMiddle.x - posProjectile.x,playerPosMiddle.y - posProjectile.y);

    gCoordinator.AddComponent(projectile, PositionComponent(posProjectile));
    gCoordinator.AddComponent(projectile, ProjectileVectorComponent(projectileVector));
    gCoordinator.AddComponent(projectile, SpriteComponent(Vec2(18.0f, 18.0f), 12, gameStruct->entityID, true));
    gCoordinator.AddComponent(projectile, LifeComponent(1));

    gameStruct->entityID++;

    gameStruct->projectilesPlaneVector.push_back(projectile);
}

void createStraightProjectile(struct GameStruct* gameStruct, Coordinator& gCoordinator, Vec2 position)
{
    Entity projectile = gCoordinator.CreateEntity();
    Vec2 posProjectile(position.x, position.y);

    gCoordinator.AddComponent(projectile, PositionComponent(posProjectile));
    gCoordinator.AddComponent(projectile, SpriteComponent(Vec2(15.0f, 15.0f), 13, gameStruct->entityID, true));
    gCoordinator.AddComponent(projectile, LifeComponent(1));

    gameStruct->entityID++;

    gameStruct->projectilesStraightVector.push_back(projectile);
}
