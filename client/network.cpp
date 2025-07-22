#include "config.hpp"

void receiveMessages(udp::socket& socket, World *w) {
    socket.async_receive_from(boost::asio::buffer(&w->_tmp, sizeof(w->_tmp)), w->sender_endpoint,
[&, w](const boost::system::error_code& error, std::size_t bytes_transferred) {
            // std::cout << "Receive messages" << std::endl;
            if (!error && bytes_transferred == sizeof(posAndSprite)) {
                w->Lock();
                // std::cout << "Received posAndSprite struct:" << std::endl;
                // std::cout << "posX: " << w->_tmp.posX << std::endl;
                // std::cout << "posY: " << w->_tmp.posY << std::endl;
                // std::cout << "spriteCode: " << w->_tmp.spriteCode << std::endl;
                // std::cout << "id: " << w->_tmp.id << std::endl;
                // std::cout << "isAlive: " << w->_tmp.isAlive << std::endl << std::endl;
                int place = w->findIdIn();
                if (place == -1) {
                    w->_posAndSprite.push_back(w->_tmp);
                    // std::cout << "Add sprite" << std::endl;
                }
                if (place != -1) {
                    w->_posAndSprite[place] = w->_tmp;
                    w->modifyPosAndSprite(place, w->_tmp.posX, w->_tmp.posY);         
                    // std::cout << "Replace sprite" << std::endl;
                }
                for (posAndSprite elem : w->_posAndSprite) {
                    if (elem.isAlive == false && elem.spriteCode != 1) {
                        w->suppresseById(place);
                    }
                }
                w->Unlock();
            }
            receiveMessages(socket, w);
        });
}

// void sendMessage(udp::socket& socket, World* w) {
//     uint16_t value = 3;
//     uint8_t bytes[2];
//     bytes[0] = static_cast<uint8_t>(value);
//     bytes[1] = static_cast<uint8_t>(value >> 8);
//     socket.async_send_to(boost::asio::buffer(w->_keyCode), *w->endpoints.begin(), [&, w](const boost::system::error_code& error, std::size_t bytes_transferred) {
//         if (error) {
//             std::cerr << "Error: SENDING FAILED / " << error.message() << std::endl;
//         }
//         sendMessage(socket, w);
//     });
// }

void sendMessage(udp::socket& socket, World* w, uint16_t keyCode) {
    socket.send_to(boost::asio::buffer(&keyCode, 2), *w->endpoints.begin());
    return;
}


udp::socket * mainNetwork(boost::asio::io_context& io_context, std::string IpPort, std::string pseudo, World *w) {
    std::string ip = IpPort.substr(0, IpPort.find(":"));
    std::string port = IpPort.substr(IpPort.find(":") + 1, IpPort.size());
    std::cout << "ip : " << ip << std::endl;
    std::cout << "port : " << port << std::endl;
    
    udp::socket* socket = new udp::socket(io_context, udp::endpoint(udp::v4(), 0));

    udp::resolver resolver(io_context);
    w->endpoints = resolver.resolve(udp::v4(), ip, port);

    socket->send_to(boost::asio::buffer(pseudo), *w->endpoints.begin());
    receiveMessages(*socket, w);

    return socket;
}
