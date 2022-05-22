#include "SocketStateMachine.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/initializer.h>

int main(int argc, char **argv) {
   log4cplus::initialize();
   log4cplus::PropertyConfigurator::doConfigure("./bin/log4cplus_configure.ini");
   Deriv::SocketStateMachine::instance().run();
}
