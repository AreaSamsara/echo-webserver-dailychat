#include "../include/global.h"

int main(void)
{
	//set client socket
	int client_socket = socket(AF_INET,SOCK_STREAM,0);
	if (client_socket == -1)
	{
		ERROR_Hint("socket");
	}

	//set server_address
	sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	inet_pton(AF_INET,SERVER_IP.c_str(), &server_addr.sin_addr);
	server_addr.sin_port = htons(SERVER_PORT);

	//try to connect for the first time
	if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == -1)
	{
		ERROR_Hint("connect");
	}


	char buffer[256] = {};
	vector<string> discources = { "Do you like C++?","Why do you like C++?","Me too." };
	for (int i = 0; i < discources.size(); ++i)
	{
		//write the discource to the socket
		int write_length = write(client_socket,discources[i].c_str(), discources[i].size());
		//fail to write
		if (write_length == -1)
		{
			ERROR_Hint("write");
		}
		//write successfully
		else	
		{
			//print the discource sent
			cout << "C: " << discources[i] << endl;

			//try to read the discource from the server
			int read_length = read(client_socket, buffer, sizeof(buffer)-1);
			//disconnect from the server
			if (read_length == 0)	
			{
				cout << "Disconnected from the server.Try to reconnect..." << endl;
				//reset the socket in order to reconnect to the server.It is necessary.
				client_socket = socket(AF_INET, SOCK_STREAM, 0);
				if (client_socket == -1)
				{
					ERROR_Hint("socket");
				}

				//try to reconnect until succeed
				while (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == -1)
				{
					sleep(1);
					cout << "Keep trying..." << endl;
				}
				cout << "Reconnect successfully!" << endl;

				//write the discource that was discarded
				write_length = write(client_socket, discources[i].c_str(), discources[i].size());
				if (write_length == -1)
				{
					ERROR_Hint("write");
				}
			}
			//read successfully
			else	
			{
				buffer[read_length] = '\0';		//set '\0'
				cout << "S:" << buffer << endl;
			}
		}
	}

	cout << "Finished." << endl;
	close(client_socket);

	return 0;
}