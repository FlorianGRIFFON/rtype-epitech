/*
** EPITECH PROJECT, 2023
** B-CPP-500-MAR-5-1-rtype-florian.griffon
** File description:
** World.hpp
*/


#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio.hpp>

#include <vector>
#include <iostream>
#include <memory>

#include <boost/asio.hpp>

#include <thread>
#include <mutex>

#include "SpriteStruct.hpp"

using boost::asio::ip::udp;

#define MAX_PLAYERS 4
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600
#define FPS 60

enum class GameState {
    Menu,
    Game,
    Options,
    GameOver,
    Exit,
};

struct menuStruct {
    std::string pseudo;
    std::string ipPort;
    GameState stateReturn;
};

struct posAndSprite {
    int posX;
    int posY;
    int spriteCode;
    int id;
    bool isAlive;
};

class World {
    public:
        World() : _mutex() {
            std::lock_guard<std::mutex> lock(_mutex);
        }
        ~World() {}

        void Lock() {
            _mutex.lock();
        }

        void Unlock() {
            _mutex.unlock();
        }

        void modifyPosAndSprite(int place, int x, int y) {
            _posAndSprite[place].posX = x;
            _posAndSprite[place].posY = y;
        }


        void addPosAndSprite(posAndSprite data) {
            _posAndSprite.push_back(data);
            std::cout << "Added posAndSprite" << std::endl;
        }

        int findIdIn(void) {
            int x = 0;
            for (posAndSprite elem : _posAndSprite) {
                if (elem.id == _tmp.id) {
                    return x;
                }
                x++;
            }
            return -1;
        }

        void suppresseById(int place) {
            _posAndSprite.erase(_posAndSprite.begin() + place);
        }

        std::string message;
        udp::resolver::results_type endpoints;
        udp::endpoint sender_endpoint;
        posAndSprite _tmp;
        std::vector<posAndSprite> _posAndSprite;
        std::string _keyCode;
    private:
        std::mutex _mutex;
};

menuStruct runMenu(sf::RenderWindow * window);
GameState runGame(sf::RenderWindow * window, World *w, udp::socket& socket);

// void sendMessage(udp::socket& socket, World *w);
void receiveMessages(udp::socket& socket, World *w);
void sendMessage(udp::socket& socket, World* w, uint16_t keyCode);

udp::socket * mainNetwork(boost::asio::io_context& io_context, std::string IpPort, std::string pseudo, World *w);
