/*
** EPITECH PROJECT, 2023
** B-CPP-500-MAR-5-1-rtype-florian.griffon
** File description:
** Player.hpp
*/

#pragma once

#include "../../config.hpp"

// Player movement and drawing
void createPlayer(struct GameStruct *gameStruct, Coordinator& gCoordinator, std::string name, udp::endpoint endpoint);
