#ifndef SOCKETSTATEMACHINE_H
#define SOCKETSTATEMACHINE_H

#include <thread>
#include <iostream>
#include "SocketState.h"
#include "TicksPrinter.h"
#include "CircularBuffer.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include <websocketpp/client.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/context.hpp>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/transport/asio/security/tls.hpp>

namespace Deriv {
    typedef websocketpp::client<websocketpp::config::asio_tls_client> client;

    using websocketpp::lib::bind;
    using websocketpp::lib::placeholders::_1;
    using websocketpp::lib::placeholders::_2;

    typedef websocketpp::config::asio_client::message_type::ptr message_ptr;
    typedef websocketpp::lib::shared_ptr<boost::asio::ssl::context> context_ptr;

    static const std::string WSS_ADDRESS = R"(wss://ws.binaryws.com/websockets/v3?app_id=1089)";

    static const std::string AUTH_MESSAGE = R"({"authorize": "JrrXdWTVc7rk5Nt"})";

    /**
     * Not sure that I have to send exactly this json
     */
    static const std::string BUY_CONTRACT_MESSAGE = R"({"buy": 1,
                                                       "subscribe": 1,
                                                       "price": 10,
                                                       "parameters": {
                                                            "amount": 10,
                                                            "basis": "stake",
                                                            "contract_type": "CALL",
                                                            "currency": "USD",
                                                            "duration": 1,
                                                            "duration_unit": "m",
                                                            "symbol": "R_10"
                                                       }
                                                   }
                                                )";

class SocketStateMachine {
    private:
        SocketStateMachine() = default;

        client c;

        SocketState state = SocketState::INIT;

        CircularBuffer ringBuffer;

        TicksPrinter ticksPrinter;

    public:
        static SocketStateMachine &instance() {
            static SocketStateMachine INSTANCE;
            return INSTANCE;
        }

        static log4cplus::Logger& getLogger() {
            static log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("Deriv.SocketStateMachine"));
            return logger;
        }


        SocketState getState() {
            return state;
        }

        CircularBuffer &getRingBuffer() {
            return ringBuffer;
        }

        void authenticate() {
            state = SocketState::AUTH;
        }

        void buy_contract() {
            state = SocketState::BUY_CONTRACT;
        }

        void subscribe_on_ticks() {
            state = SocketState::TICKS;
        }

        void shutdown() {
            state = SocketState::SHUTDOWN;
        }

        void run();

        static log4cplus::Logger logger;
};
}
#endif
