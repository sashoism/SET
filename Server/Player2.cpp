#include "Player2.h"
#include "packets/S_LEAVEGAME.h"
#include "packets/S_ONJOIN.h"
#include "packets/S_FOUNDSET.h"
#include "packets/S_CARDSELECTED.h"

Player2::Player2(QObject* parent, Player* playerObject) : Player(0), setsFound(_setsFound) {
    setObjectName(playerObject->getName());
    this->playerObject = playerObject;
    connection = playerObject->connection;
    Game* game = static_cast<Game*>(parent);
    connection->runnables[4] = new S_ONJOIN(this); connection->runnables[4]->setAutoDelete(false);
    connection->runnables[5] = new S_FOUNDSET(this, game); connection->runnables[5]->setAutoDelete(false);
    connection->runnables[6] = new S_CARDSELECTED(); connection->runnables[6]->setAutoDelete(false);
    connection->runnables[7] = new S_LEAVEGAME(); connection->runnables[7]->setAutoDelete(false);
    connect(connection->runnables[4], &Task::response, connection, &Connection::write);
    connect(connection->runnables[5], &Task::response, game, &Game::broadcast);
    connect(static_cast<S_CARDSELECTED*>(connection->runnables[6]), &S_CARDSELECTED::_response, game, &Game::cardSelected);
    connect(static_cast<S_LEAVEGAME*>(connection->runnables[7]), &S_LEAVEGAME::_response, this, &Player2::deleteLater);
    connect(game, &Game::broadcast, connection, &Connection::write);
    _setsFound = 0;
    setParent(parent);
}
