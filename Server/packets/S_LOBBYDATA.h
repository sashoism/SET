#ifndef S_LOBBYDATA_H
#define S_LOBBYDATA_H

#include "Server.h"
#include "Task.h"

class S_LOBBYDATA : public Task {
protected:
    void run() {
        QByteArray r(Server::getInstance()->gameList());
        emit response(r);
    }
};

#endif
