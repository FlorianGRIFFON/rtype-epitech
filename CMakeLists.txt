# -------- CONFIGURATION --------- #
cmake_minimum_required(VERSION 3.10)

project(R-Type)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------- CLIENT --------- #

set(CLIENT r-type_client)

set(SOURCES_CLIENT
    ${CMAKE_CURRENT_SOURCE_DIR}/client/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/client/game.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/client/menu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/client/network.cpp
)

find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)
find_package(Boost 1.83.0 REQUIRED COMPONENTS filesystem)

add_executable(${CLIENT} ${SOURCES_CLIENT})

target_link_libraries(${CLIENT} PUBLIC sfml-system sfml-window sfml-graphics sfml-audio Boost::boost)

# -------- SERVER --------- #

set(SERVER  r-type_server)

set(SOURCES_SERVER
    #Main and Utilities
    ${CMAKE_CURRENT_SOURCE_DIR}/server/server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/server/levels.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/server/sendOperation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/server/Utilities.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/server/Mutex.cpp

    #Systems
    ${CMAKE_CURRENT_SOURCE_DIR}/server/Systems/MovementSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/server/Systems/ProjectileSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/server/Systems/DamageSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/server/Systems/CollisionSystem.cpp

    #Enenmy
    ${CMAKE_CURRENT_SOURCE_DIR}/server/Game/Enemy/CreateEnemy.cpp

    #Player
    ${CMAKE_CURRENT_SOURCE_DIR}/server/Game/Player/CreatePlayer.cpp

    #Projectiles
    ${CMAKE_CURRENT_SOURCE_DIR}/server/Game/Projectile/CreateProjectile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/server/Game/Projectile/Projectile.hpp
)

find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)
find_package(Boost 1.83.0 REQUIRED COMPONENTS filesystem)

add_executable(${SERVER} ${SOURCES_SERVER})

target_link_libraries(${SERVER} PUBLIC sfml-system sfml-window sfml-graphics sfml-audio Boost::boost)
