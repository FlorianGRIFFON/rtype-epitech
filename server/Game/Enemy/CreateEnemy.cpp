/*
** EPITECH PROJECT, 2023
** B-CPP-500-MAR-5-1-rtype-florian.griffon
** File description:
** CreateEnemy.cpp
*/

#include "Enemy.hpp"

void createMosquito(struct GameStruct* gameStruct, Coordinator& gCoordinator)
{
    Entity enemyMosquito = gCoordinator.CreateEntity();
    int r = endpointToEntityMap.size();
    int rd = rand() % r;
    auto it = endpointToEntityMap.begin();
    std::advance(it, rd);
    Entity player = it->second;

    int life = 2;
    if (r != 1) {
        life = r * 1;
    }

    Vec2 playerPosition = gCoordinator.GetComponent<PositionComponent>(player).position;
    Vec2 playerSize = gCoordinator.GetComponent<SpriteComponent>(player).size;
    Vec2 enemyPosition(SCREEN_WIDTH + 100, playerPosition.y + playerSize.y / 2.0f);

    gCoordinator.AddComponent(enemyMosquito, PositionComponent(enemyPosition));
    gCoordinator.AddComponent(enemyMosquito, InitialPositionComponent(enemyPosition));
    gCoordinator.AddComponent(enemyMosquito, SpriteComponent(Vec2(66.0f, 66.0f), 2, gameStruct->entityID, true));
    gCoordinator.AddComponent(enemyMosquito, LifeComponent(life));
    
    gameStruct->entityID++;

    gameStruct->enemiesMosquitoVector.push_back(enemyMosquito);
}

void createPlane(struct GameStruct* gameStruct, Coordinator& gCoordinator)
{
    Entity enemyPlane = gCoordinator.CreateEntity();
    Vec2 enemyPosition(SCREEN_WIDTH + 50, SCREEN_HEIGHT * 0.1f);

    int r = endpointToEntityMap.size();
    int life = 2;
    if (r != 1) {
        life = r * 1;
    }

    gCoordinator.AddComponent(enemyPlane, PositionComponent(enemyPosition));
    gCoordinator.AddComponent(enemyPlane, InitialPositionComponent(enemyPosition));
    gCoordinator.AddComponent(enemyPlane, SpriteComponent(Vec2(66.0f, 66.0f), 3, gameStruct->entityID, true));
    gCoordinator.AddComponent(enemyPlane, LifeComponent(life));

    gameStruct->entityID++;

    gameStruct->enemiesPlaneVector.push_back(enemyPlane);
}

void createAlienBoss(struct GameStruct* gameStruct, Coordinator& gCoordinator)
{
    Entity enemyAlienBoss = gCoordinator.CreateEntity();
    Vec2 enemyPosition(SCREEN_WIDTH + 10, 0.0f);
    int r = endpointToEntityMap.size();
    int life = r * 100;

    gCoordinator.AddComponent(enemyAlienBoss, PositionComponent(enemyPosition));
    gCoordinator.AddComponent(enemyAlienBoss, InitialPositionComponent(enemyPosition));
    gCoordinator.AddComponent(enemyAlienBoss, SpriteComponent(Vec2(100.0f, SCREEN_HEIGHT), 21, gameStruct->entityID, true));
    gCoordinator.AddComponent(enemyAlienBoss, LifeComponent(life));

    gameStruct->entityID++;

    gameStruct->enemiesAlienBossVector.push_back(enemyAlienBoss);
}
