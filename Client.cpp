#include "Client.hpp"

Client::Client(pollfd client_poll_fd) : _client_poll_fd(client_poll_fd)
{
	std::cout << "Client constructor\n";
}

Client::~Client()
{
	std::cout << "Client destructor\n";
}
