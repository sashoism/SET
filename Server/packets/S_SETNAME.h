#ifndef S_SETNAME_H
#define S_SETNAME_H

#include "Player.h"
#include "Task.h"

class S_SETNAME : public Task {
public:
    S_SETNAME(Player* player) {
        this->player = player;
    }
protected:
    void run() {
        QByteArray r(1, char(1));
        r.append(player->setName(buffer));
        emit response(r);
    }
private:
    Player* player;
};

#endif
