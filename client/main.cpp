#include "config.hpp"

int main(int ac, char **av) {
    sf::RenderWindow * window = new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "R-Type");
    auto currentState = GameState::Menu;
    World* w = new World();
    boost::asio::io_context io_context;
    udp::socket* socket;
    while (GameState::Exit != currentState) {
        if (currentState == GameState::Menu) {
            // io_context.reset();
            menuStruct menu = runMenu(window);
            currentState = menu.stateReturn;
            socket = mainNetwork(io_context, "127.0.0.1:9876", "leo", w);
            // socket = mainNetwork(io_context, menu.ipPort, menu.pseudo, w); // "127.0.0.1:9876"
        } else if (currentState == GameState::Game) {
            std::thread io_thread([&io_context]() { io_context.run(); });
            runGame(window, w, *socket);
            io_thread.join();
        } else if (currentState == GameState::Exit) {
            currentState = GameState::Game;
        }
    }
    return 0;
}
