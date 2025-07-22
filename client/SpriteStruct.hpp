/*
** EPITECH PROJECT, 2023
** B-CPP-500-MAR-5-1-rtype-florian.griffon
** File description:
** SpriteStruct.hpp
*/

#include "config.hpp"

class SpriteStruct {
    public:
        SpriteStruct() {
            loadTexture("ressources/img/r-typesheet42.gif", 1);
            loadTexture("ressources/img/r-typesheet1.gif", 2);
            loadTexture("ressources/img/r-typesheet8.gif", 3);
            loadTexture("ressources/img/r-typesheet5.gif", 4);
            loadTexture("ressources/img/r-typesheet30.gif", 5);
            loadTexture("ressources/img/floor.png", 6);
            loadTexture("ressources/img/floor.png", 7);
            loadMusic("ressources/sounds/laser.ogg", 1);
            loadMusic("ressources/sounds/level1.ogg", 2);
            loadMusic("ressources/sounds/enemy_died.ogg", 3);
        }

        void loadTexture(std::string filePath, int spriteKey) {
            sf::Texture texture;
            if (!texture.loadFromFile(filePath)) {
                std::cerr << "Error loading texture: " << spriteKey << std::endl;
            }
            textureMap[spriteKey] = texture;
        }


        void loadMusic(std::string filePath, int musicKey) {
            std::unique_ptr<sf::Music> music = std::make_unique<sf::Music>();
            if (!music->openFromFile(filePath)) {
                std::cerr << "Error loading music: " << filePath << std::endl;
            }
            musicMap[musicKey] = std::move(music);
        }
        
        /*
        ** Create a sprite and add it to the spriteMap
        */

        void createEnt (int id, int spriteCode, int x, int y) {
            if (spriteCode == 1) {
                isANewPlayer(id);
                sf::Sprite sprite;
                sf::Texture& texture = textureMap[1];
                sf::IntRect rect (0, nb_player.size() == 0 ? 0 : nb_player.size() * 17, 33, 17);
                sprite.setTexture(texture);
                sprite.setScale(2, 2);
                sprite.setTextureRect(rect);
                sprite.setPosition(x, y);
                spriteMap[id] = sprite;
            } else if (spriteCode == 2) {
                sf::Sprite sprite;
                sf::Texture& texture = textureMap[3];
                sf::IntRect rect(0, 0, 33, 33);
                sprite.setTexture(texture);
                sprite.setScale(2, 2);
                sprite.setTextureRect(rect);
                sprite.setPosition(x, y);
                spriteMap[id] = sprite;
            } else if (spriteCode == 3) {
                sf::Sprite sprite;
                sf::Texture& texture = textureMap[4];
                sf::IntRect rect(0, 0, 33, 33);
                sprite.setTexture(texture);
                sprite.setScale(2, 2);
                sprite.setTextureRect(rect);
                sprite.setPosition(x, y);
                spriteMap[id] = sprite;
            } else if (spriteCode == 11) {
                sf::Sprite sprite;
                sf::Texture& texture = textureMap[2];
                sf::IntRect rect(250, 100, 15, 17);
                sprite.setTexture(texture);
                sprite.setTextureRect(rect);
                sprite.setPosition(x, y);
                spriteMap[id] = sprite;
            } else if (spriteCode == 12) {
                sf::Sprite sprite;
                sf::Texture& texture = textureMap[2];
                sf::IntRect rect(255, 275, 20, 20);
                sprite.setTexture(texture);
                sprite.setTextureRect(rect);
                sprite.setPosition(x, y);
                spriteMap[id] = sprite;
            } else if (spriteCode == 13) {
                sf::Sprite sprite;
                sf::Texture& texture = textureMap[2];
                sf::IntRect rect(255, 275, 20, 20);
                sprite.setTexture(texture);
                sprite.setScale(2, 2);
                sprite.setTextureRect(rect);
                sprite.setPosition(x, y);
                spriteMap[id] = sprite;
            } else if (spriteCode == 21) {
                sf::Sprite sprite;
                sf::Texture& texture = textureMap[5];
                sf::IntRect rect(10, 0, 100, 800);
                sprite.setTexture(texture);
                sprite.setTextureRect(rect);
                sprite.setPosition(x, y);
                spriteMap[id] = sprite;
            }
        }

        void destroyEnt (int id) {
            spriteMap.erase(id);
        }

        bool isStocked (int id) {
            if (spriteMap.find(id) != spriteMap.end())
                return true;
            return false;
        }

        bool isANewPlayer (int id) {
            if (std::find(nb_player.begin(), nb_player.end(), id) != nb_player.end())
                return false;
            nb_player.push_back(id);
            return true;
        }

        std::map<int, sf::Sprite> spriteMap;
        std::map<int, sf::Texture> textureMap;
        std::map<int, sf::Texture> texturePlayerAnimation;
        std::vector<int> nb_player;
        std::map<int, std::unique_ptr<sf::Music>> musicMap;};
