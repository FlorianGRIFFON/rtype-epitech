#pragma once
#include <boost/asio.hpp>
#include <iostream>

#include "../Maths/Vec2.hpp"

using boost::asio::ip::udp;

struct PlayerComponent
{
    udp::endpoint endpoint;
	std::string name;

	PlayerComponent() {
		name = "none";
	}

	PlayerComponent(std::string namee, udp::endpoint endp) {
		name = namee;
		endpoint = endp;
	}
};
