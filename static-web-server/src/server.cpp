//
// Created by Dmitry Varin on 27.02.2022.
//
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "server.h++"

bool Server::init() {
    this->_server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(_server_socket == -1) {
        std::cerr << "Can not create socket\n";
        return false;
    }

    // Set server address
    this->_server_addr.sin_family = AF_INET;
    this->_server_addr.sin_port = htons(this->_server_port);
    this->_server_addr.sin_addr.s_addr = INADDR_ANY;
    // Binding the socket to the address
    if(bind(this->_server_socket, (struct sockaddr*) &this->_server_addr, sizeof(this->_server_addr)) == -1) {
        std::cerr << "Failed to bind the socket to address " << errno << std::endl;
        return false;
    }

    // Setting the socket to passive
    listen(_server_socket, _queue_connections);

    return true;
}

void Server::run() {
    while(true) {
        // Accept the connection
        socklen_t addr_size = sizeof(this->_client_addr);
        this->_client_socket = accept(_server_socket, (struct sockaddr*) &this->_client_addr, &addr_size);
//        time_t acceptTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
//        std::string acceptTime_s = ctime(&acceptTime);
//        std::cout << acceptTime_s.substr(11, 8) << " :: ";

        // Displaying the IP of the peer app
        char peerIP[INET_ADDRSTRLEN] = {0};
        if(inet_ntop(AF_INET, &this->_client_addr.sin_addr, peerIP, sizeof(peerIP))) {
            std::cout << "Accepted connection with " << peerIP << "\n";
        } else {
            std::cout << "Failed to get the IP of the client\n";
            return;
        }

        this-> _queueMutex.lock();
        this->_requestQueue.push(_client_socket);
        this-> _cv.notify_one();
        this->_queueMutex.unlock();
        std::cout << "Pushed request to the queue\n";
    }
}

void Server::_handle_request() {
    std::unique_lock<std::mutex> lock(this->_queueMutex, std::defer_lock);
    int client_socket = -1;

    while (true) {
        // Pop the client socket from the queue
        lock.lock();

        this->_cv.wait(lock, [this](){ return !this->_requestQueue.empty();});
        client_socket = this->_requestQueue.front();
        this->_requestQueue.pop();
        lock.unlock();

        char req[2 * REQ_SIZE];
        recv(client_socket, req, sizeof(req), 0);
        std::cout << "Client Request : \n" << req << "\n";
        std::string receive = "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\n";
        send(client_socket, receive.c_str(),sizeof(receive), 0);

        // Closing connection
        close(client_socket);

    }

}
