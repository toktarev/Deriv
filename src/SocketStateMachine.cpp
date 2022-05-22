#include "SocketStateMachine.h"
#include "JsonValidator.h"
#include <chrono>
#include <utility>
#include "rapidjson/schema.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

namespace Deriv {
    bool verify_certificate(const char *hostname,
                            bool preverified,
                            boost::asio::ssl::verify_context &ctx) {
        return true;
    }

    context_ptr on_tls_init(const char *hostname, const websocketpp::connection_hdl &) {
        context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
        ctx->set_options(boost::asio::ssl::context::default_workarounds |
                         boost::asio::ssl::context::no_sslv2 |
                         boost::asio::ssl::context::no_sslv3 |
                         boost::asio::ssl::context::single_dh_use);
        ctx->set_verify_mode(boost::asio::ssl::verify_peer);
        ctx->set_verify_callback(bind(&verify_certificate, hostname, websocketpp::lib::placeholders::_1,
                                      websocketpp::lib::placeholders::_2));
        return ctx;
    }

    void on_open(client *c, websocketpp::connection_hdl hdl) {
        SocketStateMachine::instance().authenticate();
        c->send(std::move(hdl), AUTH_MESSAGE.c_str(), websocketpp::frame::opcode::text);
    }

    void on_close(websocketpp::connection_hdl hdl) {
        LOG4CPLUS_INFO(SocketStateMachine::getLogger(), LOG4CPLUS_TEXT("Shutting down state machine ! "));
        SocketStateMachine::instance().shutdown();
    }

    void on_fail(websocketpp::connection_hdl hdl) {
        LOG4CPLUS_INFO(SocketStateMachine::getLogger(), LOG4CPLUS_TEXT("Shutting down state machine !"));
        SocketStateMachine::instance().shutdown();
    }

    bool processAuth(const message_ptr &msg) {
        std::string error{};

        if (JsonValidator::validateJson(msg->get_payload(), Deriv::JsonValidator::authSchema(), error)) {
            LOG4CPLUS_INFO(SocketStateMachine::getLogger(),
                           LOG4CPLUS_TEXT("Authentication success !" + msg->get_payload()));
        } else {
            LOG4CPLUS_INFO(SocketStateMachine::getLogger(), LOG4CPLUS_TEXT("Authentication failre !" << error));
            SocketStateMachine::instance().shutdown();
            return false;
        }

        SocketStateMachine::instance().buy_contract();
        return true;
    }

    bool processBuyContract(const message_ptr &msg) {
        std::string error{};

        if (JsonValidator::validateJson(msg->get_payload(), Deriv::JsonValidator::buyContractSchema(), error)) {
            LOG4CPLUS_INFO(SocketStateMachine::getLogger(),
                           LOG4CPLUS_TEXT("Buy contract success !" + msg->get_payload()));
        } else {
            LOG4CPLUS_INFO(SocketStateMachine::getLogger(), LOG4CPLUS_TEXT("Buy contract failre !" << error));
            SocketStateMachine::instance().shutdown();
            return false;
        }

        SocketStateMachine::instance().subscribe_on_ticks();
        return true;
    }

    /*
     * I am not sure that this is actually that ticks which I should print
     * */
    bool processTicks(const message_ptr &msg) {
        std::string error{};

        if (!JsonValidator::validateJson(msg->get_payload(), Deriv::JsonValidator::ticksSchema(), error)) {
            LOG4CPLUS_INFO(SocketStateMachine::getLogger(), LOG4CPLUS_TEXT("Ticks failre !" << error));
            return false;
        }

        Holder holder{};
        holder.payload = msg->get_payload();
        auto duration = std::chrono::system_clock::now().time_since_epoch();
        holder.timeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        holder.isSuccess = true;
        SocketStateMachine::instance().getRingBuffer().put(holder);

        return true;
    }

    void on_message(client *c, websocketpp::connection_hdl hdl, const message_ptr &msg) {
        if (SocketStateMachine::instance().getState() == SocketState::AUTH) {
            if (!processAuth(msg)) {
                websocketpp::lib::error_code code{};
                c->close(hdl, 0, "Json Failure", code);
                return;
            }
            
            c->send(hdl, BUY_CONTRACT_MESSAGE, websocketpp::frame::opcode::text);
            return;
        }

        if (SocketStateMachine::instance().getState() == SocketState::BUY_CONTRACT) {
            if (!processBuyContract(msg)) {
                websocketpp::lib::error_code code{};
                c->close(hdl, 0, "Json Failure", code);
                return;
            }

            return;
        }

        if (SocketStateMachine::instance().getState() == SocketState::TICKS) {
            if (!processTicks(msg)) {
                websocketpp::lib::error_code code{};
                c->close(hdl, 0, "Json Failure", code);
                return;
            }

            return;
        }

        LOG4CPLUS_INFO(SocketStateMachine::getLogger(),
                       LOG4CPLUS_TEXT("Invalid state, message ") << msg->get_payload());
        websocketpp::lib::error_code code{};
        c->close(hdl, 0, "Json Failure", code);
    }

    void SocketStateMachine::run() {
        client c{};

        c.set_access_channels(websocketpp::log::alevel::all);
        c.clear_access_channels(websocketpp::log::alevel::frame_payload);

        c.init_asio();

        // Register our message handler
        c.set_tls_init_handler(bind(&on_tls_init, WSS_ADDRESS.c_str(), websocketpp::lib::placeholders::_1));
        c.set_open_handler(bind(&on_open, &c, websocketpp::lib::placeholders::_1));
        c.set_close_handler(bind(&on_close, websocketpp::lib::placeholders::_1));
        c.set_fail_handler(bind(&on_fail, websocketpp::lib::placeholders::_1));

        c.set_message_handler(
                bind(&on_message, &c, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));


        websocketpp::lib::error_code ec;
        client::connection_ptr con = c.get_connection(WSS_ADDRESS, ec);

        if (ec) {
            LOG4CPLUS_INFO(SocketStateMachine::getLogger(),
                           LOG4CPLUS_TEXT("Could not create connection because: " + ec.message()));
            return;
        }

        c.connect(con);
        c.run();
    };
}