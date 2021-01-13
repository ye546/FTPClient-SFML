#pragma once

#include "SFML-2.5.1/include/SFML/Main.hpp"
#include "SFML-2.5.1/include/SFML/Network/Ftp.hpp"
#include "SFML-2.5.1/include/SFML/Network.hpp"

#include <string>
#include <iostream>
#include <vector>

#define IP "127.0.0.1"
//#define username "admin"
//#define password "lol"
#define PORT 21

using namespace sf;

class FTPClient
{
public:

	//initialize the client inforamtion, such as IP and port
	FTPClient();

	//shut
	~FTPClient();

	bool Connect(std::string serverAdress, unsigned short port);
	bool Login(std::string user, std::string pass);
	void ListEverything();
	void Disconnect();
	std::string ReturnDirectory();
	void WriteToFile();

private:
	Ftp ftp;
	unsigned short m_Port;
	std::string m_IP;
	std::string m_username, m_password;
};

