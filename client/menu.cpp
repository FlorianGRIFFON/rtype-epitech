#include "config.hpp"

menuStruct runMenu(sf::RenderWindow * window)
{
    sf::Font font;
    if (!font.loadFromFile("ressources/fonts/arial.ttf")) {}
    sf::Font title;
    if (!title.loadFromFile("ressources/fonts/stocky.ttf")) {}

    sf::Text menuTitle;
    menuTitle.setFont(title);
    menuTitle.setCharacterSize(116);
    menuTitle.setFillColor(sf::Color::White);
    menuTitle.setString("R-type");
    float xPos = (window->getSize().x - menuTitle.getGlobalBounds().width) / 2.0f;
    menuTitle.setPosition(xPos, 15);

    sf::RectangleShape inputIp(sf::Vector2f(300, 30));
    inputIp.setFillColor(sf::Color::White);
    inputIp.setOutlineThickness(2);
    inputIp.setOutlineColor(sf::Color::White);
    inputIp.setPosition(50, (window->getSize().x) / 2.0f - 100);

    sf::Text inputIpText;
    inputIpText.setFont(font);
    inputIpText.setCharacterSize(16);
    inputIpText.setFillColor(sf::Color::Black);
    inputIpText.setPosition(60, (window->getSize().x) / 2.0f - 100);
    inputIpText.setString("Ip + port");

    sf::RectangleShape inputName(sf::Vector2f(300, 30));
    inputName.setFillColor(sf::Color::White);
    inputName.setOutlineThickness(2);
    inputName.setOutlineColor(sf::Color::White);
    inputName.setPosition(50, (window->getSize().x) / 2.0f - 50);

    sf::Text inputNameText;
    inputNameText.setFont(font);
    inputNameText.setCharacterSize(16);
    inputNameText.setFillColor(sf::Color::Black);
    inputNameText.setPosition(60, (window->getSize().x) / 2.0f - 50);
    inputNameText.setString("Name");

    sf::RectangleShape buttonReqPlay(sf::Vector2f(150, 50));
    buttonReqPlay.setFillColor(sf::Color::Red);
    buttonReqPlay.setOutlineThickness(2);
    buttonReqPlay.setOutlineColor(sf::Color::White);
    buttonReqPlay.setPosition((window->getSize().x - buttonReqPlay.getGlobalBounds().width) / 2.0f, 
                            (window->getSize().y - buttonReqPlay.getGlobalBounds().height) / 2.0f);

    sf::Text buttonPlay("Play", font, 30);
    buttonPlay.setFillColor(sf::Color::White);
    float buttonPlayX = (window->getSize().x - buttonPlay.getGlobalBounds().width) / 2.0f;
    float buttonPlayY = (window->getSize().y - buttonPlay.getLocalBounds().height - 10) /  2.0f;
    buttonPlay.setPosition(buttonPlayX, buttonPlayY);

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("./ressources/bgt.png"))
    {
        menuStruct menu;
        menu.stateReturn = GameState::Exit;
        return menu;
    }

    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(
        static_cast<float>(window->getSize().x) / backgroundTexture.getSize().x,
        static_cast<float>(window->getSize().y) / backgroundTexture.getSize().y
    );

    std::string ipText = "";
    std::string nameText = "";
    bool isInputIpActive = false;
    bool isInputNameActive = false;

    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("./ressources/sounds/meunu.ogg")) {
        menuStruct menu;
        menu.stateReturn = GameState::Exit;
        return menu;
    }

    // Créer une instance de son et la jouer
    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();

            if (event.type == sf::Event::TextEntered) {
                if (isInputIpActive) {
                    if (event.text.unicode == '\b' && ipText.size() > 0) {
                        ipText.pop_back();
                    }
                    else if (event.text.unicode != '\b') {
                        ipText += static_cast<char>(event.text.unicode);
                    }
                    inputIpText.setString(ipText);
                }
                else if (isInputNameActive) {
                    if (event.text.unicode == '\b' && nameText.size() > 0) {
                        nameText.pop_back();
                    }
                    else if (event.text.unicode != '\b') {
                        nameText += static_cast<char>(event.text.unicode);
                    }
                    inputNameText.setString(nameText);
                }
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
                sf::FloatRect buttonPlayBounds = buttonReqPlay.getGlobalBounds();
                if (buttonPlayBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    menuStruct menu;
                    menu.stateReturn = GameState::Game;
                    menu.ipPort = ipText;
                    menu.pseudo = nameText;
                    return menu;
                }
                sf::FloatRect inputIpBounds = inputIp.getGlobalBounds();
                if (inputIpBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    isInputIpActive = true;
                    isInputNameActive = false;
                }
                else {
                    isInputIpActive = false;
                }
                sf::FloatRect inputNameBounds = inputName.getGlobalBounds();
                if (inputNameBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    isInputNameActive = true;
                    isInputIpActive = false;
                }
                else {
                    isInputNameActive = false;
                }
            }
        }

        window->clear();
        window->draw(backgroundSprite);
        window->draw(inputName);
        window->draw(buttonReqPlay);
        window->draw(buttonPlay);
        window->draw(inputIp);
        window->draw(inputIpText);
        window->draw(inputNameText);
        window->draw(menuTitle);
        window->display();
    }

    menuStruct menu;
    menu.stateReturn = GameState::Exit;
    return menu;
}