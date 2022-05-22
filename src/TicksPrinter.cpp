#include "SocketStateMachine.h"
#include "CircularBuffer.h"
#include <chrono>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

using namespace std::chrono_literals;

namespace Deriv {
    void TicksPrinter::ticksPrinter() {
        while (SocketStateMachine::instance().getState() != SocketState::SHUTDOWN) {
            Holder holder = SocketStateMachine::instance().getRingBuffer().peek();

            if (holder.isSuccess) {
                LOG4CPLUS_INFO(TicksPrinter::getLogger(), LOG4CPLUS_TEXT("Received tick: "+holder.payload));
            } else {
                LOG4CPLUS_INFO(TicksPrinter::getLogger(), LOG4CPLUS_TEXT("No actual (1 minute left) ticks "));
                std::this_thread::sleep_for(1000ms);
            }
        }
    }
}

