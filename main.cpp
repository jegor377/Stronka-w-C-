#include <iostream>
#include "simple_http.hpp"
 
using namespace std;
 
int main()
{
	simple_tcp::init();

	auto http_server = new simple_http::HTTPServer("127.0.0.1", 8080);
	http_server->listen();

	delete http_server;

	simple_tcp::kill();
}