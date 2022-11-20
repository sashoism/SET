#include "Game.h"
#include "Player.h"
#include "Server.h"

Server* Server::instance = nullptr;
Server* Server::getInstance() {
    if (instance == nullptr) {
        instance = new Server;
    }
    return instance;
}

void Server::start(quint16 port) {
    if (!listen(QHostAddress::Any, port)) {
        qDebug() << LOG_TIME << "Could not start server, terminating ..";
    } else {
        qDebug() << LOG_TIME << "Listening for connections on port" << port;
    }
    broadcastTimer = new QTimer(this);
    connect(broadcastTimer, &QTimer::timeout, this, &Server::broadcastGameList);
    connect(this, &Server::broadcast, this, &Server::restartTimer);
    broadcastTimer->start(SCHEDULED_BROADCAST_INTERVAL_SEC * 1000);
}

QByteArray Server::gameList() {
    QByteArray gameList(1, char(0));
    QList<Game*> list(Server::getInstance()->findChildren<Game*>());
    gameList.append(QString::number(list.size()) + ";" + QString::number(MAX_PLAYERS) + "\n");
    for (int i = 0; i < list.size(); i ++) {
        gameList.append(list.at(i)->getName() + "'s room;" + QString::number(list.at(i)->activePlayers()) + "\n");
    }
    return gameList;
}

void Server::createGame(Player* creator) {
    Game* newGame = new Game(this, creator);
    connect(newGame, &Game::destroyed, this, &Server::broadcastGameList);
    connect(newGame, &Game::newPlayer, this, &Server::broadcastGameList);
    connect(newGame, &Game::playerLeft, this, &Server::broadcastGameList);
    qDebug() << LOG_TIME << creator->getName() << "created a new game";
    broadcastGameList();
}

void Server::joinGame(Player* player, Game* game) {
    Player2* newPlayer_inGame = new Player2(game, player);
    connect(player, &Player::destroyed, newPlayer_inGame, &Player2::deleteLater);
}

void Server::restartTimer(QByteArray) {
    broadcastTimer->start();
}

void Server::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket* socket = new QTcpSocket(this); socket->setSocketDescriptor(socketDescriptor);
    qDebug("%s New connection from %s:%d", LOG_TIME, qPrintable(socket->peerAddress().toString().remove(0, 7)), socket->peerPort());
    Player* newPlayer = new Player(this, socket);
    connect(socket, &QTcpSocket::destroyed, newPlayer, &Player::deleteLater);
}
