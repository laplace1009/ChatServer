#include "pch.h"
#include <iostream>
#include "Server.h"

using namespace std;

int main(void)
{
	Server server;
	server.Run(8000);
	
	return 0;
}