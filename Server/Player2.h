#ifndef PLAYER2_H
#define PLAYER2_H

#include "Player.h"

class Player2 : public Player
{
    Q_OBJECT
public:
    const int& setsFound;
    explicit Player2(QObject* parent = 0, Player* playerObject = 0);
    void foundSet() { _setsFound ++; }
private:
    Player* playerObject;
    int _setsFound;
signals:
    void leftGame();
};

#endif // PLAYER2_H
