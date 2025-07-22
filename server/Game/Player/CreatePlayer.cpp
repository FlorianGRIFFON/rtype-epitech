/*
** EPITECH PROJECT, 2023
** B-CPP-500-MAR-5-1-rtype-florian.griffon
** File description:
** CreatePlayer.cpp
*/

#include "Player.hpp"

void createPlayer(struct GameStruct *gameStruct, Coordinator& gCoordinator, std::string name, udp::endpoint endpoint)
{
    Entity entity = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(entity, PositionComponent(getRandomPos()));
    gCoordinator.AddComponent(entity, SpriteComponent(Vec2(66.0f, 34.0f), 1, gameStruct->entityID, true));
    gCoordinator.AddComponent(entity, PlayerComponent(name, endpoint));
    gCoordinator.AddComponent(entity, LifeComponent(3));

    gameStruct->entityID++;

    endpointToEntityMap[endpoint] = entity;
}
