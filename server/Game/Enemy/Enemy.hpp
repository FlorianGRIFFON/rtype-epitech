/*
** EPITECH PROJECT, 2023
** B-CPP-500-MAR-5-1-rtype-florian.griffon
** File description:
** Enemy.hpp
*/

#pragma once

#include "../../config.hpp"

// Enemies mosquito create, movement and drawing
void createMosquito(struct GameStruct* gameStruct, Coordinator& gCoordinator);

// Enemies plane create, movement and drawing
void createPlane(struct GameStruct* gameStruct, Coordinator& gCoordinator);

// Enemies alienBoss create, movement and drawing
void createAlienBoss(struct GameStruct* gameStruct, Coordinator& gCoordinator);
