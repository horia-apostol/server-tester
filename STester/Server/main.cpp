#include "login_service.h"
#include "server_network.h"
#include "unistd.h"
#include "sys/wait.h"
#include "tests_repo.h"
#include "users_repository.h"
#include "service.h"

int main() {
    ServerNetwork network(INADDR_ANY, 2024);
    network.create_server_socket(AF_INET, SOCK_STREAM, 0);
    network.bind_socket();
    network.listen_for_clients(5);
    while (1) {
        network.accept_clients();
        pid_t pid = fork();
        if (pid > 0) {
            network.close_client_socket();
            while (waitpid(-1, NULL, WNOHANG))
                continue;
        }
        else if (pid == 0) {
            network.close_server_socket();
            UserRepository users_repo;
            TestRepo test_repository;
            ServerController GRASPcontroller(network, test_repository);
            LoginService login_controller(users_repo, network);
            login_controller.start_login_service();
            GRASPcontroller.start_controller();
        }
        network.close_client_socket();
    }
}
