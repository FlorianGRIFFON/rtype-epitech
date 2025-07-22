/*
** EPITECH PROJECT, 2023
** B-CPP-500-MAR-5-1-rtype-florian.griffon
** File description:
** Projectile.hpp
*/

#pragma once

#include "../../config.hpp"

void createProjectilesPlayer(struct GameStruct* gameStruct, Coordinator& gCoordinator, udp::endpoint sender_endpoint);

void createPlaneProjectile(struct GameStruct* gameStruct, Coordinator& gCoordinator, Vec2 position);

void createStraightProjectile(struct GameStruct* gameStruct, Coordinator& gCoordinator, Vec2 position);
