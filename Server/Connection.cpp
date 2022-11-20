#include "Connection.h"
#include "Player.h"
#include "Server.h"
#include "packets/S_LOBBYDATA.h"
#include "packets/S_SETNAME.h"
#include "packets/S_JOINGAME.h"
#include "packets/S_ADDGAME.h"

Connection::Connection(QObject* parent, QTcpSocket* socket) : QObject(parent), socketDescriptor(_socketDescriptor) {
    this->socket = socket;
    _socketDescriptor = socket->socketDescriptor();
    connect(socket, &QTcpSocket::disconnected, this, &Connection::disconnected);
    connect(socket, &QTcpSocket::readyRead, this, &Connection::readyRead);
    qDebug("%s Successfully connected (ID: %d)", LOG_TIME, _socketDescriptor);
    connect(Server::getInstance(), &Server::broadcast, this, &Connection::write);
    runnables = new Task*[Server::runnablesCount]; for (int i = 0; i < Server::runnablesCount; i ++) runnables[i] = nullptr;
    runnables[0] = new S_LOBBYDATA();
    runnables[1] = new S_SETNAME((Player*) parent);
    runnables[2] = new S_JOINGAME((Player*) parent);
    runnables[3] = new S_ADDGAME((Player*) parent);
    connect(static_cast<S_JOINGAME*>(runnables[2]), &S_JOINGAME::joinGame, Server::getInstance(), &Server::joinGame);
    connect(static_cast<S_ADDGAME*>(runnables[3]), &S_ADDGAME::createGame, Server::getInstance(), &Server::createGame);
    for (int i = 0; i < Server::runnablesCount; i ++) {
        if (runnables[i] != nullptr) {
            runnables[i]->setAutoDelete(false);
            connect(runnables[i], &Task::response, this, &Connection::write);
        }
    }
}

void Connection::readyRead() {
    QList<QByteArray> buffer = socket->readAll().split('\x1f');
    for (int i = 0; i < buffer.size(); i ++) {
        if (!buffer.at(i).isEmpty()) {
            unsigned int firstByte = (unsigned int) buffer.at(i).at(0);\
            if (firstByte >= Server::runnablesCount) {
                qDebug("%s Received illegal packet, disconnecting (ID: %d)", LOG_TIME, socketDescriptor);
                qDebug() << "Packet data:" << buffer.at(i);
                socket->disconnectFromHost();
                return;
            }
            runnables[firstByte]->setBuffer(buffer.at(i));
            QThreadPool::globalInstance()->start(runnables[firstByte]);
        }
    }
}

void Connection::write(QByteArray buffer) {
    buffer.append('\x1f');
    socket->write(buffer);
    socket->flush();
}

void Connection::disconnected() {
    qDebug("%s Disconnected (ID: %d)", LOG_TIME, _socketDescriptor);
    for (int i = 0; i < Server::runnablesCount; i ++) {
        if (runnables[i] != nullptr) runnables[i]->deleteLater();
    }
    socket->deleteLater();
}
