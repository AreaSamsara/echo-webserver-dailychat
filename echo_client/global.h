#pragma once
#include <iostream>
#include <vector>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace
{
	const int SERVER_PORT = 666;
	const string SERVER_IP = "127.0.0.1";
}

void ERROR_Hint(const string& function)
{
	cout << "[Hint]" << function << " error,reason: " << strerror(errno) << endl;
	exit(EXIT_FAILURE);
}