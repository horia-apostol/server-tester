#include "test_case_latency.h"
#include "server.h"
#include "test_case.h"
#include "netdb.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "netinet/ip_icmp.h"
#include "strings.h"
#include "arpa/inet.h"
#include "unistd.h"


TestCaseLatency::TestCaseLatency(Server server, std::string test_name)
    : TestCase(server, test_name) {}

double TestCaseLatency::get_latency(){
    return this->latency;
}
void TestCaseLatency::run_test(){
    hostent *host = gethostbyname(reinterpret_cast<const char*>(this->server.get_server_name().c_str()));
    if (host == NULL){
        this->test_result = 0;
        return;
    }
    struct sockaddr_in server;
    struct sockaddr_in from;

    bzero(&server, sizeof(server));
    bzero(&from, sizeof(from));

    in_addr *host_address = reinterpret_cast<in_addr*>(host->h_addr);
    std::string ip_address = inet_ntoa(host_address[0]);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip_address.c_str());
    
    int socket_descriptor;
    if ((socket_descriptor = socket(AF_INET, SOCK_RAW, 1) == -1)){
        return;
    }

    struct icmp *icmp;
    icmp->icmp_code = 0;
    icmp->icmp_cksum = 0;
    icmp->icmp_type = ICMP_ECHO;

    struct timeval *timeval;

    this->test_result = 1;
}