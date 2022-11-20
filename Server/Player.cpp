#include "Player.h"
#include "Server.h"
#include "Game.h"
#include "Utility.h"

Player::Player(QObject* parent, QTcpSocket* socket) : QObject(parent) {
    connection = new Connection(this, socket);
}

QString Player::getName() {
    return objectName();
}

char Player::setName(QString name) {
    if (name.size() > MAX_CHARS_PNAME) return char(0);
    for (int i = 0; i < name.size(); i ++) {
        if ((name[i].script() != QChar::Script_Latin) && (name[i].script() != QChar::Script_Cyrillic) && name[i] != ' ') return char(0);
    }
    if (Server::getInstance()->findChild<Player*>(name) != nullptr) return char(2);
    if (Server::getInstance()->findChild<Game*>(name) != nullptr) return char(3);
    if(Server::getInstance()->findChild<Player2*>(name) != nullptr) return char(4);
    setObjectName(name);
    qDebug() << LOG_TIME << "Set player name to" << name << "(ID:" << qPrintable(QString::number(connection->socketDescriptor) + ')');
    return char(1);
}
