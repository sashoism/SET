#ifndef S_ADDGAME_H
#define S_ADDGAME_H

#include "Game.h"
#include "Server.h"
#include "Task.h"

class S_ADDGAME : public Task {
    Q_OBJECT
public:
    S_ADDGAME(Player* player) {
        this->player = player;
    }
protected:
    void run() {
        QByteArray r(1, char(3)); r.append(char(0));
        if (Server::getInstance()->findChild<Game*>(player->getName()) == nullptr) {
            emit createGame(player);
            r[1] = 1;
        }
        emit response(r);
    }
private:
    Player* player;
signals:
    void createGame(Player*);
};

#endif
