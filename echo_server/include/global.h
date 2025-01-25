#pragma once
#include <iostream>
#include <vector>
#include <cctype>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace
{
	const int SERVER_PORT = 8080;
	const int BackLog = 128;
}

static void Error_Hint(const string& function)
{
	cout << "[Hint]" << function << "() error,reason:" << strerror(errno) << endl;
	exit(EXIT_FAILURE);
}
