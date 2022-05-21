#include <iostream>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

int main() {
    net::io_context ioc;

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
