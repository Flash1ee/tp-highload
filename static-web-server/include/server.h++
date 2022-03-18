//
// Created by Dmitry Varin on 27.02.2022.
//
#pragma once

#include <vector>
#include <queue>
#include <mutex>
#include <netinet/in.h>
#include <iostream>
#include <thread>
#include "constants.h++"
#include <cstring>
#include <condition_variable>


class Server {
public:
    Server() = delete;
    explicit Server(size_t poolSize = POOL_SIZE, int port = PORT, int countConn = CNT_CONN)
    :_pool_size(poolSize), _server_port(port), _queue_connections(CNT_CONN) {
        this->_server_socket = 0, this->_client_socket =  0;
        memset(&this->_server_addr, 0, sizeof(struct sockaddr_in));
        memset(&this->_client_addr, 0, sizeof(struct sockaddr_in));
        this-> _thread_pool.resize(this->_pool_size); // Set the thread pool size
        for(size_t i = 0; i < this->_pool_size; i++) {
            this->_thread_pool[i] = std::thread([this](){this->_handle_request();});
        }
    };
    bool init();
    void run();
private:
    int _server_socket;
    int _client_socket;
    struct sockaddr_in _server_addr;
    struct sockaddr_in _client_addr;
    int _server_port;
    std::vector<std::thread> _thread_pool;
    size_t _pool_size;

    std::queue<int> _requestQueue;

    int _queue_connections;

    std::mutex _queueMutex;
    std::condition_variable _cv;

    void _handle_request();

    bool _loadConfig();
};

