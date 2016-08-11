#include "config.h"
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fstream>
#include <sstream>

using namespace std;

int sock = 0;
int cli_sock = 0;

struct HTTPRequest {
	string method = "GET";
	string target = "/";
	string HTTP_version = "HTTP/1.1";
};

string error404() {
	string response("HTTP/1.1 404 Not Found\r\n\r\n<html><title>404</title><body><p><h1>404 - Page Not Found</h1></p></body></html>\r\n");
	return response;
}

HTTPRequest parseRequest(string input) {
	HTTPRequest request;
	
	stringstream ins(input);
	ins >> request.method;
	ins >> request.target;
	ins >> request.HTTP_version;
	
	return request;
}

string generateResponse(HTTPRequest request) {
	string result = "";
	if (request.method == "GET") {
		if (request.target.size() == 1 && request.target[0] == '/') {
			request.target = DEFAULT_PAGE;
		} else if (request.target[0] == '/') {
			request.target = request.target.substr(1,request.target.size()-1);
		}
		ifstream ifs(request.target);
		ifs >> noskipws;
		
		stringstream buf;
		buf << ifs.rdbuf();
                
                result = "HTTP/1.1 200 OK\r\n\r\n";
		result += buf.str();
		
		if (result.size() == 0) result = error404();
		return result;
	}
	
}

string generateHeader(string response) {
    
}

void handle_ctrl_c(int signum) {
	close(sock);	
        close(cli_sock);
	exit(0);
}

int main(int argc, char** argv) {
	signal(SIGINT,handle_ctrl_c);
	int port = DEFAULT_LISTEN_PORT;
        if (argc > 1) {
            port = atoi(argv[1]);
        }
	string host = "0.0.0.0";
	sock = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in address;
	memset(&address,'0',sizeof(address));

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);
	
	bind(sock,(const sockaddr *)&address,sizeof(address));
	listen(sock,16);
	socklen_t len = sizeof(address);
	while (true) {
		cli_sock = accept(sock,(sockaddr *)&address,&len);
		if (cli_sock > 0) {
			char buf[1024];
			memset(buf,'\0',1024);
			int size = recv(cli_sock,buf,1024,0);
			cout << "New Request:\n" << buf << endl;
			string response = "";
			
			string request_message = string(buf);
			
			HTTPRequest req = parseRequest(request_message);
			response = generateResponse(req);
			
			send(cli_sock,(const void *)response.c_str(),response.size(),0);
		}
		close(cli_sock);
	}
}
