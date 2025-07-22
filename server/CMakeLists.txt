cmake_minimum_required(VERSION 3.15)

project(server)

set(NAME server)

set(CMAKE_CXX_STANDARD 11)
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

set(SOURCES
    server.cpp
    Systems/MovementSystem.cpp
    Systems/ProjectileSystem.cpp
    Systems/DamageSystem.cpp
    Systems/CollisionSystem.cpp
    Utilities.cpp
)

add_executable(${NAME} ${SOURCES})

target_include_directories(${NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Components
        ${CMAKE_CURRENT_SOURCE_DIR}/Core
        ${CMAKE_CURRENT_SOURCE_DIR}/Maths
        ${CMAKE_CURRENT_SOURCE_DIR}/Systems
        )

target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics)