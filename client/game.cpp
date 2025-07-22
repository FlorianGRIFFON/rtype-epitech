#include "config.hpp"
#include <thread>
#include <chrono>

void updateBackgroundPositions(sf::Sprite &bg1, sf::Sprite &bg2, float &bgX1, float &bgX2, float bgSpeed, const sf::RenderWindow &window) {
    bgX1 -= bgSpeed;
    bgX2 -= bgSpeed;
    
    if (bgX1 + bg1.getGlobalBounds().width < 0) {
        bgX1 = bg2.getPosition().x + bg2.getGlobalBounds().width;
    }    
    if (bgX2 + bg2.getGlobalBounds().width < 0) {
        bgX2 = bg1.getPosition().x + bg1.getGlobalBounds().width;
    }
    bg1.setPosition(bgX1, 0);
    bg2.setPosition(bgX2, 0);
}

GameState runGame(sf::RenderWindow * window, World *w, udp::socket &socket) {

    GameState currentState = GameState::Game;
    SpriteStruct * spriteStruct = new SpriteStruct();
    sf::Clock clock;
    sf::Clock clockMove;
    sf::Time time = clock.getElapsedTime();
    sf::Time lastMoveTime = time;
    sf::Time lastShootTime = time;
    int animationControl = 0;

    spriteStruct->musicMap[2]->play();

    sf::Texture backgroundTexture1;
    if (!backgroundTexture1.loadFromFile("ressources/bgt.png")) {
        // les erreurs de chargement de texture
    }

    sf::Sprite backgroundSprite1(backgroundTexture1);
    sf::Sprite backgroundSprite2(backgroundTexture1);
    backgroundSprite1.setScale(
        static_cast<float>(window->getSize().x) / backgroundTexture1.getSize().x,
        static_cast<float>(window->getSize().y) / backgroundTexture1.getSize().y
    );
    backgroundSprite2.setScale(
        static_cast<float>(window->getSize().x) / backgroundTexture1.getSize().x,
        static_cast<float>(window->getSize().y) / backgroundTexture1.getSize().y
    );


    float backgroundSpeed = 0.01f;
    float backgroundX1 = 0.0f;
    float backgroundX2 = backgroundSprite1.getGlobalBounds().width;



    while (currentState == GameState::Game) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                exit(0);
            }
        }

        updateBackgroundPositions(backgroundSprite1, backgroundSprite2, backgroundX1, backgroundX2, backgroundSpeed, *window);

        time = clockMove.getElapsedTime();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            uint16_t value = 42;

            uint8_t bytes[2];
            bytes[0] = static_cast<uint8_t>(value);
            bytes[1] = static_cast<uint8_t>(value >> 8);

            sendMessage(socket, w, value);
            exit(0);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            uint16_t value = 5;

            uint8_t bytes[2];
            bytes[0] = static_cast<uint8_t>(value);
            bytes[1] = static_cast<uint8_t>(value >> 8);

            sendMessage(socket, w, value);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            uint16_t value = 21;

            uint8_t bytes[2];
            bytes[0] = static_cast<uint8_t>(value);
            bytes[1] = static_cast<uint8_t>(value >> 8);

            sendMessage(socket, w, value);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
            uint16_t value = 21;

            uint8_t bytes[2];
            bytes[0] = static_cast<uint8_t>(value);
            bytes[1] = static_cast<uint8_t>(value >> 8);

            sendMessage(socket, w, value);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            uint16_t value = 22;

            uint8_t bytes[2];
            bytes[0] = static_cast<uint8_t>(value);
            bytes[1] = static_cast<uint8_t>(value >> 8);

            sendMessage(socket, w, value);
        }
        // Move
        if (time > lastMoveTime + sf::milliseconds(20)) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                uint16_t value = 3;

                uint8_t bytes[2];
                bytes[0] = static_cast<uint8_t>(value);
                bytes[1] = static_cast<uint8_t>(value >> 8);
                animationControl = 3;
                sendMessage(socket, w, value);
                lastMoveTime = time;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                uint16_t value = 4;

                uint8_t bytes[2];
                bytes[0] = static_cast<uint8_t>(value);
                bytes[1] = static_cast<uint8_t>(value >> 8);
                animationControl = 0;
                sendMessage(socket, w, value);
                lastMoveTime = time;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                uint16_t value = 2;

                uint8_t bytes[2];
                bytes[0] = static_cast<uint8_t>(value);
                bytes[1] = static_cast<uint8_t>(value >> 8);
                animationControl = 2;
                sendMessage(socket, w, value);
                lastMoveTime = time;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                uint16_t value = 1;

                uint8_t bytes[2];
                bytes[0] = static_cast<uint8_t>(value);
                bytes[1] = static_cast<uint8_t>(value >> 8);
                animationControl = 4;
                sendMessage(socket, w, value);
                lastMoveTime = time;
            } else {
                animationControl = 1;
            }
        }
        // Shoot
        if (time > lastShootTime + sf::milliseconds(200)) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                uint16_t value = 6;

                uint8_t bytes[2];
                bytes[0] = static_cast<uint8_t>(value);
                bytes[1] = static_cast<uint8_t>(value >> 8);

            spriteStruct->musicMap[1]->play();


                sendMessage(socket, w, value);
                lastShootTime = time;
            }
        }

        window->clear();
        /*
         * Draw bg
        */
        window->draw(backgroundSprite2);
        window->draw(backgroundSprite1);
        /*
         * Draw bg
        */
        w->Lock();
        if (!w->_posAndSprite.empty()) {
            for (posAndSprite elem : w->_posAndSprite) {
                if (!spriteStruct->isStocked(elem.id)) {
                    spriteStruct->createEnt(elem.id, elem.spriteCode, elem.posX, elem.posY);
                }
                if (elem.isAlive == false) {
                    spriteStruct->destroyEnt(elem.id);
                    spriteStruct->musicMap[3]->play();
                } else {
                    if (elem.spriteCode == 1) {
                        sf::Texture& texture = spriteStruct->textureMap[1];
                        sf::IntRect rectInit = spriteStruct->spriteMap[elem.id].getTextureRect();
                        sf::IntRect rect ((animationControl == 0 ? 0 : 33 * animationControl), rectInit.top, 33, 17);
                        spriteStruct->spriteMap[elem.id].setTextureRect(rect);
                        spriteStruct->spriteMap[elem.id].setTexture(texture);
                        spriteStruct->spriteMap[elem.id].setPosition(elem.posX, elem.posY);
                        window->draw(spriteStruct->spriteMap[elem.id]);
                    } else {
                        spriteStruct->spriteMap[elem.id].setPosition(elem.posX, elem.posY);
                        window->draw(spriteStruct->spriteMap[elem.id]);
                    }
                }
            }
        }
        /*
         * Draw décor
        */
        // window->draw(backgroundDecor1);
        /*
         * Draw décor
        */
        w->Unlock();
        window->display();
    }
    return currentState;
}