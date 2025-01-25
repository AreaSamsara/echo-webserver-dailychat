#include "../include/global.h"

int main(void)
{
	//set server socket
	int server_socket = socket(AF_INET,SOCK_STREAM,0);
	
	//set server address
	sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(SERVER_PORT);

	//bind server socket with server address
	if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == -1)
	{
		Error_Hint("bind");
	}

	//listen
	if (listen(server_socket, BackLog) == -1)
	{
		Error_Hint("listen");
	}
	cout << "The server is running:" << endl;

	bool Round2 = false;	//The label which indicts whether it's the second time client is connected to the server or not
	while (true)
	{
		//set client address
		sockaddr_in client_addr;
		memset(&client_addr, 0, sizeof(client_addr));
		socklen_t client_addr_length = sizeof(client_addr);

		//accept client socket
		int client_socket=accept(server_socket,(sockaddr*)&client_addr,&client_addr_length);
		if (client_socket == -1)
		{
			Error_Hint("accept");
		}

		//print the information of the client 
		char client_ip[16] = {};
		cout << "Connect successfully." << endl;
		cout << "client IP: " << inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip,sizeof(client_ip));
		cout << "\t" << "client port: " << ntohs(client_addr.sin_port) << endl;

		char buffer[256] = {};
		vector<string> discources = { "Yes, I do.","Because I can use it to write programs.","And you ?" };
		for (int i = 0; i < discources.size(); ++i)
		{
			//try to read the discource from socket
			int read_length = read(client_socket, buffer, sizeof(buffer) - 1);
			//fail to read
			if (read_length == -1)
			{
				Error_Hint("read");
			}
			//The client has disconnected from the server
			else if (read_length == 0)
			{
				--i;	//continue as if this round didn't take place
			}
			//read successfully
			else
			{
				buffer[read_length] = '\0';
				cout << "C:" << buffer << endl;

				//if it's the second time client is connected to the server,terminate the process
				if (Round2)
				{
					break;
				}

				//try to write discource to the client
				//fail to write
				if (write(client_socket, discources[i].c_str(), discources[i].size()) == -1)
				{
					Error_Hint("write");
				}
				//write successfully
				else
				{
					cout << "S:" << discources[i] << endl;
				}

				//write the last 2 discources at once
				if (i == discources.size() - 2)
				{
					++i;
					if (write(client_socket, discources[i].c_str(), discources[i].size()) == -1)
					{
						Error_Hint("write");
					}
					else
					{
						cout << "S:" << discources[i] << endl;
					}

					//close the client socket deliberately
					close(client_socket);
					cout << "The server has closed the client_socket.Wait for 5 seconds..." << endl;
					//open the socket again after 5 seconds' waiting
					sleep(5);
					cout << "Okay,try again." << endl;
					Round2 = true;
				}
			}
		}
	}

	close(server_socket);
	return 0;
}
