#include "test_case_http_download.h"
#include "server.h"
#include "test_case.h"
#include "netdb.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "netinet/ip_icmp.h"
#include "strings.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "sys/socket.h"
#include "cstdint"
#include "iostream"

TestCaseHttpDownload::TestCaseHttpDownload(Server server, std::string test_name)
    : TestCase(server, test_name) {}

void TestCaseHttpDownload::run_test(){
    struct sockaddr_in server;

    bzero(&server, sizeof(server));

    int socket_descriptor;
    if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        this->test_result = 0;
        return;
    }

    uint16_t http_port = 80;
    hostent *host = gethostbyname(reinterpret_cast<const char*>(this->server.get_server_name().c_str()));
    if (host == NULL){
        this->test_result = 0;
        return;
    }
    in_addr *host_address = reinterpret_cast<in_addr*>(host->h_addr);
    std::string ip_address = inet_ntoa(host_address[0]);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip_address.c_str());
	server.sin_port = htons(http_port);

    if ((connect(socket_descriptor, reinterpret_cast<struct sockaddr*>(&server), sizeof(struct sockaddr))) == -1){
		this->test_result = 0;
		return;
	}
    std::string message = "GET / HTTP/1.1\r\nHost: ";
    message+= (this->server.get_server_name() + "\r\nConnection: close\r\n\r\n");

    if (send(socket_descriptor, message.c_str(), message.size(), 0) == -1){
        this->test_result = 0;
        return;
    }
    char buffer[100] = {};
    bzero(buffer, sizeof(buffer));
    if ((recv(socket_descriptor, buffer , 100 , 0)) == -1) {
		this->test_result = 0;
        return;
	}
    std::string res = buffer;
    if (res.substr(0, 15) == "HTTP/1.1 200 OK"){
        this->test_result = 1;
        return;
    }
    else{
        this->test_result = 0;
    }
}