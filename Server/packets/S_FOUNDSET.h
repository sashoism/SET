#ifndef S_FOUNDSET_H
#define S_FOUNDSET_H

#include "Game.h"
#include "Task.h"

class S_FOUNDSET : public Task {
    Q_OBJECT
public:
    S_FOUNDSET(Player2* player, Game* game) {
        this->player = player;
        timer = new QTimer; timer->setSingleShot(true);
        connect(this, SIGNAL(timerStart(int)), timer, SLOT(start(int)));
        connect(timer, &QTimer::timeout, game, &Game::selectingPlayerTimedOut);
        connect(game, &Game::stopTimeout, timer, &QTimer::stop);
    }
    ~S_FOUNDSET() { timer->deleteLater(); }
protected:
    void run() {
        QByteArray r(1, char(5));
        Game* game = static_cast<Game*>(player->parent());
        if (!game->currentlySelecting()) {
            r.append(player->getName());
            game->setSelectingPlayer(player);
            emit response(r);
            emit timerStart(5000);
        }
    }
private:
    Player2* player;
    QTimer* timer;
signals:
    void timerStart(int);
};


#endif // S_FOUNDSET_H

