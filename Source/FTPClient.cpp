#include "FTPClient.hpp"
#include <fstream> //just for testing, delete later
#include <numeric> // for accumulate

FTPClient::FTPClient()
{

}

//shut
FTPClient::~FTPClient()
{
	Ftp::Response close;
	std::cout << "Status: " << close.getStatus() << std::endl;
	std::cout << "Message: " << close.getMessage() << std::endl;
	close = this->ftp.disconnect();
	std::cout << "\n\nDisconnected.";
	//getchar();
}

bool FTPClient::Connect(std::string serverAdress, unsigned short port)
{
	std::fstream ConnectReport("ConnectReport.txt", std::fstream::out);
	
	//set variables
	m_IP = serverAdress;
	m_Port = port;

	Ftp::Response response = this->ftp.connect(m_IP, m_Port, sf::seconds(10));

	if (response.isOk())
	{
		ConnectReport << "Response status: " << response.getStatus() << std::endl << std::endl;
		ConnectReport << "Response message: " << response.getMessage() << std::endl << std::endl;
		ConnectReport << "\n";
		ConnectReport << "Connected to [" << m_IP << "] on port << [" << m_Port << "]\n";
		return true;
	}
	else
	{
		/*std::cout << "Response status: " << response.getStatus() << std::endl << std::endl;
		std::cout << "Response message: " << response.getMessage() << std::endl << std::endl;
		std::cout << "Error\n" << std::endl;*/

		ConnectReport << "Response status: " << response.getStatus() << std::endl << std::endl;
		ConnectReport << "Response message: " << response.getMessage() << std::endl << std::endl;
		ConnectReport << "Error\n" << std::endl;
		return false;
	}
}

bool FTPClient::Login(std::string user, std::string pass)
{
	std::fstream LoginReport("LoginReport.txt", std::fstream::out);
	
	Ftp::Response response = this->ftp.login(user, pass);
	if (response.isOk())
	{
		LoginReport << "Response status: " << response.getStatus() << std::endl << std::endl;
		LoginReport << "Response message: " << response.getMessage() << std::endl << std::endl;
		LoginReport << "\n";
		LoginReport << "logged in with user " << user << std::endl;
		return true;
	}
	else
	{
		/*std::cout << "Response status: " << response.getStatus() << std::endl << std::endl;
		std::cout << "Response message: " << response.getMessage() << std::endl << std::endl;
		std::cout << "Error\n" << std::endl;*/

		LoginReport << "Response status: " << response.getStatus() << std::endl << std::endl;
		LoginReport << "Response message: " << response.getMessage() << std::endl << std::endl;
		LoginReport << "Error\n" << std::endl;

		return false;
	}

}

std::string FTPClient::ReturnDirectory() {
	Ftp::DirectoryResponse response = this->ftp.getWorkingDirectory(); // get current working directory, what does that mean?
	if (response.isOk())
	{
		std::cout << "Current directory is: \"" << response.getDirectory() << "\"" << std::endl;
	}
	

	Ftp::ListingResponse listresponse = this->ftp.getDirectoryListing();
	if (listresponse.isOk()) 
	{
		const std::vector<std::string>& DirContent = listresponse.getListing();
		std::string tmp;
		
		for (auto& i : DirContent)
		{
			//std::cout << "Directory content:\n" << i << std::endl;
			tmp += i + "\t";
		}
		return tmp;
	}
}

void FTPClient::ListEverything()
{
retry:
	//std::fstream temp("ListEverythingReport.txt", std::fstream::out);

	Ftp::DirectoryResponse response = this->ftp.getWorkingDirectory(); // get current working directory, what does that mean?

	if (response.isOk())
	{
		std::cout << "Current directory is: \"" << response.getDirectory() << "\"" << std::endl;
	//	temp << "response is ok.\n";
	}
	//else
	//{
	//	temp << "directory was not ok.\n";
	//}

	Ftp::ListingResponse listresponse = this->ftp.getDirectoryListing();

	if (listresponse.isOk()) {
		//temp << "listresponse is ok.\n";
		const std::vector<std::string> DirContent = listresponse.getListing();
		for (auto i : DirContent)
		{
			std::cout << "Directory content:\n" << i << std::endl;
			//temp << i;
		}
	}
	//else
	//	temp << "getDirectoryListing was not ok.\n";
}

void FTPClient::Disconnect()
{
	this->~FTPClient();
}
