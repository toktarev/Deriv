#ifndef SOCKETSTATE_H
#define SOCKETSTATE_H

namespace Deriv {
    enum SocketState {
        INIT,
        AUTH,
        BUY_CONTRACT,
        TICKS,
        SHUTDOWN
    };
}

#endif //SOCKETSTATE_H
