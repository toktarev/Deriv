#ifndef TICKSPRINTER_H
#define TICKSPRINTER_H

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

namespace Deriv {
    class TicksPrinter {
    private:
        std::thread ticksPrinterThread;

        void ticksPrinter();

    public:
        static log4cplus::Logger& getLogger() {
            static log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("Derive.TicksPrinter"));
            return logger;
        }

        TicksPrinter() {
            ticksPrinterThread = std::thread{&Deriv::TicksPrinter::ticksPrinter, this};
        }

        ~TicksPrinter() {
            ticksPrinterThread.join();
        }
    };
}

#endif //TICKSPRINTER_H
