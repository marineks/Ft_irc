#include "Client.hpp"

Client::Client(pollfd clientPollFd) : _clientPollFd(clientPollFd)
{
	std::cout << "Client constructor\n";
}

Client::~Client()
{
	std::cout << "Client destructor\n";
}
