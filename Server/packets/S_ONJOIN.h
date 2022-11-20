#ifndef S_ONJOIN_H
#define S_ONJOIN_H

#include "Game.h"
#include "Task.h"

class S_ONJOIN : public Task {
public:
    S_ONJOIN(Player2* player) {
        this->player = player;
    }
protected:
    void run() {
        Game* game = static_cast<Game*>(player->parent());
        QByteArray r(game->cardsList());
        r.append(";"); r.append(game->currentlySelecting());
        emit response(r);
    }
private:
    Player2* player;
};

#endif // S_ONJOIN_H

