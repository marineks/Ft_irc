#include "Client.hpp"

Client::Client(pollfd client_poll_fd) : _client_poll_fd(client_poll_fd)
{
	std::cout << YELLOW << "Client constructor" << RESET << std::endl;
}

Client::~Client()
{
	std::cout << YELLOW << "Client destructor" << RESET << std::endl;
}
