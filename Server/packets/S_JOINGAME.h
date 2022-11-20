#ifndef S_JOINGAME_H
#define S_JOINGAME_H

#include "Game.h"
#include "Player2.h"
#include "Server.h"
#include "Task.h"

class S_JOINGAME : public Task {
    Q_OBJECT
public:
    S_JOINGAME(Player* player) {
        this->player = player;
    }
protected:
    void run() {
        QByteArray r(1, char(2)); r.append(char(0));
        QString name = QString::fromUtf8(buffer); name.truncate(name.indexOf('\''));
        Player2* testPlayer2 = Server::getInstance()->findChild<Player2*>(player->getName());
        Game* game = Server::getInstance()->findChild<Game*>(name);
        if (testPlayer2 == nullptr && game != nullptr && game->activePlayers() < MAX_PLAYERS) {
            emit joinGame(player, game);
            r[1] = 1;
        }
        emit response(r);
    }
private:
    Player* player;
signals:
    void joinGame(Player*, Game*);
};

#endif
